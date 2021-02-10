/*
 * Interface.cpp
 *
 *  Created on: Feb 4, 2021
 *      Author: george
 */

#include "Interface.h"
#include "tmod.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <unistd.h>			//sleep
#include <exception>
#include <cstring>			// std::strcmp
#include <map>


#define SLEEPTIMER 1
#define DEBUG false






/*_____ MACRO define the lock unlock mechanism _____*/
#define LOCK unsigned int duration = 1; \
	/*Critical Section*/ \
	while (sensorReadLock.test_and_set()) { \
		usleep(duration); \
		if (duration < 1000000) \
			duration = duration * 2; \
	}

#define UNLOCK sensorReadLock.clear(); \
	/*End of Critical Section*/

/*__________________________________________________*/


struct strCmp {
	bool operator()(const char * mapElement, const char * userElement) const {
		return std::strncmp(mapElement, userElement, std::strlen(mapElement)) <0;
	}
};
const static std::map<const char*, uint8_t, strCmp> sensorType {
	{"0-10V"	, (uint8_t)1},
	{"4-20mA"	, (uint8_t)2}
};

static std::atomic_flag sensorReadLock = ATOMIC_FLAG_INIT;
static std::atomic<bool> end (false);

void timeTriggeredReadSensorsThread(Interface* interfaceInstance) {
	while(true) {
		interfaceInstance->readAllSensorsTS();
		// Waking up to see if end of program is triggered while waiting for next read:
		for (uint8_t counter = 0; counter < SLEEPTIMER; ++counter) {
			sleep(1);
			if (end)
				return;
		}
	}
}

Interface::Interface() {
}

bool Interface::start(void) {
	if (!end) {
		timeTriggeredThread = (void*)(new std::thread(timeTriggeredReadSensorsThread, this));
		return true;
	}
	return false;
}

Interface::~Interface() {
	halt();
}

bool Interface::addSensor(const uint16_t& hardwareAddress, const uint8_t& signalType) {
	if (hardwareAddress >= 0 && hardwareAddress < tmodMaxAdcs()) {
		LOCK
		{
			for (Sensor& it : sensorsList) {
				if (it.getHardwareAddress() == hardwareAddress) {
					std::cerr << "Hardware address already registered." << std::endl;
					UNLOCK
					return false;
				}
			}
			sensorsList.push_back(Sensor(hardwareAddress, signalType));
		}
		UNLOCK
		return true;
	}
	else {
		std::cerr << "Hardware address out of bounds." << std::endl;
		return false;
	}
}

bool Interface::addSensor(const uint16_t &hardwareAddress, const char * const signalType) {
	uint8_t curType;
	try {
		curType = sensorType.at(signalType);
	} catch (std::exception& e) {
		std::cerr <<	"Invalid signal type used."	<< std::endl;
		if (DEBUG)
			std::cerr <<	"Exception info: "			<< e.what() <<  std::endl;
		return false;
	}
	return addSensor(hardwareAddress, sensorType.at(signalType));
}

bool Interface::remSensor(const uint16_t& hardwareAddress) {
	LOCK
	{
		for (std::list<Sensor>::iterator it = sensorsList.begin(); it != sensorsList.end(); ++it) {
			if ((*it).getHardwareAddress() == hardwareAddress) {
				sensorsList.erase(it);
				UNLOCK
				return true;
			}
		}
	}
	UNLOCK
	return false;
}

float rawToTemp(const int16_t& rawVoltageValue, const float& scalingFactor, const float& offset) {
	return ((scalingFactor * rawVoltageValue) - offset);
}

// Not Thread Safe
void Interface::toYAML() {
	if (outputStream == NULL) {
//		if (DEBUG)
		std::cerr << "Output stream is not set. Please set it before trying to export data." << std::endl;
	}
	float curScalingFactor;
	float curSignalType;
	*outputStream << "SensorReadings:" <<  std::endl;

	for (Sensor& it : sensorsList) {
		curScalingFactor = it.getScalingFactor();
		curSignalType = it.getSignalType();
		*outputStream << "  " << (unsigned int)(it.getHardwareAddress()) << ": " << std::endl;
		*outputStream << "  " << "  " << "Current: " << rawToTemp(it.getCurReading(), curScalingFactor, curSignalType) << std::endl;
		*outputStream << "  " << "  " << "Minimum: " << rawToTemp(it.getMinReading(), curScalingFactor, curSignalType) << std::endl;
		*outputStream << "  " << "  " << "Maximum: " << rawToTemp(it.getMaxReading(), curScalingFactor, curSignalType) << std::endl;
	}

}

void Interface::setScaling(const uint16_t& hardwareAddress, const float& scalingFactor, const float& offset) {
	LOCK
	{
		for (std::list<Sensor>::iterator it = sensorsList.begin(); it != sensorsList.end(); ++it) {
			if ((*it).getHardwareAddress() == hardwareAddress) {
				(*it).setScalingFactor(scalingFactor);
				(*it).setOffset(offset);
				UNLOCK
				return;
			}
		}
	}
	UNLOCK
	return;
}

std::list<Sensor> Interface::getSensorsList() {
	return sensorsList;
}

// Not Thread Safe
void Interface::readAllSensors() {
	uint16_t curTemp;
	for (std::list<Sensor>::iterator it = sensorsList.begin(); it != sensorsList.end(); ++it) {
		curTemp = tmodReadAdc((*it).getHardwareAddress());
		if (curTemp < (*it).getMinReading())
			(*it).setMinReading(curTemp);
		if (curTemp > (*it).getMaxReading())
			(*it).setMaxReading(curTemp);
		(*it).setCurReading(curTemp);
	}
}

// Thread Safe
void Interface::readAllSensorsTS() {
	LOCK
		readAllSensors();
		if (DEBUG)
			std::cerr <<  "Time triggered reading..." << std::endl;
	UNLOCK
}

void Interface::halt(void) {
	if (!end.exchange(true)) {
		if (timeTriggeredThread != NULL) {
			((std::thread*)timeTriggeredThread)->join();
			delete (std::thread*)timeTriggeredThread;
			timeTriggeredThread = NULL;
		}
	}
}

void Interface::readAndReport(void) {
	LOCK
	{
		if (DEBUG)
			std::cerr <<  "Requested reading..." << std::endl;
		readAllSensors();
		toYAML();
	}
	UNLOCK
}

