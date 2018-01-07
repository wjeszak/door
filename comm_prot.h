/*
 * comm_prot.h
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#ifndef COMM_PROT_H_
#define COMM_PROT_H_

#include <inttypes.h>

//#define DEBUG

#define FRAME_LENGTH_REQUEST 				4

#define COMM_CHECK_ELECTROMAGNET 			0x01
#define COMM_GET_STATUS_BEFORE_MOVEMENT 	0x02
#define COMM_ELM_OFF 						0x03
#define COMM_ELM_OFF_ON 					0x04
#define COMM_GET_STATUS 					command & (1 << 7)								// 0x80
#define COMM_GET_SET_STATUS 				(command & (1 << 7)) && (command & (1 << 6))	// 0xC0

class Comm_prot
{
public:
	Comm_prot();
	void Parse(uint8_t* frame);
#ifdef DEBUG
	void Prepare(uint8_t trans_val, uint8_t sub_pos, uint8_t status); 			// debug
#else
	void Prepare(uint8_t status);
#endif
private:
	uint8_t Crc8(uint8_t* frame, uint8_t len);
	uint8_t address;
};

extern Comm_prot comm;

#endif /* COMM_PROT_H_ */
