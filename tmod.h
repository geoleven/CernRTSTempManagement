/*
 * tmod.h
 *
 *  Created on: Feb 4, 2021
 *      Author: george
 */

#ifndef TMOD_H_
#define TMOD_H_

#include <stdint.h>

int16_t tmodReadAdc(uint16_t hardwareAddress);
uint16_t tmodMaxAdcs(void);


#endif /* TMOD_H_ */
