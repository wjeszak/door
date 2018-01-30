/*
 * main.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "comm.h"
#include "timer.h"
#include "transoptors.h"
#include "electromagnet.h"
#include "door.h"
#include "usart.h"
#include "stamp.h"

Timer timer(T0_PS_64, 115);
Transoptors transoptors;
Elm elm;
DoorData door_data;
Door door;
UsartData usart_data;
Usart usart;
Comm_prot comm;

int main()
{
	StampInit();
	eeprom_read_block(&stamp_data, &ee_stamp_data, sizeof(Stamp_data));
	comm.address = stamp_data.address;
	if(IsStampProgrammingMode())
	{
		comm.frame_length = FRAME_LENGTH_STAMP;
		PORTD &= ~(1 << 4); 	// green led on
	}
	else
	{
		comm.frame_length = FRAME_LENGTH_NORMAL;
	}
	sei();
	while(1);
}
