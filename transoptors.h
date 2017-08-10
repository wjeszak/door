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

#define TRANSOPTORS_DDR 		DDRD
#define TRANSOPTORS_PORT 		PORTD
#define TRANSOPTORS_PIN 		PIND

#define TRANSOPTOR_1 			7
#define TRANSOPTOR_2 			2
#define TRANSOPTOR_3 			3

extern uint16_t GetTransoptors();

#endif /* TRANSOPTORS_H_ */
