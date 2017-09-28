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
		case COMM_CHECK_ELECTROMAGNET:
			ELECTROMAGNET_ON;
			timer.Assign(TIMER_TEST_ELECTROMAGNET, 4, ElectromagnetTest);
		break;
		case COMM_CHECK_TRANSOPTORS_GET_STATUS:
			if(transoptors.Check())
				comm.Prepare(door.GetTransVal(), door.GetSubpos(), door.GetStatus());
			else
				comm.Prepare(F03_OPTICAL_SWITCHES_FAULT, 0,0);
		break;
		default:
			if(frame[1] & (1 << 7)) {door.required_position = frame[1] - 0x80; ELECTROMAGNET_ON;}
		break;
		}
	}
}

void Comm_prot::Prepare(uint8_t trans_val, uint8_t sub_pos, uint8_t status)
{
	usart_data.frame[0] = address;
	usart_data.frame[1] = trans_val;
	usart_data.frame[2] = sub_pos;
	usart_data.frame[3] = status;
	usart_data.frame[4] = Crc8(usart_data.frame, 3);
	usart_data.frame[5] = 0x0A;
	usart_data.len = 6;
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
