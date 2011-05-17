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
#include <avr/sleep.h>

#include "uart.h"
#include "timer.h"
#include "eeprom.h"

#include <stdio.h>

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, uart_getchar,
                                             _FDEV_SETUP_RW);


void divers_init(void);
char etat_vbat(void);
void allume_led(void);
void eteint_led(void);
char etat_detection_balle(void);

unsigned char buffer_cmd[32];

#define MOTOR_LEFT_NORMAL 1
#define MOTOR_LEFT_TURBO 2
#define MOTOR_RIGHT_NORMAL 3
#define MOTOR_RIGHT_TURBO 4

unsigned char motorLeftNormal;
unsigned char motorRightNormal;
unsigned char motorLeftTurbo;
unsigned char motorRightTurbo;

struct ST_EEPROM params;

#define VBAT_SEUIL_ALERTE		0x88
#define VBAT_SEUIL_STOP 		0x7C

#define FW_MAJOR_VER			1
#define FW_MINOR_VER			2

#define CMD_PING 				'p'
#define CMD_RESET				'r'
#define CMD_SET_MOTORS			'm'
#define CMD_START_WATCHDOG		'W'
#define CMD_RESET_WATCHDOG  	'w'
#define CMD_GET_SENSOR 			's'
#define CMD_GET_ODO				'o'
#define CMD_GET_VBAT			'v'
#define CMD_GET_VERSION			'V'
#define CMD_SET_LEFT_NORMAL		'Y'
#define CMD_SET_LEFT_TURBO		'y'
#define CMD_SET_RIGHT_NORMAL	'Z'
#define CMD_SET_RIGHT_TURBO		'z'
#define CMD_RECORD_PARAMS		'R'
#define CMD_GET_PARAMS			'G'
#define CMD_UNSECURE_START		'u'

const char OK_ANS[]="O\n";
const char ERR_ANS[]="E\n";

extern char WDT_fautes;

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
unsigned char c;
unsigned char *ptr_cmd;
unsigned char cmd_received;

int moteur_droit, moteur_gauche;
int odo_gauche, odo_droit;

char system;

	stdout = &mystdout;

	init_periph();
	
	cmd_received= 0;
	ptr_cmd = buffer_cmd;

	moteur_droit =0;
	moteur_gauche = 0;

	odo_gauche=0;
	odo_droit=0;
	
	system = WDT_ETAT_ATTENTE;
	
	set_sleep_mode(SLEEP_MODE_IDLE);

	sei();

	for (;;)
	{
		sleep_mode();

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
			
			system = etat_systeme();
			
			switch (system)
			{
			case WDT_ETAT_ACTIF:
				switch (*ptr_cmd)
				{
					case CMD_PING: /* ping command: check if robot is on */
						printf (OK_ANS);
						break;
					case CMD_SET_MOTORS: /* motor command: set motor speed and direction */
						if (sscanf ((char *)ptr_cmd, "m=%i,%i", &moteur_gauche, &moteur_droit)==2)
						{
							if ((regle_moteur(MOTEUR_GAUCHE, moteur_gauche)) && 
							    (regle_moteur(MOTEUR_DROIT, moteur_droit)))
							{
								printf (OK_ANS);
							}
							else printf (ERR_ANS);
						}
						else
						{
							printf (ERR_ANS);
						} 
						break;
					case CMD_RESET_WATCHDOG: /* reset watchdog */
						if (acquite_WDT() == WDT_ETAT_INACTIF)
						{
							printf (ERR_ANS);
						}
						else
						{
							printf ("O:%d\n",WDT_fautes);
						}
						break;
					case CMD_GET_SENSOR: /* sensor cmd: return sensor state */
						printf ("O:%d\n",etat_detection_balle());
						break;
					case CMD_GET_ODO: /* odometers cmd: return odometers */
						printf ("O:%d,%d\n", odo_gauche, odo_droit);
						break;
					case CMD_GET_VBAT: /* voltage cmd: return voltage state */
						printf ("O:%d\n", etat_vbat());
						break;
					case CMD_RESET: /* reset system */
						regle_moteur(MOTEUR_GAUCHE, MOTEUR_STOP);
						regle_moteur(MOTEUR_DROIT, MOTEUR_STOP);
						
						odo_gauche=0;
						odo_droit=0;
						
						reset_WDT();
						printf (OK_ANS);
						break;
					case CMD_GET_VERSION: /* return FW version */
						printf ("O:%d,%d\n", FW_MAJOR_VER, FW_MINOR_VER);
						break;
					case CMD_SET_LEFT_NORMAL: /* set motor left speed normal*/
						if (sscanf ((char *)ptr_cmd, "Y=%u", (unsigned int*)&params.motorLeftNormal)==1)
						{
							printf (OK_ANS);
						}
						else
						{
							printf (ERR_ANS);
						}
						break;
					case CMD_SET_LEFT_TURBO: /* set motor left speed turbo */
						if (sscanf ((char *)ptr_cmd, "y=%u", (unsigned int*)&params.motorLeftTurbo)==1)
						{
							printf (OK_ANS);
						}
						else
						{
							printf (ERR_ANS);
						}
						break;
					case CMD_SET_RIGHT_NORMAL: /* set motor right speed normal*/
						if (sscanf ((char *)ptr_cmd, "Z=%u", (unsigned int*)&params.motorRightNormal)==1)
						{
							printf (OK_ANS);
						}
						else
						{
							printf (ERR_ANS);
						}
						break;
					case CMD_SET_RIGHT_TURBO: /* set motor right speed turbo*/
						if (sscanf ((char *)ptr_cmd, "z=%u", (unsigned int*)&params.motorRightTurbo)==1)
						{
							printf (OK_ANS);	
						}
						else
						{
							printf (ERR_ANS);
						}
						break;	
					case CMD_RECORD_PARAMS: /* record params */
						printf (OK_ANS);
						e2p_write (0, sizeof(struct ST_EEPROM), (unsigned char*) &params);
						break;
					case CMD_GET_PARAMS:
						printf ("O:%u,%u,%u,%u\n",params.motorLeftNormal,params.motorLeftTurbo,
						                          params.motorRightNormal,params.motorRightTurbo);
						break;
					case CMD_START_WATCHDOG:    /* start watchdog */
					case CMD_UNSECURE_START:    /* start insecurely*/
						printf (ERR_ANS); /* Commande non autorisée */
						break;
					default: /* unknown cmd */
						printf ("C:%c\n",*ptr_cmd);	
						break;
				}				
				break;
			case WDT_ETAT_ATTENTE:
				switch (*ptr_cmd)
				{
					case CMD_PING: /* ping command: check if robot is on */
						printf (OK_ANS);
						break;
					case CMD_START_WATCHDOG: /* start watchdog */
						demarre_systeme(WITH_WDT);
						printf (OK_ANS);
						break;
					case CMD_UNSECURE_START: /* start insecurely*/
						demarre_systeme(WITHOUT_WDT);
						printf (OK_ANS);
						break;
					case CMD_RESET: /* reset system */
						regle_moteur(MOTEUR_GAUCHE, MOTEUR_STOP);
						regle_moteur(MOTEUR_DROIT, MOTEUR_STOP);
						
						odo_gauche=0;
						odo_droit=0;
						
						reset_WDT();
						printf (OK_ANS);
						break;
					case CMD_GET_VERSION: /* return FW version */
						printf ("O:%d,%d\n", FW_MAJOR_VER, FW_MINOR_VER);
						break;
					case CMD_SET_MOTORS: 		/* motor command: set motor speed and dir */
					case CMD_RESET_WATCHDOG: 	/* reset watchdog */
					case CMD_GET_SENSOR: 		/* sensor cmd: return sensor state */	
					case CMD_GET_ODO: 			/* odometrie cmd: return odometrie */
					case CMD_GET_VBAT: 			/* voltage cmd: return voltage state */
					case CMD_SET_LEFT_NORMAL: 	/* set motor left speed normal*/
					case CMD_SET_LEFT_TURBO: 	/* set motor left speed turbo */
					case CMD_SET_RIGHT_NORMAL: 	/* set motor right speed normal*/
					case CMD_SET_RIGHT_TURBO: 	/* set motor right speed turbo*/
					case CMD_RECORD_PARAMS: 	/* record params */
					case CMD_GET_PARAMS:		/* get params */
						printf (ERR_ANS); /* Commande non autorisée */
						break;
					default: /* unknown cmd */
						printf ("C:%c\n",*ptr_cmd);	
						break;
				}					
				break;
			default:
				 /*		etat_systeme() == WDT_ETAT_INACTIF	=> Systeme inactif (du au WDT) 
			      ||	etat_systeme() == WDT_ETAT_VBAT		=> Batterie trop faible   
				  */
				switch (*ptr_cmd)
				{
					case CMD_PING: /* ping command: check if robot is on */
						printf (OK_ANS);
						break;
					case CMD_RESET: /* reset system */
						regle_moteur(MOTEUR_GAUCHE, MOTEUR_STOP);
						regle_moteur(MOTEUR_DROIT, MOTEUR_STOP);
						
						odo_gauche=0;
						odo_droit=0;
						
						reset_WDT();
						printf (OK_ANS);
						break;
					case CMD_GET_VERSION: /* return FW version */
						printf ("O:%d,%d\n", FW_MAJOR_VER, FW_MINOR_VER);
						break;
					case CMD_SET_MOTORS: 		/* motor command: set motor speed and dir */
					case CMD_START_WATCHDOG: 	/* start watchdog */
					case CMD_UNSECURE_START:    /* start insecurely */
					case CMD_RESET_WATCHDOG: 	/* reset watchdog */
					case CMD_GET_SENSOR: 		/* sensor cmd: return sensor state */	
					case CMD_GET_ODO: 			/* odometrie cmd: return odometrie */
					case CMD_GET_VBAT: 			/* voltage cmd: return voltage state */
					case CMD_SET_LEFT_NORMAL: 	/* set motor left speed normal*/
					case CMD_SET_LEFT_TURBO: 	/* set motor left speed turbo */
					case CMD_SET_RIGHT_NORMAL: 	/* set motor right speed normal*/
					case CMD_SET_RIGHT_TURBO: 	/* set motor right speed turbo*/
					case CMD_RECORD_PARAMS: 	/* record params */
					case CMD_GET_PARAMS:		/* get params */
						printf (ERR_ANS); /* Commande non autorisée */
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

