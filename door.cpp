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
	zero_achieved = true;
	SetStatus(DOOR_STATE_CLOSED);
}

void Door::ST_Opened(DoorData* pdata)
{
	static uint8_t seq[6], i;
	SetStatus(position);
	if(zero_achieved)
	{
		seq[i++] = pdata->val;
		if((i == 4) && (position == 0))
		{
			for(i = 0; i < 4; i++)
			{
				if(seq[i] != seq1[i]) return;
			}
			i = 0;
			position++;
			SetStatus(position);
			//SetStatus(DOOR_STATE_OPENED_1STOP);
		}
		if(i == 6)
		{
			for(i = 0; i < 6; i++)
			{
				if(seq[i] != seqn[i]) return;
			}
			i = 0;
			position++;
			SetStatus(position);
		}

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
