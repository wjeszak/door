/*
 * door.cpp
 *
 *  Created on: 31 sie 2017
 *      Author: tomek
 */

#include "door.h"
#include "transoptors.h"
#include "electromagnet.h"
#include "timer.h"
// sekwencja drzwi normalnych: 								0, 1, 0, 2, 3, 7
// sekwencja drzwi "felernych" (grubszych, krotsza listwa): 0, 1, 0, 2, 3, 1

uint8_t sequence_1_thin[4]  = {1, 2, 3, 7};			// cienkie
uint8_t sequence_1_thick[4] = {1, 2, 3, 1};			// grube
uint8_t sequence_n[6]       = {5, 4, 6, 2, 3, 1};

Door::Door()
{
	zero_achieved = false;
	sub_pos = 0;
	pos = 0;
	last_val = 0;
	cnt = 0;
	val = transoptors.Read();
	// Power ON
	if(val == 0)
	{
		zero_achieved = true;
		SetStatus(DOOR_STATE_CLOSED);
	}
	else
	{
		SetStatus(DOOR_STATE_OPENED);
	}
}

void Door::EV_ChangeVal(DoorData* pdata)
{
	if(pos == required_position) ELM_OFF;
	last_val = val;
	val = transoptors.Read();
	if(!zero_achieved && last_val == 1 && val == 0)
	{
		zero_achieved = true;
		SetStatus(DOOR_STATE_CLOSED);
		return;
	}

	if(zero_achieved)
	{
		if(pos == 0)
		{
			if((last_val == 1) && (val == 0)) timer.Assign(TIMER_DOOR_CLOSED, 5000, DoorClosed);
			// next position
			if(sub_pos == 4 && val == sequence_n[0]) { sub_pos = 0; SetStatus(++pos); }
			// forward
			if(((val == sequence_1_thick[sub_pos]) || (val == sequence_1_thin[sub_pos])) && (sub_pos <= 3))
			{
				SetStatus(DOOR_STATE_OPENED);
				sub_pos++;
				return;
			}
			// backward
			if(((val == sequence_1_thick[sub_pos - 2]) || (val == sequence_1_thin[sub_pos - 2])) && (sub_pos >= 2))
			{
				sub_pos--;
				return;
			}
		}
		if(pos > 0)
		{
			// next position
			if((sub_pos == 6) && (val == sequence_n[0])) { sub_pos = 0;	SetStatus(++pos); }
			// forward
			if((val == sequence_n[sub_pos]) && (sub_pos <= 5))
			{
				sub_pos++;
				return;
			}
			// backward
			if((val == sequence_n[sub_pos - 2]) && (sub_pos >= 2))
			{
				sub_pos--;
				return;
			}
			// 1 -> 0,  v -  		 v -
			if(((val == 7) || val == 1) && (sub_pos == 1) && pos == 1) { sub_pos = 4; SetStatus(--pos); return; }
			// n -> n - 1
			if((val == 1) && (sub_pos == 1)) { sub_pos = 6; SetStatus(--pos); }

		}
	}
}

void Door::SetStatus(uint8_t st)
{
	status = st;
}

uint8_t Door::GetTransVal()
{
	return val;
}

uint8_t Door::GetSubpos()
{
	return sub_pos;
}

uint8_t Door::GetStatus()
{
	if(status == DOOR_STATE_CLOSED || status == DOOR_STATE_OPENED_AND_CLOSED || status == DOOR_STATE_DOOR_NOT_YET_OPENED)
		return status;
	else
		if(elm.ElmOn())
			return	status;
		else
			return	status + DOOR_STATE_EM_OFF;
}
