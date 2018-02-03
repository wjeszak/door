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
#include "led.h"
#include "commands_lockerbox.h"

Timer timer(T0_PS_64, 115);
Transoptors transoptors;
Elm elm;
DoorData door_data;
Door door;
UsartData usart_data;
Usart usart;
Comm comm;
Led led;

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
	led.SetParams(LED_RED1_PIN, LED_RED2_PIN, 2);

	while(1)
	{
		if(!IsClosed())
		{
			LED_GREEN1_ON;
			LED_GREEN2_ON;
		}
		else
		{
			LED_GREEN1_OFF;
			LED_GREEN2_OFF;
		}
	}
}
