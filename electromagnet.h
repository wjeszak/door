/*
 * electromagnet.h
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#ifndef ELECTROMAGNET_H_
#define ELECTROMAGNET_H_

#include <inttypes.h>
#include "machine.h"

#define ELECTROMAGNET_CTRL_DDR 			DDRC
#define ELECTROMAGNET_CTRL_PORT 		PORTC
#define ELECTROMAGNET_CTRL_PIN 			2

#define ELECTROMAGNET_CHECK_DDR 		DDRB
#define ELECTROMAGNET_CHECK_PIN 		1

#define ELECTROMAGNET_INIT 				ELECTROMAGNET_CTRL_DDR |= (1 << ELECTROMAGNET_CTRL_PIN); ELECTROMAGNET_OFF

#define ELECTROMAGNET_ON 				ELECTROMAGNET_CTRL_PORT &= ~(1 << ELECTROMAGNET_CTRL_PIN)
#define ELECTROMAGNET_OFF 				ELECTROMAGNET_CTRL_PORT |=  (1 << ELECTROMAGNET_CTRL_PIN)

class Electromagnet
{
public:
	Electromagnet();
	void On();
	void Off();
	bool CheckCoil();
};

extern Electromagnet electromagnet;

#endif /* ELECTROMAGNET_H_ */
