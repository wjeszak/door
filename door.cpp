/*
 * door.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: tomek
 */

#include "door.h"
#include "transoptors.h"

uint8_t seq1[4] = {1, 2, 3, 7};
uint8_t seqn[6] = {5, 4, 6, 2, 3, 1};

Door::Door() : StateMachine(ST_MAX_STATES)
{
	zero_achieved = false;
	position = 0;
	ST_Init();
}

void Door::SetStatus(uint8_t st)
{
	status = st;
}

void Door::ST_Init(DoorData* pdata)
{
	transoptors.Read();
	if(door_data.val == 0)
	{
		InternalEvent(ST_CLOSED, NULL);
	}
	else
	{
		SetStatus(DOOR_STATE_OPENED);
		InternalEvent(ST_OPENED, NULL);
	}
}

void Door::ST_Closed(DoorData* pdata)
{
	position = 0;
	zero_achieved = true;
	SetStatus(DOOR_STATE_CLOSED);
}

void Door::ST_Opened(DoorData* pdata)
{
	static uint8_t i;
	SetStatus(position);
	if(zero_achieved)
	{
		if(pdata->val == seq1[i]) i++;
		if(i == 4) { i = 0; SetStatus(++position); }

		if(pdata->val == seqn[i]) i++;
		if(i == 6) { i = 0; SetStatus(++position); }
	}
}

void Door::EV_ChangeVal(DoorData* pdata)
{
	if(pdata->val == 0)
		InternalEvent(ST_CLOSED, pdata);
	else
		InternalEvent(ST_OPENED, pdata);
}

uint8_t Door::GetStatus()
{
	return status;
}
