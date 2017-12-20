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
	address = 13;
}

void Comm_prot::Parse(uint8_t* frame)
{
	uint8_t crc = Crc8(frame, 2);
	if((frame[0] == address) && (frame[2] == crc))
	{
		uint8_t command = frame[1];
		// check electromagnet
		if(command == COMM_CHECK_ELECTROMAGNET)
		{
			ELM_ON;
			timer.Assign(TIMER_TEST_ELM, 4, ElmTest);
		}
		// set state
		if(COMM_GET_SET_STATUS)
		{
			if(transoptors.Check())
			{
				door.SetStatus(DOOR_STATE_DOOR_NOT_YET_OPENED);
#ifdef DEBUG
				comm.Prepare(door.GetTransVal(), door.GetSubpos(), door.GetStatus());
#else
				comm.Prepare(door.GetStatus());
#endif
				door.required_position = 0;
				door.required_position = command - 0xC0;//COMM_GET_SET_STATUS;
				ELM_ON;
				return;
			}
			else
			{
#ifdef DEBUG
				comm.Prepare(door.GetTransVal(), door.GetSubpos(), F03_OPTICAL_SWITCHES_FAULT);
#else
				comm.Prepare(F03_OPTICAL_SWITCHES_FAULT);
#endif
				return;
			}
		}
		// get state
		if(COMM_GET_STATUS)
		{
			if(transoptors.Check())
#ifdef DEBUG
				comm.Prepare(door.GetTransVal(), door.GetSubpos(), door.GetStatus());
#else
				comm.Prepare(door.GetStatus());
#endif
			else
#ifdef DEBUG
				comm.Prepare(door.GetTransVal(), door.GetSubpos(), F03_OPTICAL_SWITCHES_FAULT);
#else
				comm.Prepare(F03_OPTICAL_SWITCHES_FAULT);
#endif
		}

	}
}
#ifdef DEBUG
void Comm_prot::Prepare(uint8_t trans_val, uint8_t sub_pos, uint8_t status)
{
	usart_data.frame[0] = address;
	usart_data.frame[1] = trans_val;
	usart_data.frame[2] = sub_pos;
	usart_data.frame[3] = status;
	usart_data.frame[4] = Crc8(usart_data.frame, 4);
	usart_data.frame[5] = 0x0A;
	usart_data.len = 6;
	usart.SendFrame(&usart_data);
}
#else
void Comm_prot::Prepare(uint8_t status)
{
	usart_data.frame[0] = address;
	usart_data.frame[1] = status;
	usart_data.frame[2] = Crc8(usart_data.frame, 2);
	usart_data.frame[3] = 0x0A;
	usart_data.len = 4;
	usart.SendFrame(&usart_data);
}
#endif
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
