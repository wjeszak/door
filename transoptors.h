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
#include "modbus_rtu.h"

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

#define F03_OPTICAL_SWITCHES_FAULT 			0x03
#define F05_ELECTROMAGNET_FAULT 			0x05
#define F06_CLOSE_THE_DOOR 					0x06

class Transoptors
{
public:
	Transoptors();
	bool Check();
	uint8_t Read();
	uint16_t GetDoorState();
private:
	uint8_t val;
	uint8_t val_tmp;
	uint16_t state;
};

extern Transoptors transoptors;

#endif /* TRANSOPTORS_H_ */
