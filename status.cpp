/*
 * status.cpp
 *
 *  Created on: 26 lip 2017
 *      Author: tomek
 */

#include <avr/io.h>
#include "status.h"
void AdcInit()
{
	//ADMUX |= (1 << REFS1) | (1 << REFS0); // wewnetrzne zrodlo odniesienia
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADEN) | (1 << ADSC);
	ADMUX = 0;
	//ADMUX |=  (1 << REFS0); // AVCC
}

uint16_t AdcRead(uint8_t kanal)
{
    uint16_t pomiar = 0;
//	unsigned char i;

	ADMUX = (ADMUX & 0xE0) + kanal;
	//for (i = 1; i <= ILOSC_PROBEK; i++)
	//{
		ADCSRA |= (1 << ADSC);
		while(ADCSRA & (1 << ADSC));
		//pomiar += ADCW;
		pomiar = ADCW;
	//}
	return pomiar;
	//return pomiar / ILOSC_PROBEK;
}

uint8_t GetStatus()
{
	uint8_t adres = 0;
	int wart_adc = 0;

// -------------------------------------------------------------------------
	// Odczyt pinów 1, 2
	wart_adc = AdcRead(ADC_BITY_1_2);
	if (wart_adc > V_BITY_R_R)
	{
		adres &= 0b11111100;
	}
	else if (wart_adc > V_BITY_Z_R)
	{
		adres |= 0b00000001;
		adres &= 0b11111101;
	}
	else if (wart_adc > V_BITY_R_Z)
	{
		adres |= 0b00000010;
		adres &= 0b11111110;
	}
	else
	{
		adres |= 0b00000011;
	}

// -------------------------------------------------------------------------
	// Odczyt pinów 3, 4
	wart_adc = AdcRead(ADC_BITY_3_4);
	if (wart_adc > V_BITY_R_R)
	{
		adres &= 0b11110011;
	}
	else if (wart_adc > V_BITY_Z_R)
	{
		adres |= 0b00000100;
		adres &= 0b11110111;
	}
	else if (wart_adc > V_BITY_R_Z)
	{
		adres |= 0b00001000;
		adres &= 0b11111011;
	}
	else
	{
		adres |= 0b00001100;
	}

	// -------------------------------------------------------------------------
	// Odczyt pinów 5, 6
	wart_adc = AdcRead(ADC_BITY_5_6);
	if (wart_adc > V_BITY_R_R)
	{
		adres &= 0b11001111;
	}
	else if (wart_adc > V_BITY_Z_R)
	{
		adres |= 0b00010000;
		adres &= 0b11011111;
	}
	else if (wart_adc > V_BITY_R_Z)
	{
		adres |= 0b00100000;
		adres &= 0b11101111;
	}
	else
	{
		adres |= 0b00110000;
	}

	if (adres > 50) return 100;
	else
	return adres;
}

