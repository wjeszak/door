/*
 * stamp.h
 *
 *  Created on: 20 sty 2018
 *      Author: wjeszak
 */

#ifndef STAMP_H_
#define STAMP_H_

#include <inttypes.h>
#include <avr/eeprom.h>

#define STAMP_DDR 			DDRB
#define STAMP_PORT 			PORTB
#define STAMP_PPIN 			PINB
#define STAMP_PIN 			4

struct Stamp_data
{
	uint8_t type_of_hardware;
	uint8_t address;
	uint8_t year;
	uint8_t week;
	uint16_t sn;
};
extern void StampInit();
extern bool IsStampProgrammingMode();

extern Stamp_data stamp_data;
extern Stamp_data EEMEM ee_stamp_data;

#endif /* STAMP_H_ */
