/*
 * transoptors.cpp
 *
 *  Created on: 10 sie 2017
 *      Author: tomek
 */

#include <avr/interrupt.h>
#include "transoptors.h"
#include "door.h"

// niezasloniety -> lo
// zasloniety    -> hi

Transoptors::Transoptors()
{
//	DDRD |= (1 << 4);
//	DDRC |= (1 << 1);
//	DDRD |= (1 << 5);
//	PORTD |= (1 << 4);
//	PORTC |= (1 << 1);
//	PORTD |= (1 << 5);

	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT23) | (1 << PCINT19) | (1 << PCINT18);
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
	//door_data.val = 0;
	//PORTD |= (1 << 4) | (1 << 5);
	//PORTC |= (1 << 1);
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_1)) { v |= (1 << 0); } // PORTD &= ~(1 << 4);
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_2)) { v |= (1 << 1); } // PORTC &= ~(1 << 1);
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_3)) { v |= (1 << 2); } // PORTD &= ~(1 << 5);
	return v;
}

ISR(PCINT2_vect)
{
	door.EV_ChangeVal(NULL);
}
