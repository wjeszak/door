/*
 * timer.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include <avr/interrupt.h>
#include "timer.h"
#include "electromagnet.h"
#include "comm_prot.h"
#include "door.h"

Timer::Timer(T0Prescallers Prescaller, uint8_t Tick)
{
	for(uint8_t n = 0; n < TNumberOfTimers; n++)
	{
		timer_handlers[n].active = false;
		timer_handlers[n].counter = 0;
		timer_handlers[n].interval = 0;
		timer_handlers[n].fp = NULL;
	}

	TCCR0A |= (1 << WGM01); 		// CTC
	TCCR0B |= Prescaller;
	OCR0A = Tick; 					// :		F_CPU / preskaler / 200 Hz = OCR
	TIMSK0 |= (1 << OCIE0A);
}

void Timer::Irq()
{
	for(uint8_t n = 0; n < TNumberOfTimers; n++)
	{
		if ((timer_handlers[n].active) && (timer_handlers[n].fp != NULL))
		{
			if ((timer_handlers[n].counter == timer_handlers[n].interval))
			{
				timer_handlers[n].counter = 0;
				timer_handlers[n].fp();
			}
			else
			{
				timer_handlers[n].counter++;
			}
		}
	}
}

void Timer::Assign(TimerId handler_id, uint16_t interval, void(*fp)())
{
	timer_handlers[handler_id].interval = interval;
	timer_handlers[handler_id].counter = 0;
	timer_handlers[handler_id].active = true;
	timer_handlers[handler_id].fp = fp;
}

void Timer::Enable(TimerId handler_id)
{
	timer_handlers[handler_id].active = true;
	timer_handlers[handler_id].counter = 0;
}

void Timer::Disable(TimerId handler_id)
{
	timer_handlers[handler_id].active = false;
}

ISR(TIMER0_COMPA_vect)
{
	timer.Irq();
}
// -----------------------------------------------------------------
void ElmTestDynabox()
{
	if(ELM_TEST_COIL_PPIN & (1 << ELM_TEST_COIL_PIN))
#ifdef DEBUG
		comm.Prepare(0,0, F05_ELECTROMAGNET_FAULT);
#else
	comm.Prepare(F05_ELECTROMAGNET_FAULT);
#endif
	else
#ifdef DEBUG
		comm.Prepare(0,0, 0x00);
#else
	comm.Prepare(0x00);
#endif
	ELM_OFF;
	timer.Disable(TTest_Elm);
	// before next movement 0xD0 -> 0xC0
	//if(door.GetStatus() == DOOR_STATE_OPENED_AND_CLOSED) door.SetStatus(DOOR_STATE_CLOSED);
}

void ElmTestLockerbox()
{
	timer.Disable(TTest_Elm);

	if(ELM_TEST_COIL_PPIN & (1 << ELM_TEST_COIL_PIN))
	{
		ELM_OFF;
		comm.Prepare(F05_ELECTROMAGNET_FAULT);
	}
	else
	{
		ELM_OFF;
		uint8_t status;
		if(LOCK_PPIN & (1 << LOCK_PIN))
			status = DOOR_STATE_CLOSED;
		else
			status = DOOR_STATE_EM_OFF;
		door.SetStatus(status);
		comm.Prepare(status);
	}
}

void DoorClosed()
{
	timer.Disable(TDoorClosed);
	door.SetStatus(DOOR_STATE_OPENED_AND_CLOSED);
	door.sub_pos = 0;
}

void ElmOffOn()
{
	ELM_ON;
	timer.Disable(TElmOffOn);
}

void WaitingForOpen()
{
	uint8_t status;
	// opened
	if(!(LOCK_PPIN & (1 << LOCK_PIN)))
	{
		ELM_OFF;
		timer.Disable(TWaitingForOpen);
		timer.Disable(TEmergencyOff);
		timer.Disable(TEmergencyOn);
		status = DOOR_STATE_EM_OFF;
		door.SetStatus(status);
		comm.Prepare(status);
	}
}

void EmergencyOff()
{
	timer.Disable(TEmergencyOff);
	ELM_OFF;
	timer.Assign(TEmergencyOn, 2000, EmergencyOn);
}

void EmergencyOn()
{
	timer.Disable(TEmergencyOn);
	ELM_ON;
	timer.Assign(TEmergencyOff, 6000, EmergencyOff2);
}

void EmergencyOff2()
{
	timer.Disable(TEmergencyOff);
	timer.Disable(TWaitingForOpen);
	ELM_OFF;
	comm.Prepare(0x07);
}
