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

uint8_t sequence_1[4] = {1, 2, 3, 7};		// rzecz. 0, 1, 0, 2, 3, 7
uint8_t sequence_n[6] = {5, 4, 6, 2, 3, 1};

Door::Door()
{
	last_sub_pos = false;
	zero_achieved = false;
	sub_pos = 0;
	pos = 0;
	last_val = 0;
	val = transoptors.Read();
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
	timer.Disable(TIMER_DOOR_CLOSED);
	last_val = val;
	val = transoptors.Read();
	if(!zero_achieved && (last_val == 1) && (val == 0))
	{
		zero_achieved = true;
		SetStatus(DOOR_STATE_CLOSED);
		return;
	}

	if(zero_achieved)
	{
		if(pos == 0)
		{
			// Drzwi musza byc w pozycji 1 a nastepnie przez 100 ms w 0 zeby uznac je za zamkniete.
			// Mala szansa ze ktos zatrzyma drzwi w pozycji "drugiego" zera.
			if((last_val == 1) && (val == 0)) timer.Assign(TIMER_DOOR_CLOSED, 100, DoorClosed);
			// next position
			if((sub_pos == 4) && (val == sequence_n[0])) { sub_pos = 0; SetStatus(DOOR_STATE_OPENED + ++pos); }
			// forward
			if((val == sequence_1[sub_pos]) && (sub_pos <= 3))
			{
				SetStatus(DOOR_STATE_OPENED);
				sub_pos++;
				return;
			}
			// backward
			if((val == sequence_1[sub_pos - 2]) && (sub_pos >= 2))
			{
				sub_pos--;
				return;
			}
		}
		if(pos > 0)
		{
			// next position
			if((sub_pos == 6) && (val == sequence_n[0])) { sub_pos = 0;	SetStatus(DOOR_STATE_OPENED + ++pos); }
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
			// n -> n - 1
			if((val == 1) && (sub_pos == 1)) { sub_pos = 6; SetStatus(DOOR_STATE_OPENED + --pos); }
			// 1 -> 0
			if((val == 7) && (sub_pos == 1)) { sub_pos = 4; SetStatus(DOOR_STATE_OPENED + --pos); }
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
	return status;
}


/*
uint8_t Door::CmpArrays(uint8_t* tab1, uint8_t* tab2, uint8_t len)
{
	for(uint8_t i = 0; i < len; i++)
	{
		if(tab1[i] != tab2[i]) return 0;
	}
	return 1;
}
*/
