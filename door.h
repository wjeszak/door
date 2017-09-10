/*
 * door.h
 *
 *  Created on: 31 sie 2017
 *      Author: tomek
 */

#ifndef DOOR_H_
#define DOOR_H_

#include "state_machine.h"

#define DOOR_STATUS_CLOSED 					0xC0
#define DOOR_STATUS_OPENED_AND_CLOSED 		0xD0
#define DOOR_STATUS_DOOR_NOT_YET_OPENED		0x80
#define DOOR_STATUS_OPENED 					0x40

#define F03_OPTICAL_SWITCHES_FAULT 			(0x03 << 8)
#define F05_ELECTROMAGNET_FAULT 			(0x05 << 8)
#define F06_CLOSE_THE_DOOR 					(0x06 << 8)

class DoorData : public EventData
{
public:
};

class Door : public StateMachine
{
public:
	Door();
	// Events
	void EV_Close(DoorData* pdata = NULL);
	void EV_Open(DoorData* pdata = NULL);
	void SetStatus(uint16_t st);
	void UpdateStatus();
	uint16_t GetStatus();
private:
	// States functions
	void ST_Init(DoorData* pdata = NULL);
	void ST_Closed(DoorData* pdata);
	void ST_Opened(DoorData* pdata);
	enum States {ST_INIT = 0, ST_CLOSED, ST_OPENED, ST_MAX_STATES};
	BEGIN_STATE_MAP
		STATE_MAP_ENTRY(&Door::ST_Init)
		STATE_MAP_ENTRY(&Door::ST_Closed)
		STATE_MAP_ENTRY(&Door::ST_Opened)
	END_STATE_MAP

	uint16_t status;
};

extern Door door;
extern DoorData door_data;

#endif /* DOOR_H_ */
