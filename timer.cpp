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
	for(uint8_t n = 0; n < NUMBER_OF_TIMERS; n++)
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
	for(uint8_t n = 0; n < NUMBER_OF_TIMERS; n++)
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

void Timer::Assign(uint8_t handler_id, uint16_t interval, void(*fp)())
{
	timer_handlers[handler_id].interval = interval;
	timer_handlers[handler_id].counter = 0;
	timer_handlers[handler_id].active = true;
	timer_handlers[handler_id].fp = fp;
}

void Timer::Enable(uint8_t handler_id)
{
	timer_handlers[handler_id].active = true;
	timer_handlers[handler_id].counter = 0;
}

void Timer::Disable(uint8_t handler_id)
{
	timer_handlers[handler_id].active = false;
}

ISR(TIMER0_COMPA_vect)
{
	timer.Irq();
}
// -----------------------------------------------------------------
void ElectromagnetTest()
{
	if(ELECTROMAGNET_TEST_COIL_PPIN & (1 << ELECTROMAGNET_TEST_COIL_PIN))
		comm.Prepare(F05_ELECTROMAGNET_FAULT);
	else
		comm.Prepare(0x00);
	ELECTROMAGNET_OFF;
	timer.Disable(TIMER_TEST_ELECTROMAGNET);
}

void DoorClosed()
{
	timer.Disable(TIMER_DOOR_CLOSED);
	door.SetStatus(DOOR_STATE_CLOSED);
}
