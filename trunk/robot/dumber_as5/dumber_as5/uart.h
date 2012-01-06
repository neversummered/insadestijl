/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of Dumber project.
 *
 * Dumber is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * Dumber is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with Dumber; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __UART_H__
#define __UART_H__

#include <stdio.h>

#ifdef F_CPU
#undef F_CPU
#define F_CPU 7372800UL
#endif

#define UART_BAUDRATE 9600
#define UART_RX_BUFFER_SIZE 100

extern unsigned char UART_RX_BufferLength;

#define UartGetBufferSize() UART_RX_BufferLength

void uart_init(void);
void UartPutchar(char c);
char UartGetchar(void);

int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

#endif /* __UART_H__ */
