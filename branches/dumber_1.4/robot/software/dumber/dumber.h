/**
 * \file dumber.h
 * \brief Header for miscellaneous functions
 * \author Sebastien DI MERCURIO
 * 
 * dumber.h file is header for various functions
 */

#ifndef DUMBER_H_
#define DUMBER_H_

#define STATE_RUNNING	1
#define STATE_DISABLED	0
#define STATE_IDLE		2
#define STATE_POWER_OFF 3

void DUMBERInitPeriph(void);
char DUMBERGetRobotState(void);
void DUMBERSetRobotState(char state);
void DUMBERCallbackActivity(void);

#endif /* DUMBER_H_ */