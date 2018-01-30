/*
 * commands_dynabox.cpp
 *
 *  Created on: 30 sty 2018
 *      Author: wjeszak
 */

#include "commands_dynabox.h"
#include "door.h"
#include "electromagnet.h"
#include "transoptors.h"
#include "timer.h"
#include "comm.h"

void Dynabox_CheckElm()
{
	ELM_ON;
	timer.Assign(TTest_Elm, 4, ElmTestDynabox);
}

void Dynabox_GetStatusBeforeMovement()
{
	// before next movement 0xD0 -> 0xC0
	if(door.GetStatus() == DOOR_STATE_OPENED_AND_CLOSED) door.SetStatus(DOOR_STATE_CLOSED);
	comm.Prepare(door.GetStatus());
}

void Dynabox_ElmOff()
{
	ELM_OFF;
	uint8_t status = DOOR_STATE_CLOSED;
	door.SetStatus(status);
	comm.Prepare(status);
}

void Dynabox_ElmOffOn()
{
	ELM_OFF;
	timer.Assign(TElmOffOn, 50, ElmOffOn);
	comm.Prepare(door.GetStatus());
}

void Dynabox_GetStatus()
{
	if(transoptors.Check())
		comm.Prepare(door.GetStatus());
	else
		comm.Prepare(F03_OPTICAL_SWITCHES_FAULT);
}

void Dynabox_GetSetStatus(uint8_t pos)
{
	if(transoptors.Check())
	{
		door.SetStatus(DOOR_STATE_DOOR_NOT_YET_OPENED);
		comm.Prepare(door.GetStatus());
		door.required_position = 0;
		door.required_position = pos;
		ELM_ON;
	}
	else
	{
		comm.Prepare(F03_OPTICAL_SWITCHES_FAULT);
	}
}
