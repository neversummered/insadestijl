/*
 ============================================================================
 Name        : robot.h
 Author      : Sebastien DI MERCURIO
 Version     :
 Copyright   :
 Description : Interface de la bibliotheque de commande du robot
 ============================================================================
 */

#ifndef __ROBOT_H__
#define __ROBOT_H__

extern char *RobotComFilename;
typedef int RobotStatus;

#define STATUS_OK 					0
#define STATUS_ERR_NO_FILE			1
#define STATUS_ERR_TIMEOUT			2
#define STATUS_ERR_UNKNOWN_CMD		3
#define STATUS_ERR_INVALID_PARAMS	4
#define STATUS_ERR_WDT_EXPIRED		5
#define STATUS_ERR_SELECT			6
#define STATUS_ERR_UNKNOWN			0xFF

RobotStatus RobotOpenCom(void);
RobotStatus RobotCloseCom(void);

RobotStatus RobotStart(void);
RobotStatus RobotStop(void);

RobotStatus RobotSendChar(char c);
RobotStatus RobotGetChar(char *c);

RobotStatus RobotSetMotors(int motor_left, int motor_right);
RobotStatus RobotReloadWdt(void);
RobotStatus RobotGetSensor(int *sensor);
RobotStatus RobotGetOdo(int *odo_left, int *odo_right);
RobotStatus RobotGetVBat(int *vbat);

#endif /* __ROBOT_H__ */
