/*
 * electromagnet.h
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#ifndef ELECTROMAGNET_H_
#define ELECTROMAGNET_H_

#include <avr/io.h>
//#include "state_machine.h"

#define ELM_CTRL_DDR		 			DDRC
#define ELM_CTRL_PORT			 		PORTC
#define ELM_CTRL_PPIN			 		PINC
#define ELM_CTRL_PIN		 			2

#define ELM_TEST_COIL_DDR				DDRB
#define ELM_TEST_COIL_PPIN 				PINB
#define ELM_TEST_COIL_PIN				1

#define ELM_ON			 				ELM_CTRL_PORT &= ~(1 << ELM_CTRL_PIN)
#define ELM_OFF			 				ELM_CTRL_PORT |=  (1 << ELM_CTRL_PIN)

#define ELM_INIT 						ELM_CTRL_DDR |= (1 << ELM_CTRL_PIN); ELM_OFF

class Elm
{
public:
	Elm();
	bool ElmOn();
};

extern Elm elm;

#endif /* ELECTROMAGNET_H_ */
