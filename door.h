/*
 * door.h
 *
 *  Created on: 31 sie 2017
 *      Author: tomek
 */

#ifndef DOOR_H_
#define DOOR_H_

#include "state_machine.h"
// ---------------------- Lockerbox ----------------------
#define LOCK_DDR 							DDRB
#define LOCK_PORT 							PORTB
#define LOCK_PPIN							PINB
#define LOCK_PIN 							0			// F, closed -> hi, opened -> lo
// -------------------------------------------------------

#define DOOR_STATE_CLOSED 					0xC0
#define DOOR_STATE_OPENED_AND_CLOSED 		0xD0
#define DOOR_STATE_DOOR_NOT_YET_OPENED		0x80
#define DOOR_STATE_OPENED 					0x00
#define DOOR_STATE_EM_OFF 					0x40
#define DOOR_STATE_EM_OFF_1STOP 			0x41
// reply in command 0x02 (Dynabox)
#define F03_OPTICAL_SWITCHES_FAULT 			0xF0
// reply in command 0x01
#define F05_ELECTROMAGNET_FAULT 			0x01

class DoorData : public EventData
{
public:
	uint8_t val;
};

class Door
{
public:
	Door();
	// Events
	void EV_ChangeVal(DoorData* pdata);
	void SetStatus(uint8_t st);
	uint8_t GetTransVal();
	uint8_t GetSubpos();
	uint8_t GetStatus();
	uint8_t required_position;
	uint8_t sub_pos;
	bool lockerbox_has_been_opened;
private:
	uint8_t status, pos, val, last_val;
	bool zero_achieved;
};

extern Door door;
extern DoorData door_data;

#endif /* DOOR_H_ */
