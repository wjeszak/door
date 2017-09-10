/*
 * door.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: tomek
 */

#include "door.h"
#include "transoptors.h"

uint8_t seq1[5] = {1, 2, 3, 7};
uint8_t tab[6];

Door::Door() : StateMachine(ST_MAX_STATES)
{
	ST_Init();
}

void Door::SetStatus(uint16_t st)
{
	status = st;
}

void Door::ST_Init(DoorData* pdata)
{
	if(transoptors.Read() == 0)		// closed
	{
		SetStatus(DOOR_STATUS_CLOSED);
		InternalEvent(ST_CLOSED, NULL);
	}
	else
	{
		SetStatus(DOOR_STATUS_OPENED | F06_CLOSE_THE_DOOR);
		InternalEvent(ST_OPENED, NULL);
	}
}

void Door::ST_Closed(DoorData* pdata)
{
	SetStatus(DOOR_STATUS_CLOSED);
}

void Door::ST_Opened(DoorData* pdata)
{

}

void Door::EV_Close(DoorData* pdata)
{
	BEGIN_TRANSITION_MAP								// current state
		TRANSITION_MAP_ENTRY(ST_NOT_ALLOWED)			// ST_INIT
		TRANSITION_MAP_ENTRY(ST_NOT_ALLOWED)			// ST_CLOSED
		TRANSITION_MAP_ENTRY(ST_CLOSED)					// ST_OPENED
	END_TRANSITION_MAP(pdata)
}

void Door::UpdateStatus()
{
/*	static uint8_t i;
	uint8_t state_tmp = transoptors.Read();
	if(state != state_tmp)
	{
		state = state_tmp;
		tab[i++] = state;
	}
	if(i == 4)
	{
		for(uint8_t j = 0; j < 4; j++)
		{
			if(tab[j] != seq1[j]) return;
		}
		i = 0;
		position++;
	}
*/
	if(transoptors.Read() == 0)
	{
		EV_Close(NULL);
	}
}

uint16_t Door::GetStatus()
{
	return status;
}
