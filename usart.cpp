/*
 * usart.cpp
 *
 *  Created on: 27 cze 2017
 *      Author: tomek
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"
#include "timer.h"
#include "modbus_rtu.h"
#include "state_machine.h"

Usart::Usart(uint16_t baud) : StateMachine(ST_MAX_STATES)
{
	uint8_t ubrr = F_CPU / 16 / baud - 1;
	US_UBRRH = (uint8_t)(ubrr >> 8);
	US_UBRRL = (uint8_t)ubrr;

	US_UCSRB |= (1 << US_RXEN) | (1 << US_TXEN) | (1 << US_TXCIE);
#if defined (__AVR_ATmega88P__)
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
#endif

#if defined (__AVR_ATmega8__)
	UCSRC |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
#endif
	USART_DE_INIT;
	rx_head = 0;
	rx_tail = 0;
	tx_head = 0;
	tx_tail = 0;
	RxEnable();
}

void Usart::RxEnable()
{
	USART_DE_RECEIVE;
	US_UCSRB |= (1 << US_RXEN) | (1 << US_RXCIE);
}

void Usart::RxDisable()
{
	US_UCSRB &= ~((1 << US_RXEN) | (1 << US_RXCIE));
}

void Usart::TxEnable()
{
	USART_DE_SEND;
	US_UCSRB |= (1 << US_UDRIE);
}

void Usart::TxDisable()
{
	US_UCSRB &= ~(1 << US_UDRIE);
}

void Usart::ST_Init(UsartData* pdata) {}

void Usart::ST_Idle(UsartData* pdata)
{

}

void Usart::ST_ByteReceived(UsartData* pdata)
{
	uint8_t tmp_head;
	tmp_head = (rx_head + 1) & UART_BUF_MASK;
	if(tmp_head == rx_tail)
	{
		// nadpisanie bufora
	}
	else
	{
		rx_head = tmp_head;
		rx_buf[tmp_head] = pdata->c;
		pdata->len++;
	}
}

void Usart::ST_FrameReceived(UsartData* pdata)
{
	usart_data.len = 8;
	uint8_t i = 0;
	while(rx_tail != rx_head)
	{
		rx_tail = (rx_tail + 1) & UART_BUF_MASK;
		usart_data.frame[i] = rx_buf[rx_tail];
		i++;
	}
	modbus_rtu.ParseFrame(usart_data.frame, 8);
}

void Usart::EV_NewByte(UsartData* pdata)
{
	const uint8_t Transitions[] =
	{
		// musi byc obsluga jesli znak przyjdzie w stanie INIT
		ST_BYTE_RECEIVED,			// ST_INIT
		ST_BYTE_RECEIVED, 			// ST_IDLE
		ST_BYTE_RECEIVED,			// ST_BYTE_RECEIVED
		ST_BYTE_RECEIVED			// ST_FRAME_RECEIVED
	};
	Event(Transitions[current_state], pdata);
}

void Usart::EV_TXBufferEmpty(UsartData* pdata)
{
	if(tx_head != tx_tail)
	{
		tx_tail = (tx_tail + 1) & UART_BUF_MASK;
		US_UDR = tx_buf[tx_tail];
	}
	else
	{
		TxDisable();
	}
}

void Usart::EV_TXComplete(UsartData* pdata)
{
	RxEnable();
}

void Usart::SendFrame(UsartData* pdata)
{
	RxDisable();
	uint8_t tmp_tx_head;
	uint8_t *w = pdata->frame;
	uint16_t len = pdata->len;
	while(len)
	{
		tmp_tx_head = (tx_head  + 1) & UART_BUF_MASK;
		while(tmp_tx_head == tx_tail) {}
		tx_buf[tmp_tx_head] = *w++;
		tx_head = tmp_tx_head;
		len--;
	}

	TxEnable();
}
/*
void Usart::SendInt(UsartData *pdata)
{
	char buf[10];
	itoa(pdata->c, buf, 10);
	//pdata->frame = buf;
	SendFrame(pdata);
}
*/

ISR(US_RX)
{
	usart_data.c = US_UDR;
	usart.EV_NewByte(&usart_data);
}

ISR(USART_UDRE_vect)
{
	usart.EV_TXBufferEmpty();
}

ISR(US_TX)
{
	usart.EV_TXComplete();
}
