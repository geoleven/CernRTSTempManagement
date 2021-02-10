/*
 * Sensor.h
 *
 *  Created on: Feb 4, 2021
 *      Author: george
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <cstdint>

class Sensor {
private:
	uint16_t	hardwareAddress			= -1;
	uint8_t		signalType				= 0;
	float		scalingFactor			= 0;
	float		offset					= 0;
	int16_t		curReading				= 0;
	int16_t		maxReading				= INT16_MIN;
	int16_t		minReading				= INT16_MAX;

public:
				Sensor				(void);
				Sensor				(uint16_t hardwareAddress, uint8_t signalType);
	virtual 	~Sensor				();

	/*Getters*/
	/*________________________________________________________________________________________________________________________*/
	uint16_t	getHardwareAddress	(void)		{return this->hardwareAddress;	}
	uint8_t 	getSignalType		(void)		{return this->signalType;		}
	float		getScalingFactor	(void)		{return this->scalingFactor;	}
	float		getOffset			(void)		{return this->offset;			}
	int16_t		getCurReading		(void)		{return this->curReading;		}
	int16_t		getMinReading		(void)		{return this->minReading;		}
	int16_t		getMaxReading		(void)		{return this->maxReading;		}

	/*Setters*/
	/*________________________________________________________________________________________________________________________*/

	void setHardwareAddress		(const uint16_t&	hardwareAddress)	{this->hardwareAddress		= hardwareAddress;	}
	void setSignalType			(const uint8_t&		signalType)			{this->signalType			= signalType;		}
	void setScalingFactor		(const float&		scalingFactor)		{this->scalingFactor		= scalingFactor;	}
	void setOffset				(const float&		offset)				{this->offset				= offset;			}
	void setCurReading			(const int16_t&		curReading)			{this->curReading			= curReading;		}
	void setMinReading			(const int16_t&		minReading)			{this->minReading			= minReading;		}
	void setMaxReading			(const int16_t&		maxReading)			{this->maxReading			= maxReading;		}
};

#endif /* SENSOR_H_ */
