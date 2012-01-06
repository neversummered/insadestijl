/**
 * \file timer.c
 * \brief header for timer and motor driver
 * \author Sebastien DI MERCURIO
 * 
 * timer.h file is header for timer and motor driver
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

void callback_activite(void);

#define MOTEUR_VITESSE_NORMAL	0x35
#define MOTEUR_VITESSE_TURBO	0x70

#define __WITH_WDT__

char WDT_demarre;
unsigned int WDT_compteur;
char WDT_fautes;
char WDT_etat;

#define SEUIL_MIN_WDT	34800 /* 950 ms */
#define SEUIL_MAX_WDT   37800 /* 1050 ms */

#define WDT_FAUTES_MAX	10

extern struct ST_EEPROM params;
extern void clignotte_led(void);

void timer_init(void)
{
	/* Rappel: FCPU = 8000000 Hz */
	/* Reglage du Timer 1 (16 bits), canal A en pwm */
	/* Utilisation pour moduler la led IR. Periode = 36 Khz */

	/* Connecte OC1A sur le port, Regle le timer en mode fast PWM 14*/
	TCCR1A = (1<<COM1A1)+ (1<<WGM11); 
	TCCR1B = (1<<WGM13) + (1<<WGM12) + (1<<CS10);
	//ICR1 = 222; /* -> Permet une periode de 36000 Hz à 8000000 Hz */
	ICR1 = 204; /* -> Permet une periode de 36000 Hz à 7372800 Hz */
	OCR1A = 5; /* Desactive la fonction */

	TIMSK1 = (1<<TOIE1);
	
	//DDRB = DDRB | (1<<PIN1);

	/* Reglage du Timer 0 (8 bits), canal A et B en fast PWM */
	/* Utilisation pour controler le moteur 1 (Gauche) */
	/* Reglage pour une periode de 8000000/256 -> 31250 Hz */
	TCCR0A = (1<<COM0A1) + (1<<COM0B1) + (3<<WGM00);
	TCCR0B = (1<<CS00);
	OCR0A = 0;
	OCR0B = 0;

	DDRD = DDRD | (1<<PIN5) | (1<<PIN6);

	/* Reglage du Timer 2 (8 bits), canal A et B en fast PWM */
	/* Utilisation pour controler le moteur 2 (Droit) */
	/* Reglage pour une periode de 8000000/256 -> 31250 Hz */
	TCCR2A = (1<<COM2A1) + (1<<COM2B1) + (3<<WGM20);
	TCCR2B = (1<<CS20);
	OCR2A = 0;
	OCR2B = 0;

	DDRD = DDRD | (1<<PIN3);
	DDRB = DDRB | (1<<PIN3);

	/* Watchdog */
	WDT_compteur = 0;
	WDT_etat = WDT_ETAT_ATTENTE;
	WDT_demarre=0;
	WDT_fautes = 0;
}			

ISR(TIMER1_OVF_vect, ISR_BLOCK)
{
static unsigned int compteur=0;

#ifdef __WITH_WDT__
	if (WDT_demarre==1)
	{
		WDT_compteur ++;

		if (WDT_compteur > SEUIL_MAX_WDT) 
		{
			if (WDT_fautes> WDT_FAUTES_MAX)
			{
				WDT_etat = WDT_ETAT_INACTIF;
				DDRB = DDRB & ~(1<<PIN1);
				regle_moteur(MOTEUR_GAUCHE, MOTEUR_STOP);
				regle_moteur(MOTEUR_DROIT, MOTEUR_STOP);
			}
			else
			{
				WDT_fautes++;
				WDT_compteur =0;	
			}
		}
	}
#endif /* __WITH_WDT__ */

	compteur ++;
	if (compteur >=3600) /* environ 100 ms */
	{
		compteur =0;
		callback_activite();
	}	
}

void demarre_systeme(char wdt)
{
	if (wdt == WITH_WDT) WDT_demarre = 1;
	WDT_compteur = 0;
	WDT_fautes = 0;

	WDT_etat = WDT_ETAT_ACTIF;

	/* Demarrage de la scrutation de la balle */
	//OCR1A = 5;
	DDRB = DDRB | (1<<PIN1);
}

char acquite_WDT(void)
{
#ifdef __WITH_WDT__
	if (WDT_demarre==1)
	{
		if (WDT_compteur < SEUIL_MIN_WDT) 
		{
			if (WDT_fautes > WDT_FAUTES_MAX)
			{
				WDT_etat = WDT_ETAT_INACTIF;
				DDRB = DDRB & ~(1<<PIN1);
				regle_moteur(MOTEUR_GAUCHE, MOTEUR_STOP);
				regle_moteur(MOTEUR_DROIT, MOTEUR_STOP);
			}
			else
			{
				WDT_fautes++;
				WDT_compteur=0;	
			}
		}
		else 
		{
			if (WDT_fautes !=0) WDT_fautes--;
			WDT_compteur =0;
		}
	}	

	return WDT_etat;
#else
	return WDT_ETAT_ACTIF;
#endif /* __WITH_WDT__ */ 
}

void reset_WDT(void)
{
	WDT_demarre=0;
	WDT_compteur = 0;
	WDT_fautes = 0;
	WDT_etat = WDT_ETAT_ATTENTE;
	//OCR1A = 0;
	DDRB = DDRB & ~(1<<PIN1);
}

char etat_systeme(void)
{
	return WDT_etat;
}

void systeme_faible_vbat(void)
{
	WDT_etat = WDT_ETAT_VBAT;
	DDRB = DDRB & ~(1<<PIN1);
	regle_moteur(MOTEUR_GAUCHE, MOTEUR_STOP);
	regle_moteur(MOTEUR_DROIT, MOTEUR_STOP);
}

char regle_moteur(char moteur, signed char cmd)
{
char status = 1;

	if (moteur == MOTEUR_GAUCHE) 
	{
		switch (cmd)
		{
		case MOTEUR_STOP:
			OCR0A = 0xFF;
			OCR0B = 0xFF;
			break;
		case MOTEUR_AVANT:
			OCR0A = params.motorLeftNormal;
			OCR0B = 0;			
			break;
		case MOTEUR_ARRIERE:
			OCR0A = 0;
			OCR0B = params.motorLeftNormal;
			break;
		case MOTEUR_AVANT_TURBO:
			OCR0A = params.motorLeftTurbo;
			OCR0B = 0;
			break;
		case MOTEUR_ARRIERE_TURBO:
			OCR0A = 0;
			OCR0B = params.motorLeftTurbo;
			break;
		default:
			status = 0;
			break;
		}
	}
	else /* Moteur Droit */
	{
		switch (cmd) 
		{
		case MOTEUR_STOP:
			OCR2A = 0xFF;
			OCR2B = 0xFF;
			break;
		case MOTEUR_ARRIERE:
			OCR2A = params.motorRightNormal;
			OCR2B = 0;			
			break;
		case MOTEUR_AVANT:
			OCR2A = 0;
			OCR2B = params.motorRightNormal;
			break;
		case MOTEUR_ARRIERE_TURBO:
			OCR2A = params.motorRightTurbo;
			OCR2B = 0;
			break;
		case MOTEUR_AVANT_TURBO:
			OCR2A = 0;
			OCR2B = params.motorRightTurbo;
			break;
		default:
			status = 0;
			break;
		}
	}

	return status;
} 


