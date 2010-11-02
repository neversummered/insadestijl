/*
 ============================================================================
 Name        : main.c
 Author      : Sebastien DI MERCURIO
 Version     :
 Copyright   :
 Description : Programme principal de test des commandes robot
 ============================================================================
 */

#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>

#include "lib_robot.h"

int i;
int fin;

int sensor;
int odo_left, odo_right;
int vbat;

void print_menu(void)
{
	printf ("Menu:\n");
	printf ("\t1=Start Robot\n\t2=Set Motors[0,0]\n\t3=Set Motors[1,-1]\n\t4=Set Motors[1,1]\n");
	printf ("\t5=Get sensor\n\t6=Get Odo\n\t7=Get VBAT\n\t8=Reset WDT\n\t9=Stop Robot\n\t10=Unknown command\n\t11=Set Motors[2,2]\n\n");
	printf ("\t0=End test\n\n");
	printf ("Enter your command > ");
}

int main(void)
{
	i=0;

	//RobotComFilename="/dev/ttyUSB1";
	if(RobotOpenCom()!=STATUS_OK) exit (0);

	fin = 0;

	while (fin == 0)
	{
		print_menu();
		scanf("%d", &i);

		switch (i)
		{
		case 0:
			printf ("End test !\nByebye\n");
			fin=1;
			break;
		case 1:
			printf ("Start Robot!\n");
			RobotStart();
			break;
		case 2:
			printf ("Set Motor [0,0]!\n");
			RobotSetMotors(0,0);
			break;
		case 3:
			printf ("Set Motor [1,-1]!\n");
			RobotSetMotors(1,-1);
			break;
		case 4:
			printf ("Set Motor [1,1]!\n");
			RobotSetMotors(1,1);
			break;
		case 5:
			printf ("Get sensor!\n");
			RobotGetSensor(&sensor);
			break;
		case 6:
			printf ("Get Odo!\n");
			RobotGetOdo(&odo_left, &odo_right);
			break;
		case 7:
			printf ("Get VBAT!\n");
			RobotGetVBat(&vbat);
			break;
		case 8:
			printf ("Reset WDT!\n");
			RobotReloadWdt();
			break;
		case 9:
			printf ("Stop Robot!\n");
			RobotStop();
			break;
		case 10:
			printf ("Send unknown command!\n");
			RobotStop();
			break;
		case 11:
			printf ("Set Motor [2,2]!\n");
			RobotSetMotors(2,2);
			break;
		default:
			printf ("Invalid number [%d]: try again !\n", i);
			break;
		}
	}

	RobotCloseCom();

	printf ("Fin du prog\n");
	return EXIT_SUCCESS;
}
