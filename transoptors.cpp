/*
 * transoptors.cpp
 *
 *  Created on: 10 sie 2017
 *      Author: tomek
 */

#include "transoptors.h"

// niezasloniety -> lo
// zasloniety    -> hi

Transoptors::Transoptors()
{
	val = 0;
}

bool Transoptors::Check()
{
	if(TRANSOPTOR_CONTROL_PIN & (1 << TRANSOPTOR_CONTROL))
		return false;
	else
		return true;
}

uint8_t Transoptors::Read()
{
	val = 0;
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_1)) val |= (1 << 0);
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_2)) val |= (1 << 1);
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_3)) val |= (1 << 2);
	return val;
}
