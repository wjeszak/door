/*
 * door.h
 *
 *  Created on: 31 sie 2017
 *      Author: tomek
 */

#ifndef DOOR_H_
#define DOOR_H_

#include <inttypes.h>

#define DOOR_STATE_CLOSED 					0xC0
#define DOOR_STATE_OPENED_AND_CLOSED 		0xD0
#define DOOR_STATE_DOOR_NOT_YET_OPENED 		0x80

#define F03_OPTICAL_SWITCHES_FAULT 			(0x03 << 8)
#define F05_ELECTROMAGNET_FAULT 			(0x05 << 8)
#define F06_CLOSE_THE_DOOR 					(0x06 << 8)

class Door
{
public:
	Door();
	uint16_t GetState();
	void SetState(uint8_t state);
private:
	uint16_t state;
};

extern Door door;

#endif /* DOOR_H_ */
