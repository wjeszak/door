/*
 * transoptors.cpp
 *
 *  Created on: 10 sie 2017
 *      Author: tomek
 */

#include "transoptors.h"

// niezasloniety -> lo
// zasloniety    -> hi

uint16_t GetTransoptors()
{
	uint16_t state = 0;
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_1)) state |= (1 << 0);
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_2)) state |= (1 << 1);
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_3)) state |= (1 << 2);
	return state;
}
