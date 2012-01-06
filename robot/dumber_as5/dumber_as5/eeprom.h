/**
 * \file eeprom.h
 * \brief header for EEPROM driver
 * \author Sebastien DI MERCURIO
 * 
 * eeprom.h file is header for EEPROM driver
 */

#ifndef _EEPROM_H_
#define _EEPROM_H_

/**
 * \brief Status type for EEPROM functions 
 */ 
typedef unsigned char e2p_status;

enum
{
	SUCCESS=0, /**< Return when EEPROM functions succeed */
	ADDR_ERROR /**< Return when EEPROM functions failed due to invalid address */
};

/*
 * e2p_init
 * EEPROM Initialization
 */
void e2p_init(void);

/*
 * e2p_read
 * Read data in EEPROM
 */
e2p_status e2p_read(unsigned int offset, unsigned int length, unsigned char *buffer);

/*
 * e2p_write
 * Write data in EEPROM 
 */
e2p_status e2p_write(unsigned int offset, unsigned int length, unsigned char *buffer);

#endif /* _EEPROM_H_ */

 
