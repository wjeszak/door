/*
 * commands_lockerbox.cpp
 *
 *  Created on: 30 sty 2018
 *      Author: wjeszak
 */

#include "commands_lockerbox.h"
#include "electromagnet.h"
#include "timer.h"
#include "door.h"
#include "comm.h"

void Lockerbox_CheckElmGetStatus()
{
	ELM_ON;
	timer.Assign(TTest_Elm, 1, ElmTestLockerbox);
}

void Lockerbox_GetStatus()
{
	if((door.lockerbox_has_been_opened && IsClosed()))
	{
		comm.Prepare(DOOR_STATE_OPENED_AND_CLOSED);
		return;
	}

	if(IsClosed())
		comm.Prepare(DOOR_STATE_CLOSED);
	else
		comm.Prepare(DOOR_STATE_EM_OFF_1STOP);
}

void Lockerbox_Open()
{
	comm.Prepare(0x00); // door ok
	if(IsClosed())
	{
		ELM_ON;
		// polling lock
		timer.Assign(TWaitingForOpen, 1, WaitingForOpen);
		timer.Assign(TEmergencyOff, LOCKERBOX_EMERG_ON1, EmergencyOff);
	}
	else
	{
		timer.Assign(TLockerboxOpenedReply, 1, LockerboxOpenedReply);
	}
}

bool IsClosed()
{
	if(LOCK_PPIN & (1 << LOCK_PIN))
		return true;
	else
		return false;
}
