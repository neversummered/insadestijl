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
#include "timer.h"
#include "eeprom.h"

#include <stdio.h>

void divers_init(void);
char etat_vbat(void);
void allume_led(void);
void eteint_led(void);
char etat_detection_balle(void);

char buffer_cmd[32];

unsigned char motorLeftNormal;
unsigned char motorRightNormal;
unsigned char motorLeftTurbo;
unsigned char motorRightTurbo;

struct ST_EEPROM params;

#define VBAT_SEUIL_ALERTE	0x88
#define VBAT_SEUIL_STOP 	0x7C

#define FW_MAJOR_VER		1
#define FW_MINOR_VER		1

/*
 * Fonction: init_periph
 * Initialise l'ensemble des peripheriques
 * Param. entrée: Aucun
 * Param. sortie: Aucun 
 */ 
void init_periph(void)
{
	/* Recuperation des parametres en EEPROM*/
	e2p_init();
	
	e2p_read(0, sizeof(struct ST_EEPROM), (unsigned char*)&params);

	if (params.eepromVer == 0xFF) /* la table est invalide: a initialiser */
	{
		params.eepromVer=1;
		params.motorLeftNormal=0x35;
		params.motorLeftTurbo=0x70;
		params.motorRightNormal=0x35;
		params.motorRightTurbo=0x70;

		e2p_write (0, sizeof(struct ST_EEPROM), (unsigned char*) &params);
	}
	
	uart_init();

	timer_init();

	divers_init();
}

/*
 * Fonction: main
 * fonction principale
 * Param. entrée: Aucun
 * Param. sortie: Aucun (non utilisée)
 */ 
int main (void)
{
char c;
char *ptr_cmd;
char cmd_received;

int moteur_droit, moteur_gauche;
int odo_gauche, odo_droit;

	init_periph();
	
	cmd_received= 0;
	ptr_cmd = buffer_cmd;

	moteur_droit =0;
	moteur_gauche = 0;

	odo_gauche=0;
	odo_droit=0;

	sei();

	for (;;)
	{
		/* Verification de la batterie */
		if (etat_vbat()==0)
		{
			systeme_faible_vbat();
		}

		if (UartGetBufferSize() != 0)
		{
			c= UartGetchar();
			
			switch (c)
			{
			case '\b': /* backspace char = flush buffer */
				ptr_cmd = buffer_cmd;
				cmd_received = 0;
				break;
			case '\n': /* newline char  = end of command*/
			case '\r':
				*ptr_cmd = 0;
				cmd_received = 1;
			default:
				*ptr_cmd = c;
				ptr_cmd ++;	
			}	
		} 
		
		if (cmd_received)
		{
			ptr_cmd = buffer_cmd;
			cmd_received=0;
			
			if ((etat_systeme() == WDT_ETAT_ACTIF) ||
			    (etat_systeme() == WDT_ETAT_ATTENTE))
			{
				switch (*ptr_cmd)
				{
					case 'p': /* ping command: check if robot is on */
						printf ("O\n");
						break;
					case 'm': /* motor command: set motor speed and dir */
						if (sscanf (ptr_cmd, "m=%i,%i", &moteur_gauche, &moteur_droit)==2)
						{
							if ((regle_moteur(1, moteur_gauche)) && 
							    (regle_moteur(2, moteur_droit)))
							{
								printf ("O\n");
							}
							else printf ("E\n");
						}
						else
						{
							printf ("E\n");
						} 
						break;
					case 'W': /* start watchdog */
						demarre_WDT();
						printf ("O\n");
						break;
					case 'w': /* reset watchdog */
						if (acquite_WDT() == WDT_ETAT_INACTIF)
						{
							printf ("E\n");
						}
						else
						{
							printf ("O\n");
						}
						break;
					case 's': /* sensor cmd: return sensor state */
						printf ("O:%d\n",etat_detection_balle());
						break;
					case 'o': /* odometrie cmd: return odometrie */
						printf ("O:%d,%d\n", odo_gauche, odo_droit);
						break;
					case 'v': /* voltage cmd: return voltage state */
						printf ("O:%d\n", etat_vbat());
						break;
					case 'r': /* reset system */
						regle_moteur(1, MOTEUR_STOP);
						regle_moteur(2, MOTEUR_STOP);
						
						odo_gauche=0;
						odo_droit=0;
						
						reset_WDT();
						printf ("O\n");
						break;
					case 'V': /* return FW version */
						printf ("O:%d,%d\n", FW_MAJOR_VER, FW_MINOR_VER);
						break;
					case 'S': /* set motors speed */
						if (sscanf (ptr_cmd, "S=%i,%i,%i,%i", 
						    &motorLeftNormal, &motorLeftTurbo,
							&motorRightNormal, &motorRightTurbo)==4)
						{
							printf ("O\n");
							
							params.motorLeftNormal=motorLeftNormal;
							params.motorLeftTurbo=motorLeftTurbo;
							params.motorRightNormal=motorRightNormal;
							params.motorRightTurbo=motorLeftNormal;
						}
						else
						{
							printf ("E\n");
						}
						break;
					case 'R': /* record motors speed */
						printf ("O\n");
						e2p_write (0, sizeof(struct ST_EEPROM), (unsigned char*) &params);
						break;
					case 'l':
						break;
					default: /* unknown cmd */
						printf ("C:%c\n",*ptr_cmd);	
						break;
				}
			}
			else if (etat_systeme() == WDT_ETAT_INACTIF) /* System inactif (du au WDT) */
			{
				switch (*ptr_cmd)
				{
					case 'p': /* ping command: check if robot is on */
						printf ("O\n");
						break;
					case 'r': /* reset system */
						regle_moteur(1, MOTEUR_STOP);
						regle_moteur(2, MOTEUR_STOP);
						
						odo_gauche=0;
						odo_droit=0;
						
						reset_WDT();
						printf ("O\n");
						break;
					case 'm': /* motor command: set motor speed and dir */
					case 'W': /* start watchdog */
					case 'w': /* reset watchdog */
					case 's': /* sensor cmd: return sensor state */	
					case 'o': /* odometrie cmd: return odometrie */
					case 'v': /* voltage cmd: return voltage state */
					case 'V': /* return FW version */
					case 'S': /* set motors speed */
					case 'R': /* record motors speed */
						printf ("E\n"); /* Commande non autorisée */
						break;
					default: /* unknown cmd */
						printf ("C:%c\n",*ptr_cmd);	
						break;
				}
			}
		}
	}
}

void divers_init(void)
{
	/* Mise en route de l'ADC pour la surveillance de la batterie */
	ADMUX = (1<<REFS0) + (1<<ADLAR); /* Conversion d'ADC0, ajustement à gauche et utilisation d'AVCC comme reference */
	ADCSRB = 0;
	ADCSRA = (1<<ADEN) + (1<<ADATE) + (7<<ADPS0);
	ADCSRA = ADCSRA | (1<<ADSC);

	/* Mise en route de la LED */
	DDRB = DDRB | (1<<PIN0);
	PORTB = PORTB & ~(1<<PIN0);
}

char etat_vbat(void)
{
unsigned char vbat;
	/* Lire l'ADC */

	vbat = (unsigned char) ADCH;
	
	if (vbat == 0) /* premiere conversion -> ne pas en tenir compte */
	{
		return 2;
	}
	else if (vbat <VBAT_SEUIL_STOP)
	{
		return 0;
	}
	else if (vbat<VBAT_SEUIL_ALERTE)
	{
		return 1;
	}
	else return 2;
}

void allume_led(void)
{
	PORTB = PORTB | (1<<PIN0);
}

void eteint_led(void)
{
	PORTB = PORTB & ~(1<<PIN0);
}

void clignotte_led(void)
{
	PORTB = PORTB ^ (1<<PIN0);
}

char etat_detection_balle(void)
{
unsigned char etat;
	
	etat = PIND;
	etat = etat & (1<<PIN2);
	etat = etat >> PIN2;

	return (etat);
}

void callback_activite(void)
{
static char compteur=0;
char etat;

	/* Appelée toute les 100ms environ */

	compteur ++;
	etat = etat_systeme();

	switch (etat)
	{
	case WDT_ETAT_ATTENTE:
		if (compteur <=1) allume_led();
		else if (compteur <10) eteint_led();
		else compteur =0; 
		break;
	case WDT_ETAT_ACTIF:
		allume_led();
		compteur = 0;
		break;
	case WDT_ETAT_INACTIF:
		if (compteur>=10)
		{
			clignotte_led();
			compteur = 0;	
		}
		break;
	case WDT_ETAT_VBAT:
		if (compteur>=1)
		{
			clignotte_led();
			compteur = 0;	
		}		
		break;
	}
}

