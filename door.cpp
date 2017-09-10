/*
 * door.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: tomek
 */

#include "door.h"
#include "transoptors.h"

uint8_t seq1[4] = {1, 2, 3, 7};
uint8_t tab[6];

Door::Door() : StateMachine(ST_MAX_STATES)
{
	ST_Init();
}

void Door::SetStatus(uint8_t st)
{
	status = st;
}

void Door::ST_Init(DoorData* pdata)
{
	door_data.val = 0;

	if(transoptors.Read() == 0)
	{
		InternalEvent(ST_CLOSED, NULL);
	}
	else
	{
		zero_achieved = false;
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
	static uint8_t seq[5], i;
	SetStatus(DOOR_STATE_OPENED);
	if(zero_achieved)
	{
		seq[i++] = pdata->val;
		if(i == 4)
		{
			for(i = 0; i < 4; i++)
			{
				if(seq[i] != seq1[i]) return;
			}
			SetStatus(DOOR_STATE_OPENED_1STOP);
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

void Door::Poll()
{
	uint8_t trans_val = transoptors.Read();

	if(trans_val != door_data.val)
	{
		door_data.val = trans_val;
		EV_ChangeVal(&door_data);
	}
//	if(i == 4)
//	{
//		for(uint8_t j = 0; j < 4; j++)
//		{
//			if(tab[j] != seq1[j]) return;
//		}
//		i = 0;
//		position++;
//	}
}

uint8_t Door::GetStatus()
{
	return status;
}
