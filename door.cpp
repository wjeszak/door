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

uint8_t sequence_1[5] = {1, 2, 3, 7};		// rzecz. 0, 1, 0, 2, 3, 7
uint8_t sequence_n[6] = {5, 4, 6, 2, 3, 1};

Door::Door()
{
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
	if(!zero_achieved && (val == 0) && (last_val == 1))
	{
		zero_achieved = true;
		SetStatus(DOOR_STATE_CLOSED);
		return;
	}

	if(zero_achieved)
	{
		if(pos == 0)
		{
			// Drzwi musza byc w pozycji 1 a nastepnie przez sekunde w 0 zeby uznac je za zamkniete.
			// Mala szansa ze ktos zatrzyma drzwi w pozycji "drugiego" zera.
			if((last_val == 1) && (val == 0)) timer.Assign(TIMER_DOOR_CLOSED, 1000, DoorClosed);

			if(val == sequence_1[sub_pos])
			{
				SetStatus(DOOR_STATE_OPENED);
				sub_pos++;
				if(sub_pos == 4) { sub_pos = 0; SetStatus(++pos); }
				return;
			}

			if((val == sequence_1[sub_pos - 2]) && (sub_pos > 1))
			{
				sub_pos--;
				return;
			}
		}
		if(pos > 0)
		{
			if(val == sequence_n[sub_pos])
			{
				//SetStatus(DOOR_STATE_OPENED);
				sub_pos++;
				if(sub_pos == 5) { sub_pos = 0; SetStatus(++pos); }
				return;
			}
		}
	}
}

void Door::SetStatus(uint8_t st)
{
	status = st;
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
