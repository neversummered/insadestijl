/**
 * \file timer.h
 * \brief header for timer and motor driver
 * \author Sebastien DI MERCURIO
 * 
 * timer.h file is header for timer and motor driver
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

#define WITH_WDT	1
#define WITHOUT_WDT	0

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
void demarre_systeme(char wdt);
char acquite_WDT(void);
void reset_WDT(void);
char etat_systeme(void);
void systeme_faible_vbat(void);

#endif /* __TIMER_H__ */
