/*
 * electromagnet.h
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#ifndef ELECTROMAGNET_H_
#define ELECTROMAGNET_H_

#include <inttypes.h>
#include "state_machine.h"

#if defined (__AVR_ATmega88P__)
#define ELECTROMAGNET_CTRL_DDR 			DDRC
#define ELECTROMAGNET_CTRL_PORT 		PORTC
#define ELECTROMAGNET_CTRL_PIN 			2

#define ELECTROMAGNET_TEST_COIL_DDR		DDRB
#define ELECTROMAGNET_TEST_COIL_PPIN 	PINB
#define ELECTROMAGNET_TEST_COIL_PIN		1

#define ELECTROMAGNET_INIT 				ELECTROMAGNET_CTRL_DDR |= (1 << ELECTROMAGNET_CTRL_PIN); ELECTROMAGNET_OFF

#define ELECTROMAGNET_ON 				ELECTROMAGNET_CTRL_PORT &= ~(1 << ELECTROMAGNET_CTRL_PIN)
#define ELECTROMAGNET_OFF 				ELECTROMAGNET_CTRL_PORT |=  (1 << ELECTROMAGNET_CTRL_PIN)
#endif

#if defined (__AVR_ATmega8__)
#define ELECTROMAGNET_CTRL_DDR 			DDRD
#define ELECTROMAGNET_CTRL_PORT 		PORTD
#define ELECTROMAGNET_CTRL_PIN 			6

#define ELECTROMAGNET_TEST_COIL_DDR		DDRD
#define ELECTROMAGNET_TEST_COIL_PPIN 	PIND
#define ELECTROMAGNET_TEST_COIL_PIN		7

#define ELECTROMAGNET_INIT 				ELECTROMAGNET_CTRL_DDR |= (1 << ELECTROMAGNET_CTRL_PIN); ELECTROMAGNET_OFF

#define ELECTROMAGNET_ON 				ELECTROMAGNET_CTRL_PORT &= ~(1 << ELECTROMAGNET_CTRL_PIN)
#define ELECTROMAGNET_OFF 				ELECTROMAGNET_CTRL_PORT |=  (1 << ELECTROMAGNET_CTRL_PIN)

#endif

class ElectromagnetData : public EventData
{
public:

};

class Electromagnet
{
public:
	Electromagnet();
};

extern Electromagnet electromagnet;

#endif /* ELECTROMAGNET_H_ */
