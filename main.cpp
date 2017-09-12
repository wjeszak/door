/*
 * main.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include <avr/interrupt.h>
#include "timer.h"
#include "transoptors.h"
#include "electromagnet.h"
#include "door.h"
#include "usart.h"
#include "comm_prot.h"

Timer timer(T0_PS_64, 115);
Transoptors transoptors;
Electromagnet electromagnet;
DoorData door_data;
Door door;
UsartData usart_data;
Usart usart;
Comm_prot comm;

int main()
{
	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT23) | (1 << PCINT19) | (1 << PCINT18);
	sei();
	while(1)
	{
	//	door.Poll();
	}
}
