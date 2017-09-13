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
	DDRD |= (1 << 4);
	DDRC |= (1 << 1);
	DDRD |= (1 << 5);
	PORTD |= (1 << 4);
	PORTC |= (1 << 1);
	PORTD |= (1 << 5);

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

void Transoptors::Read()
{
	door_data.val = 0;
	PORTD |= (1 << 4) | (1 << 5);
	PORTC |= (1 << 1);
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_1)) { PORTD &= ~(1 << 4); door_data.val |= (1 << 0); }
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_2)) { PORTC &= ~(1 << 1); door_data.val |= (1 << 1); }
	if(TRANSOPTORS_PIN & (1 << TRANSOPTOR_3)) { PORTD &= ~(1 << 5); door_data.val |= (1 << 2); }
}

ISR(PCINT2_vect)
{
	transoptors.Read();
	door.EV_ChangeVal(&door_data);
}
