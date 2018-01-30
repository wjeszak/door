/*
 * comm.h
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#ifndef COMM_H_
#define COMM_H_

#include <inttypes.h>

#define FRAME_ADDRESS 									0
#define FRAME_DATA 										1
#define FRAME_CRC 										2
#define FRAME_END 										3

#define FRAME_LENGTH_NORMAL 							4
#define FRAME_LENGTH_STAMP 								9

#define COMM_DYNABOX_CHECK_ELM			 				0x01
#define COMM_DYNABOX_GET_STATUS_BEFORE_MOVEMENT 		0x02
#define COMM_DYNABOX_ELM_OFF 							0x03
#define COMM_DYNABOX_ELM_OFF_ON 						0x04
#define COMM_DYNABOX_GET_STATUS 						command & (1 << 7)								// 0x80
#define COMM_DYNABOX_GET_SET_STATUS 					(command & (1 << 7)) && (command & (1 << 6))	// 0xC0

#define COMM_LOCKERBOX_CHECK_ELM_GET_STATUS			 	0x05
#define COMM_LOCKERBOX_GET_STATUS						0x06
#define COMM_LOCKERBOX_OPEN 							0x07//(command & (1 << 7)) && (command & (1 << 6)) && (command & (1 << 5))

class Comm
{
public:
	Comm();
	void Parse(uint8_t* frame);
	uint8_t frame_length;
	void Prepare(uint8_t status);
	uint8_t address;
private:
	bool FrameIsForMeAndCrcOk(uint8_t* frame, uint8_t address);
	uint8_t GetCommand(uint8_t* frame);
	uint8_t Crc8(uint8_t* frame, uint8_t len);
};

extern Comm comm;

#endif /* COMM_H_ */
