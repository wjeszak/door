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
#include "stamp.h"

Comm_prot::Comm_prot()
{
	//frame_length = 4;
	//address = stamp_data.address;
}

void Comm_prot::Parse(uint8_t* frame)
{
	if(!IsStampProgrammingMode()) 	// normal mode
	{
		uint8_t crc = Crc8(frame, 2);
		if((frame[0] == address) && (frame[2] == crc))
		{
			uint8_t command = frame[1];
			// check electromagnet
			if(command == COMM_CHECK_ELM)
			{
				ELM_ON;
				timer.Assign(TTest_Elm, 4, ElmTestDynabox);
			}
			if(command == COMM_GET_STATUS_BEFORE_MOVEMENT)
			{
				// before next movement 0xD0 -> 0xC0
				if(door.GetStatus() == DOOR_STATE_OPENED_AND_CLOSED) door.SetStatus(DOOR_STATE_CLOSED);
				comm.Prepare(door.GetStatus());
			}
			if(command == COMM_ELM_OFF)
			{
				ELM_OFF;
				uint8_t status = DOOR_STATE_CLOSED;
				door.SetStatus(status);
				comm.Prepare(status);
			}
			if(command == COMM_ELM_OFF_ON)
			{
				ELM_OFF;
				timer.Assign(TElmOffOn, 50, ElmOffOn);
				comm.Prepare(door.GetStatus());
			}
			// Lockerbox
			if(command == COMM_CHECK_ELM_GET_STATUS_LOCKERBOX)
			{
				ELM_ON;
				timer.Assign(TTest_Elm, 1, ElmTestLockerbox);
			}
			if(command == COMM_GET_STATUS_LOCKERBOX)
			{
				if(door.lockerbox_has_been_opened && (LOCK_PPIN & (1 << LOCK_PIN)))
				{
					door.lockerbox_has_been_opened = false;
					timer.Assign(TLockerboxD0Timer, 1000, LockerboxD0);
					door.lockerbox_has_been_opened1 = true;
					//return;
				}
				if(door.lockerbox_has_been_opened1)
				{
					comm.Prepare(0xD0);
					return;
				}
				uint8_t status;
				if(LOCK_PPIN & (1 << LOCK_PIN))
					status = DOOR_STATE_CLOSED;
				else
					status = DOOR_STATE_EM_OFF_1STOP;
				door.SetStatus(status);
				comm.Prepare(status);
			}

			if(COMM_OPEN_LOCKERBOX)
			{
				comm.Prepare(0x00); // door ok
				// if closed
				if(LOCK_PPIN & (1 << LOCK_PIN))
				{
					ELM_ON;
					// polling lock
					timer.Assign(TWaitingForOpen, 1, WaitingForOpen);
					//uint8_t time = command - 0xE0;
					//timer.Assign(TEmergencyOff, time * 100, EmergencyOff);
					timer.Assign(TEmergencyOff, LOCKERBOX_EMERG_ON1, EmergencyOff);
				}
				else
				{
					 timer.Assign(TLockerboxOpenedReply, 1, LockerboxOpenedReply);
				}
				return;
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
