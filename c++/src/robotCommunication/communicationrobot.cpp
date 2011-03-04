/*
 ============================================================================
 Name        : robot.c
 Author      : Sebastien DI MERCURIO
 Version     :
 Copyright   :
 Description : Bibliotheque de commande du robot
 ============================================================================
 */

#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>
#include "communicationrobot.h"

#define MAX_RETRY 3
#define MAX_TIMEOUT 10 /* Exprimé en multiple de 5 ms */

    /* Command list */
#define ROBOT_CMD_PING                  'p'
#define ROBOT_CMD_SET_MOTORS            'm'
#define ROBOT_CMD_START_WATCHDOG	    'W'
#define ROBOT_CMD_RELOAD_WATCHDOG	    'w'
#define ROBOT_CMD_GET_SENSOR            's'
#define ROBOT_CMD_GET_VBAT              'v'
#define ROBOT_CMD_GET_ODO               'o'
#define ROBOT_CMD_RESET                 'r'

namespace robotInsa {

    int i;
    char c;
    int com_fd;

    /* Variable pour select */
    fd_set set;
    struct timeval timeout;

    char buffer_out[32];
    char buffer_in[32];
    char *ptr_in, *ptr_out;

    char *RobotComFilename = "/dev/ttyUSB0";
    //char *RobotComFilename = "/dev/tty.usbserial-A700f1t1";                   /* pour mac installer driver http://www.ftdichip.com/Support/Documents/AppNotes/AN_134_FTDI_Drivers_Installation_Guide_for_MAC_OSX.pdf */


    RobotStatus CommunicationRobot::RobotSendChar(char c) {
        write(com_fd, &c, 1);
        status = STATUS_OK;
        return status;
    }

    RobotStatus CommunicationRobot::RobotGetChar(char *c) {
        int n;

        int delay;

        status = STATUS_OK;
        delay = 0;
        n = 0;

        while (n <= 0) {
            n = read(com_fd, c, 1);

            if (n <= 0) {
                delay++;
                usleep(5000);
            }

            if (delay > MAX_TIMEOUT) {
                return STATUS_ERR_TIMEOUT;
            }
        }

        return status;
    }

    RobotStatus CommunicationRobot::RobotManageCmd(const char *cmd, char *ans) {
        RobotStatus status_get;
        int nb_retry;
        char *p_ans;
        char c;
        int cmd_succeed;


        nb_retry = 0;
        status = STATUS_ERR_UNKNOWN_CMD;
        status_get = STATUS_OK;
        cmd_succeed = 0;

        while ((nb_retry < MAX_RETRY) && (cmd_succeed == 0)) {
            p_ans = ans;
            *p_ans = 0;
            c = 0;

            /* Envoi de la commande */
            printf("Envoi cmd: %s\n", cmd);
            write(com_fd, cmd, strlen(cmd));

            while ((c != '\n') && (c != '\r') && (status_get != STATUS_ERR_TIMEOUT)) {
                status_get = RobotGetChar(&c);
                *p_ans = c;
                p_ans++;
            }

            if (status_get == STATUS_ERR_TIMEOUT) {
                nb_retry++;
            } else {
                cmd_succeed = 1;
                p_ans--;
                *p_ans = 0;
            }
        }

        if (nb_retry >= MAX_RETRY) {
            printf("Timeout!!\n");
            status = STATUS_ERR_TIMEOUT;
        }

        printf("Recu ans: %s\n", ans);

        /* Analyse de la reponse */
        switch (*ans) {
            case 'C':
            case 'c':
                printf("Unknown CMD\n");
                status = STATUS_ERR_UNKNOWN_CMD;
                break;
            case 'E':
            case 'e':
                printf("Invalid params\n");
                status = STATUS_ERR_INVALID_PARAMS;
                break;
            case 'O':
            case 'o':
                printf("Status OK!\n");
                status = STATUS_OK;
                break;
            default:
                break;
        }

        return status;
    }

    RobotStatus CommunicationRobot::RobotOpenCom(void) {
        struct termios options;

        int fd; /* File descriptor for the port */
        fd = open(RobotComFilename, O_RDWR | O_NOCTTY | O_NDELAY);

        if (fd == -1) {
            /*
             * Could not open the port.
             */
            fprintf(stderr, "[Error %d] RobotOpenCom: Can't open device file %s\n", STATUS_ERR_NO_FILE, RobotComFilename);
            return STATUS_ERR_NO_FILE;
        } else printf("Opened %s successfully\n", RobotComFilename);

        fcntl(fd, F_SETFL, FNDELAY);

        /*
         * Get the current options for the port...
         */
        tcgetattr(fd, &options);

        /*
         * Set the baud rates to 9600 bauds 8N1, no hardware flow control...
         */
        cfsetispeed(&options, B9600);
        cfsetospeed(&options, B9600);
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
        options.c_cflag &= ~CRTSCTS;
        /*
         * Enable the receiver and set local mode...
         */
        options.c_cflag |= (CLOCAL | CREAD);
        /*
         * Set the new options for the port...
         */
        tcsetattr(fd, TCSANOW, &options);

        com_fd = fd;

        /* Initialize the file descriptor set. */
        FD_ZERO(&set);
        FD_SET(fd, &set);

        /* Initialize the timeout data structure. */
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        status = STATUS_OK;
        return status;
    }

    RobotStatus CommunicationRobot::RobotCloseCom(void) {
        close(com_fd);

        status = STATUS_OK;
        return status;
    }

    /* command function */
    RobotStatus CommunicationRobot::RobotStart(void) {
        sprintf(buffer_out, "%c\r", ROBOT_CMD_START_WATCHDOG);

        status = (RobotManageCmd(buffer_out, buffer_in));
        return status;
    }

    RobotStatus CommunicationRobot::RobotStop(void) {
        sprintf(buffer_out, "%c\r", ROBOT_CMD_RESET);

        status = (RobotManageCmd(buffer_out, buffer_in));
        return status;
    }

    RobotStatus CommunicationRobot::RobotSetMotors(int motor_left, int motor_right) {
        sprintf(buffer_out, "%c=%d,%d\r", ROBOT_CMD_SET_MOTORS, motor_left, motor_right);

        status = (RobotManageCmd(buffer_out, buffer_in));
        return status;
    }

    RobotStatus CommunicationRobot::RobotReloadWdt(void) {
        sprintf(buffer_out, "%c\r", ROBOT_CMD_RELOAD_WATCHDOG);

        status = (RobotManageCmd(buffer_out, buffer_in));
        return status;
    }

    RobotStatus CommunicationRobot::RobotGetSensor(int *sensor) {
        
        *sensor = -1;

        sprintf(buffer_out, "%c\r", ROBOT_CMD_GET_SENSOR);

        status = RobotManageCmd(buffer_out, buffer_in);

        if (status != STATUS_OK) return status;

        sscanf(buffer_in, "O:%d", sensor);
        printf("Sensor = %d\n", *sensor);

        status = STATUS_OK;
        return status;
    }

    RobotStatus CommunicationRobot::RobotGetOdo(int *odo_left, int *odo_right) {
        *odo_left = -1;
        *odo_right = -1;

        sprintf(buffer_out, "%c\r", ROBOT_CMD_GET_ODO);

        status = RobotManageCmd(buffer_out, buffer_in);

        if (status != STATUS_OK) return status;

        sscanf(buffer_in, "O:%d,%d", odo_left, odo_right);
        printf("Odo_left = %d, Odo_right= %d\n", *odo_left, *odo_right);

        status = STATUS_OK;
        return status;
    }

    RobotStatus CommunicationRobot::RobotGetVBat(int *vbat) {
        *vbat = -1;

        sprintf(buffer_out, "%c\r", ROBOT_CMD_GET_VBAT);

        status = RobotManageCmd(buffer_out, buffer_in);

        if (status != STATUS_OK) return status;

        sscanf(buffer_in, "O:%d", vbat);
        printf("Vbat = %d\n", *vbat);
        status = STATUS_OK;
        return status;
    }
}
