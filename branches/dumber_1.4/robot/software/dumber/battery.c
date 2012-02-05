/**
 * \file battery.c
 * \brief battery control functions
 * \author Sebastien DI MERCURIO
 * 
 * battery.c file contains functions related to battery
 */
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "battery.h"

unsigned char vbat_array[16];

char battery_counter;

unsigned char vbat;

/*
 * Constants used for battery monitoring
 */ 
#define VBAT_SEUIL_ALERTE		185
#define VBAT_SEUIL_STOP 		174

/**
 * \brief Battery initialization
 *
 * This function initialize ADC for battery voltage monitoring.
 * There is neither input parameters nor output parameters.
 */
void BATTERYInit(void)
{
unsigned char i;

	/* Mise en route de l'ADC pour la surveillance de la batterie */
	ADMUX = (1<<REFS0) + (1<<ADLAR) + (6<<ADMUX); /* Conversion d'ADC6, ajustement à gauche et utilisation d'AVCC comme reference */
	ADCSRB = 0;
	ADCSRA = (1<<ADEN) + (1<<ADATE) + (7<<ADPS0);
	ADCSRA = ADCSRA | (1<<ADSC);
	
	battery_counter =0;
	vbat = 0;
	
	for (i=0; i<16; i++)
	{
		vbat_array[i] = 0;
	}
}

/**
 * \brief Shift values in battery array
 *
 * Shift values in battery array to the left and set rightmost value to O.
 */
void BATTERYShift (void)
{
unsigned char i;

	for (i=0; i<15; i++)
	{
		vbat_array[i] = vbat_array[i+1];
	}
	
	vbat_array[15] = 0;		
}

/**
 * \brief Get battery voltage value
 *
 * This function is used to get battery voltage and return a level depending on this value.
 * @return Battery level
 */
char BATTERYVbatLevel(void)
{
	if (vbat == 0) /* premiere conversion -> ne pas en tenir compte */
	{
		return BAT_NORMAL;
	}
	else if (vbat < VBAT_SEUIL_STOP)
	{
		return BAT_EMPTY;
	}
	else if (vbat < VBAT_SEUIL_ALERTE)
	{
		return BAT_LOW_BAT;
	}
	else return BAT_NORMAL;
}

/**
 * \brief Sample vbat value and filter vbat
 *
 * This function is used to sample vbat and filter its value.
 */
void BATTERYSampleVbat(void)
{
unsigned char i;
unsigned short temp;

	BATTERYShift();
	
	/* Lire l'ADC */
	vbat_array[15] = (unsigned char) ADCH;
	
	if (battery_counter < 16) battery_counter++;
	
	if (battery_counter >=16)
	{
		temp = 0;
		
		for (i=0; i<16; i++)
		{
			temp = temp + vbat_array[i];
		}
		
		vbat = temp >> 4;
	}		
}