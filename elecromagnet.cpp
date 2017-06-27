/*
 * elecromagnet.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include <avr/io.h>
#include "electromagnet.h"
#include "timer.h"
#include "modbus_rtu.h"
#include "usart.h"

Electromagnet::Electromagnet()
{
	ELECTROMAGNET_INIT;

}

void ElectromOff()
{
	ELECTROMAGNET_OFF;
	modbus_rtu.PrepareFrame(usart_data.frame);
	timer.Disable(2);
}

bool Electromagnet::CheckCoil()
{
	ELECTROMAGNET_ON;
	timer.Assign(2, 100, ElectromOff);
	return true;
}



