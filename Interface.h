/*
 * Interface.h
 *
 *  Created on: Feb 4, 2021
 *      Author: george
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include "Sensor.h"
#include <list>
#include <ostream>

struct SmallSensor {
	uint16_t	hardwareAddress;
	uint8_t		signalType;
	float		scalingFactor;
	float		offset;
};

class Interface {
private:
			std::ostream * 		outputStream				= NULL;
			void *				timeTriggeredThread			= NULL;
			std::list<Sensor>	sensorsList;
			void				readAllSensors		(void																				);


public:
	Interface();
	virtual ~Interface();

			bool				addSensor			(const uint16_t& hardwareAddress, const uint8_t& signalType							);
			bool				addSensor			(const uint16_t& hardwareAddress, const char * const signalType						);
			bool				start				(void																				);
			bool				remSensor			(const uint16_t& hardwareAddress													);
			void				setScaling			(const uint16_t& hardwareAddress, const float& scalingFactor, const float& offset	);
			std::list<Sensor>	getSensorsList		(void																				);
			void				setOutputStream		(std::ostream* outputStream															)
													{this->outputStream = outputStream;													}
			void				readAllSensorsTS	(void																				);
			void				halt				(void																				);
			void 				toYAML				(void																				);
			void				readAndReport		(void																				);

};

#endif /* INTERFACE_H_ */
