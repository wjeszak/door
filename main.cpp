/*
 * main.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#include "comm_prot.h"
#include "usart.h"
#include "timer.h"
#include "electromagnet.h"
#include "status.h"
#include "transoptors.h"
#include "door.h"

#if defined (__AVR_ATmega88P__)
Timer timer(T0_PS_64, 115);
#endif
#if defined (__AVR_ATmega8__)
Timer timer(T1_PS_64, 229);
#endif

Usart usart;
UsartData usart_data;
Comm_prot comm;
Electromagnet electromagnet;
Transoptors transoptors;
Door door;

int main()
{
	AdcInit();
	DDRC |= (1 << 0) | (1 << 1); 		// led
	PORTC |= (1 << 0) | (1 << 1);
	sei();
	while(1)
	{
		door.UpdatePosition();
	}
}
