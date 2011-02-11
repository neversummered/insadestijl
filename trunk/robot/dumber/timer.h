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

#ifndef __TIMER_H__
#define __TIMER_H__

#define MOTEUR_STOP	0
#define MOTEUR_AVANT 1
#define MOTEUR_ARRIERE -1
#define MOTEUR_AVANT_TURBO 2
#define MOTEUR_ARRIERE_TURBO -2

#define MOTEUR_GAUCHE	1
#define MOTEUR_DROIT	2

#define WDT_ETAT_ACTIF 1
#define WDT_ETAT_INACTIF 0
#define WDT_ETAT_ATTENTE 2
#define WDT_ETAT_VBAT 3

struct ST_EEPROM
{
	unsigned char eepromVer;
	unsigned char motorLeftNormal;
	unsigned char motorLeftTurbo;
	unsigned char motorRightNormal;
	unsigned char motorRightTurbo;
};

void timer_init(void);
char regle_moteur(char moteur, signed char cmd);
void demarre_WDT(void);
char acquite_WDT(void);
void reset_WDT(void);
char etat_systeme(void);
void systeme_faible_vbat(void);

#endif /* __TIMER_H__ */
