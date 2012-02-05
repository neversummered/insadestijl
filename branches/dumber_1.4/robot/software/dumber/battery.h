/**
 * \file battery.h
 * \brief Header for battery functions
 * \author Sebastien DI MERCURIO
 * 
 * battery.h file is header for battery functions
 */


#ifndef BATTERY_H_
#define BATTERY_H_

#define BAT_NORMAL		2
#define BAT_LOW_BAT		1
#define BAT_EMPTY		0

void BATTERYInit(void);
char BATTERYVbatLevel(void);
void BATTERYSampleVbat(void);

#endif /* BATTERY_H_ */