/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of brushless DC motor controller.
 *
 * Testeur RF is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * Testeur RF is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with Testeur RF; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

unsigned char UART_RX_Buffer[UART_RX_BUFFER_SIZE];
unsigned char UART_RX_BufferIndex;
unsigned char UART_RX_BufferLength;

#define MYUBRR (((F_CPU/16/UART_BAUDRATE-1)*2)+1)

void uart_init(void)
{	
	/*Set baud rate */
	UBRR0H = (unsigned char)(MYUBRR>>8);
	UBRR0L = (unsigned char)MYUBRR;
	/*UBRR0H = 0;
	UBRR0L = 12;*/

	UCSR0A = (1<<U2X0);
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (3<<UCSZ00);
} 

void UartPutchar(char c)
{
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0)));

	/* Put data into buffer, sends the data */
	UDR0 = c;
}

char UartGetchar(void)
{
char tmp;

	tmp =0;

	if (UART_RX_BufferLength!=0)
	{
		tmp =UART_RX_Buffer[UART_RX_BufferIndex];

		UART_RX_BufferIndex++;
		UART_RX_BufferLength--;

		if (UART_RX_BufferIndex>UART_RX_BUFFER_SIZE) UART_RX_BufferIndex =0;
	}

	return tmp;
}

ISR(SIG_USART_RECV, ISR_BLOCK)
{
unsigned char receive_data;
unsigned char index;

	receive_data=UDR0;
	
	if (receive_data != 0)
	{
		if (UART_RX_BufferLength <UART_RX_BUFFER_SIZE)
		{
			index = UART_RX_BufferIndex + UART_RX_BufferLength;
			if (index >= UART_RX_BUFFER_SIZE) index = index - UART_RX_BUFFER_SIZE;
			UART_RX_Buffer[index]=receive_data;
			UART_RX_BufferLength ++;
		}
	}
}

