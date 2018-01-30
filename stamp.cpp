/*
 * stamp.cpp
 *
 *  Created on: 20 sty 2018
 *      Author: wjeszak
 */

#include <avr/io.h>
#include "stamp.h"

Stamp_data stamp_data;
Stamp_data EEMEM ee_stamp_data;

void StampInit()
{
	STAMP_DDR &= ~(1 << STAMP_PIN);
	STAMP_PORT |= (1 << STAMP_PIN);
	// leds
	DDRD |= (1 << 4);
	DDRC |= (1 << 1);
	DDRD |= (1 << 5);
	PORTD |= (1 << 4);
	PORTC |= (1 << 1);
	PORTD |= (1 << 5);
}

bool IsStampProgrammingMode()
{
	if(!(STAMP_PPIN & (1 << STAMP_PIN)))
		return true;
	else
		return false;
}
