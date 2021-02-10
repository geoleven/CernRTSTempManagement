/*
 * Sensor.cpp
 *
 *  Created on: Feb 4, 2021
 *      Author: george
 */

#include "Sensor.h"

Sensor::Sensor(void) {
}

Sensor::Sensor(uint16_t hardwareAddress, uint8_t signalType) {
	this->hardwareAddress = hardwareAddress;
	this->signalType = signalType;
}

Sensor::~Sensor() {
}

