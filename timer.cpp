/*
 * timer.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>			// NULL
#include "timer.h"
#include "usart.h"
#include "electromagnet.h"
#include "comm_prot.h"

#if defined (__AVR_ATmega88P__)
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
#endif

#if defined (__AVR_ATmega8__)
Timer::Timer(T1Prescallers Prescaller, uint8_t Tick)
{
	for(uint8_t n = 0; n < 8; n++)
	{
		timer_handlers[n].active = false;
		timer_handlers[n].counter = 0;
		timer_handlers[n].interval = 0;
		timer_handlers[n].fp = NULL;
	}

	TCCR1B |= (1 << WGM12); 		// CTC
	TCCR1B |= Prescaller;
	OCR1A = Tick; 					// :		F_CPU / preskaler / 200 Hz = OCR
	TIMSK |= (1 << OCIE1A);
}
#endif

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

void ElectromSW()
{
	if(ELECTROMAGNET_TEST_COIL_PPIN & (1 << ELECTROMAGNET_TEST_COIL_PIN))
		comm.Prepare(1);
	else
		comm.Prepare(2);
	ELECTROMAGNET_OFF;
	timer.Disable(TIMER_TEST_ELECTROMAGNET);
}

#if defined (__AVR_ATmega88P__)
ISR(TIMER0_COMPA_vect)
#endif

#if defined (__AVR_ATmega8__)
ISR(TIMER1_COMPA_vect)
#endif
{
	timer.Irq();
}
