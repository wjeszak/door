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
	val = Read();
	state = 0;
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
	uint8_t v = 0;
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_1)) v |= (1 << 0);
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_2)) v |= (1 << 1);
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_3)) v |= (1 << 2);
	return v;
}

uint16_t Transoptors::GetDoorState()
{
//	uint8_t val_tmp = Read();
//	if(val_tmp != 0) state = F06_CLOSE_THE_DOOR << 8;
//	else state = 0;
//	return state;
}
