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

Door::Door()
{
	state = transoptors.Read();
	position = 0;
}

void Door::UpdatePosition()
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
	position = transoptors.Read();
}

uint8_t Door::GetPosition()
{
	return position;
}
