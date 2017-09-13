/*
 * transoptors.h
 *
 *  Created on: 10 sie 2017
 *      Author: tomek
 */

#ifndef TRANSOPTORS_H_
#define TRANSOPTORS_H_

#include <avr/io.h>
#include <inttypes.h>

#define TRANSOPTORS_DDR 					DDRD
#define TRANSOPTORS_PORT 					PORTD
#define TRANSOPTORS_PIN 					PIND

#define TRANSOPTOR_1 						3
#define TRANSOPTOR_2 						2
#define TRANSOPTOR_3 						7

#define TRANSOPTOR_CONTROL_DDR 				DDRB
#define TRANSOPTOR_CONTROL_PORT 			PORTB
#define TRANSOPTOR_CONTROL_PIN 				PINB
#define TRANSOPTOR_CONTROL 					2

class Transoptors
{
public:
	Transoptors();
	bool Check();
	void Read();
};

extern Transoptors transoptors;

#endif /* TRANSOPTORS_H_ */
