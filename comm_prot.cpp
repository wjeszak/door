/*
 * comm_prot.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include "comm_prot.h"
#include "usart.h"
#include "timer.h"
#include "transoptors.h"
#include "electromagnet.h"
#include "door.h"

Comm_prot::Comm_prot()
{
	address = 1;
}

void Comm_prot::Parse(uint8_t* frame)
{
	uint8_t crc = Crc8(frame, 2);
	if((frame[0] == address) && (frame[2] == crc))
	{
		switch(frame[1])
		{
		case 0x01:
			ELECTROMAGNET_ON;
			timer.Assign(TIMER_TEST_ELECTROMAGNET, 4, ElectromagnetTest);
		break;
		case 0x02:
			if(transoptors.Check())
				comm.Prepare(door.GetStatus());
			else
				comm.Prepare(F03_OPTICAL_SWITCHES_FAULT);
		break;
		case 0x03:

		break;

		}
	}
}

void Comm_prot::Prepare(uint8_t status)
{
	usart_data.frame[0] = address;
	usart_data.frame[1] = status;
	usart_data.frame[2] = Crc8(usart_data.frame, 2);
	usart_data.frame[3] = 0x0A;
	usart_data.len = FRAME_LENGTH_RESPONSE;
	usart.SendFrame(&usart_data);
}

uint8_t Comm_prot::Crc8(uint8_t *frame, uint8_t len)
{
	uint8_t crc = 0x00;
	for (uint8_t pos = 0; pos < len; pos++)
	{
		crc ^= frame[pos];
		for (uint8_t i = 8; i != 0; i--)
		{
			if ((crc & 0x80) != 0)
			{
				crc <<= 1;
				crc ^= 0x07;
			}
			else
				crc <<= 1;
		}
	}
	return crc;
}
