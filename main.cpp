/*
 * main.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "timer.h"
#include "modbus_rtu.h"
#include "electromagnet.h"


Timer timer(T0_PS_64, 229);
Usart usart;
UsartData usart_data;
ModbusRTU modbus_rtu;
Electromagnet electromagnet;

int main()
{
	DDRC |= (1 << 0); 		// led
	//DDRC |= (1 << 2);
	//timer.Assign(0, 1000, ElmSW);
	sei();
	while(1);
}



