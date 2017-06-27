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


Timer timer(T0_PS_64, 229);
Usart usart;
UsartData usart_data;
ModbusRTU modbus_rtu;

int main()
{
	DDRC |= (1 << 0);
	//timer.Assign(1, 100, ModbusRTU35T);
	//
	sei();
	while(1);
}



