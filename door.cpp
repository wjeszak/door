/*
 * door.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: tomek
 */

#include "door.h"
#include "transoptors.h"

uint8_t seq1[5] = {0, 1, 2, 3, 7};
uint8_t seq1_back[4] = {7, 3, 2, 1};

uint8_t seqn[6] = {5, 4, 6, 2, 3, 1};
uint8_t seqn_back[6] = {1, 3, 2, 6, 4, 5};

Door::Door() : StateMachine(ST_MAX_STATES)
{
	zero_achieved = false;
	sub_position = 0;
	position = 1;
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
	position = 1;
	zero_achieved = true;
	SetStatus(DOOR_STATE_CLOSED);
}

void Door::ST_Opened(DoorData* pdata)
{
	static uint8_t i, seq[6];

	SetStatus(position);
	if(zero_achieved)
	{
		if(pdata->val == seq1[sub_position + 1]) sub_position++;
		if((pdata->val == seq1[sub_position - 1]) && (sub_position >= 1)) sub_position--;
		if(sub_position == 4)
		{
			SetStatus(++position);
			//if(CmpArrays(seq, seq1, 4)) { i = 0; SetStatus(++position); }
			//if(CmpArrays(seq, seq1_back, i)) { i = 0; SetStatus(--position); }
		}
		//if(i == 6)
		//{
		//	if(CmpArrays(seq, seqn, 6)) { i = 0; SetStatus(++position); }
		//	if(CmpArrays(seq, seqn_back, 6)) { i = 0; SetStatus(--position); }
		//}
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

uint8_t Door::CmpArrays(uint8_t* tab1, uint8_t* tab2, uint8_t len)
{
	for(uint8_t i = 0; i < len; i++)
	{
		if(tab1[i] != tab2[i]) return 0;
	}
	return 1;
}
