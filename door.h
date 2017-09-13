/*
 * door.h
 *
 *  Created on: 31 sie 2017
 *      Author: tomek
 */

#ifndef DOOR_H_
#define DOOR_H_

#include "state_machine.h"

#define DOOR_STATE_CLOSED 					0xC0
#define DOOR_STATE_OPENED_AND_CLOSED 		0xD0
#define DOOR_STATE_DOOR_NOT_YET_OPENED		0x80
#define DOOR_STATE_OPENED 					0x40
#define DOOR_STATE_OPENED_1STOP				0x01

#define F03_OPTICAL_SWITCHES_FAULT 			0xF0
// reply in command 0x01
#define F05_ELECTROMAGNET_FAULT 			0x01

class DoorData : public EventData
{
public:
	uint8_t val;
};

class Door : public StateMachine
{
public:
	Door();
	// Events
	void EV_ChangeVal(DoorData* pdata);
	void SetStatus(uint8_t st);
	uint8_t GetStatus();
private:
	// States functions
	void ST_Init(DoorData* pdata = NULL);
	void ST_Closed(DoorData* pdata);
	void ST_Opened(DoorData* pdata);
	uint8_t CmpArrays(uint8_t* tab1, uint8_t* tab2, uint8_t len);
	enum States {ST_INIT = 0, ST_CLOSED, ST_OPENED, ST_MAX_STATES};
	BEGIN_STATE_MAP
		STATE_MAP_ENTRY(&Door::ST_Init)
		STATE_MAP_ENTRY(&Door::ST_Closed)
		STATE_MAP_ENTRY(&Door::ST_Opened)
	END_STATE_MAP

	uint8_t status, sub_position, position;
	bool zero_achieved;
};

extern Door door;
extern DoorData door_data;

#endif /* DOOR_H_ */
