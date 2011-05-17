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

#ifndef __LED_H__
#define __LED_H__

#define LED_ROUGE LED2
#define LED_VERTE LED1

#define ETEINT	1
#define ALLUME	0

#define XBEE_RESET_ON 0
#define XBEE_RESET_OFF 1
#define XBEE_SLEEP_ON 1
#define XBEE_SLEEP_OFF 0

#define APPUI	0
#define RELACHE (!APPUI)

#define LED1 10
#define LED2 11
#define SW1 20
#define SW2 21
#define BUTTON 22
#define RESET_XBEE 23
#define SLEEP_XBEE 24

void LEDInit(void);
void LEDSet(char led, char etat);
char LEDEtat(char led);

#endif /* __LED_H__ */
