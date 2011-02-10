/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of Avrlib.
 *
 * Avrlib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * Avrlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with Avrlib; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Peripheral: eeprom
 * Devices: Megax8
 */

#include <avr/io.h>
#include "eeprom.h"

#define EEPROM_SIZE 512

/*
 * e2p_init
 * Initialisation de l'EEPROM
 * entrée: rien
 * sortie: rien
 */
void e2p_init(void)
{
	/* rien de special */
}

/*
 * e2p_read
 * Lecture d'un bloc de donnée en eeprom
 * entrée: offset: Deplacement par rapport au debut de l'eeprom
 *         length: longueur du bloc a ecrire
 * sortie: status 
 *         buffer: pointeur sur un bloc de donnée a remplir
 */
e2p_status e2p_read(unsigned int offset, unsigned int length, unsigned char *buffer)
{
register int i;

	if (offset+length>=EEPROM_SIZE) return ADDR_ERROR;

	for (i=0; i<length; i++)
	{
		EEAR = (unsigned int)(offset+i);
		EECR = EECR | (1<<EERE);
		buffer[i]=EEDR;
	}

	return SUCCESS;
}

/*
 * e2p_write
 * Ecriture d'un bloc de donnée en eeprom
 * entrée: offset: Deplacement par rapport au debut de l'eeprom
 *         length: longueur du bloc a ecrire
 *         buffer: pointeur sur un bloc de donnée à utiliser
 * sortie: status 
 */
e2p_status e2p_write(unsigned int offset, unsigned int length, unsigned char *buffer)
{
register int i;

	if (offset+length>=EEPROM_SIZE) return ADDR_ERROR;

	for (i=0; i<length; i++)
	{
		while (EECR & (1<<EEPE));

		EEAR = (unsigned int)(offset+i);
		EEDR = (unsigned char)buffer[i];
		EECR = EECR | (1<<EEMPE);
		EECR = EECR | (1<<EEPE);
	}

	return SUCCESS;
}
