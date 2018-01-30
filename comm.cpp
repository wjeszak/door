/*
 * comm.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include "comm.h"
#include "usart.h"
#include "timer.h"
#include "door.h"
#include "stamp.h"
#include "commands_dynabox.h"
#include "commands_lockerbox.h"

Comm::Comm()
{

}

bool Comm::FrameIsForMeAndCrcOk(uint8_t* frame, uint8_t address)
{
	uint8_t crc = Crc8(frame, FRAME_CRC);

	if((frame[FRAME_ADDRESS] == address) && (frame[FRAME_CRC] == crc))
		return true;
	else
		return false;
}

uint8_t Comm::GetCommand(uint8_t* frame)
{
	return frame[FRAME_DATA];
}

void Comm::Parse(uint8_t* frame)
{
	if(!IsStampProgrammingMode()) 	// normal mode
	{
		if(FrameIsForMeAndCrcOk(frame, stamp_data.address))
		{
			uint8_t command = GetCommand(frame);

			if(command == COMM_DYNABOX_CHECK_ELM) 				   { Dynabox_CheckElm(); 					return; }
			if(command == COMM_DYNABOX_GET_STATUS_BEFORE_MOVEMENT) { Dynabox_GetStatusBeforeMovement(); 	return; }
			if(command == COMM_DYNABOX_ELM_OFF)					   { Dynabox_ElmOff(); 						return; }
			if(command == COMM_DYNABOX_ELM_OFF_ON) 				   { Dynabox_ElmOffOn(); 					return; }
			if(COMM_DYNABOX_GET_STATUS) 						   { Dynabox_GetStatus(); 					return; }
			if(COMM_DYNABOX_GET_SET_STATUS) 					   { Dynabox_GetSetStatus(command - 0xC0);	return; }

			if(command == COMM_LOCKERBOX_CHECK_ELM_GET_STATUS) 	   { Lockerbox_CheckElmGetStatus(); 		return; }
			if(command == COMM_LOCKERBOX_GET_STATUS) 			   { Lockerbox_GetStatus(); 				return; }
			if(command == COMM_LOCKERBOX_OPEN) 					   { Lockerbox_Open(); 						return; }

		}
	}
	else		// stamp programming mode
	{
		uint8_t crc = Crc8(frame, 7);
		// read
		if((frame[0] == 1) && (frame[7] == crc))
		{
			eeprom_read_block(&stamp_data, &ee_stamp_data, sizeof(Stamp_data));
			usart_data.frame[0] = 1;
			usart_data.frame[1] = stamp_data.type_of_hardware;
			usart_data.frame[2] = stamp_data.address;
			usart_data.frame[3] = stamp_data.year;
			usart_data.frame[4] = stamp_data.week;
			usart_data.frame[5] = stamp_data.sn >> 8;
			usart_data.frame[6] = stamp_data.sn & 0xFF;
			usart_data.frame[7] = Crc8(frame, 7);
			usart_data.frame[8] = 10;
			usart_data.len = 9;
			usart.SendFrame(&usart_data);
		}
		if((frame[0] == 2) && (frame[7] == crc))
		{
			stamp_data.type_of_hardware = usart_data.frame[1];
			stamp_data.address = usart_data.frame[2];
			stamp_data.year = usart_data.frame[3];
			stamp_data.week = usart_data.frame[4];
			stamp_data.sn = (usart_data.frame[5] << 8) | (usart_data.frame[6] & 0xFF);
			eeprom_update_block(&stamp_data, &ee_stamp_data, sizeof(Stamp_data));

			eeprom_read_block(&stamp_data, &ee_stamp_data, sizeof(Stamp_data));
			usart_data.frame[0] = 2;
			usart_data.frame[1] = stamp_data.type_of_hardware;
			usart_data.frame[2] = stamp_data.address;
			usart_data.frame[3] = stamp_data.year;
			usart_data.frame[4] = stamp_data.week;
			usart_data.frame[5] = stamp_data.sn >> 8;
			usart_data.frame[6] = stamp_data.sn & 0xFF;
			usart_data.frame[7] = Crc8(frame, 7);
			usart_data.frame[8] = 10;
			usart_data.len = 9;
			usart.SendFrame(&usart_data);
		}
	}
}

void Comm::Prepare(uint8_t status)
{
	usart_data.frame[0] = address;
	usart_data.frame[1] = status;
	usart_data.frame[2] = Crc8(usart_data.frame, 2);
	usart_data.frame[3] = 0x0A;
	usart_data.len = 4;
	usart.SendFrame(&usart_data);
}

uint8_t Comm::Crc8(uint8_t *frame, uint8_t len)
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
