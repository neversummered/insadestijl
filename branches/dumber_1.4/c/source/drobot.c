/**
 * \file DRobot.c
 * \brief Source de la classe DRobot
 * \author P.-E. Hladik
 * \version 0.1
 * \date 28 novembre 2011
 *
 * Implémentation de la classe DRobot
 *
 */

#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>

#include "../headers/drobot.h"

#define MAX_RETRY 3
#define MAX_TIMEOUT 10 /* Exprimé en multiple de 5 ms */

/* Command list */
#define ROBOT_CMD_PING                  'p'
#define ROBOT_CMD_SET_MOTORS            'm'
#define ROBOT_CMD_START_WATCHDOG        'W'
#define ROBOT_CMD_START_INSECURELY      'u'
#define ROBOT_CMD_RELOAD_WATCHDOG       'w'
#define ROBOT_CMD_GET_SENSOR            's'
#define ROBOT_CMD_GET_VBAT              'v'
#define ROBOT_CMD_GET_ODO               'o'
#define ROBOT_CMD_RESET                 'r'
#define ROBOT_CMD_GET_VERSION           'V'
#define ROBOT_CMD_MOVE                  'M'
#define ROBOT_CMD_TURN                  'T'
#define ROBOT_CMD_IS_BUSY               'b'

int i;
char c;
int com_fd;

int verbose = 0;

/* Variable pour select */
fd_set set;
struct timeval timeout;

char buffer_out[32];
char buffer_in[32];
char *ptr_in, *ptr_out;

char *RobotComFilename = "/dev/ttyUSB0";


static void d_robot_init(DRobot *This);
RobotStatus d_robot_send_char(struct DRobot * This, char c);
RobotStatus d_robot_get_char(struct DRobot * This, char *c);
RobotStatus d_robot_manage_cmd(struct DRobot * This, const char *cmd, char *ans);

static void d_robot_init(DRobot *This) {
    This->close_com = d_robot_close_com;
    This->get_odo = d_robot_get_odo;
    This->get_sensor = d_robot_get_sensor;
    This->get_status = d_robot_get_status;
    This->get_vbat = d_robot_get_vbat;
    This->get_version = d_robot_get_version;
    This->is_busy = d_robot_is_busy;
    This->move = d_robot_move;
    This->open_com = d_robot_open_com;
    This->print = d_robot_print;
    This->reload_wdt = d_robot_reload_wdt;
    This->set_motors = d_robot_set_motors;
    This->start = d_robot_start;
    This->start_insecurely = d_robot_start_insecurely;
    This->status = STATUS_ERR_UNKNOWN;
    This->stop = d_robot_stop;
    This->turn = d_robot_turn;
}

DRobot* d_new_robot(void) {
    DRobot *This = malloc(sizeof (DRobot));
    if (This == NULL) {
        printf("\nAttempt to allocate DRobot failed.\n");
        exit(1);
    }
    if (!This) return NULL;
    d_robot_init(This);
    This->free = d_robot_free;
    return This;
}

void d_robot_free(DRobot *This){
    free(This);
}

void d_robot_print(DRobot *This) {
    printf("robot{status:%i}\n", This->status);
}

RobotStatus d_robot_send_char(struct DRobot * This, char c) {
    write(com_fd, &c, 1);
    This->status = STATUS_OK;
    return This->status;
}

RobotStatus d_robot_get_char(struct DRobot * This, char *c) {
    int n;
    int delay;

    This->status = STATUS_OK;
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
    return This->status;
}

RobotStatus d_robot_manage_cmd(struct DRobot * This, const char *cmd, char *ans) {
    RobotStatus status_get;
    int nb_retry;
    char *p_ans;
    char c;
    int cmd_succeed;

    nb_retry = 0;
    This->status = STATUS_ERR_UNKNOWN_CMD;
    status_get = STATUS_OK;
    cmd_succeed = 0;

    while ((nb_retry < MAX_RETRY) && (cmd_succeed == 0)) {
        p_ans = ans;
        *p_ans = 0;
        c = 0;
        /* Envoi de la commande */
        if (verbose) printf("Envoi cmd: %s\n", cmd);
        write(com_fd, cmd, strlen(cmd));
        while ((c != '\n') && (c != '\r') && (status_get != STATUS_ERR_TIMEOUT)) {
            status_get = d_robot_get_char(This, &c);
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
        if (verbose) printf("Timeout!!\n");
        This->status = STATUS_ERR_TIMEOUT;
    }
    if (verbose) printf("Recu ans: %s\n", ans);
    /* Analyse de la reponse */
    switch (*ans) {
        case 'C':
        case 'c':
            if (verbose) printf("Unknown CMD\n");
            This->status = STATUS_ERR_UNKNOWN_CMD;
            break;
        case 'E':
        case 'e':
            if (verbose) printf("Invalid params\n");
            This->status = STATUS_ERR_INVALID_PARAMS;
            break;
        case 'O':
        case 'o':
            if (verbose) printf("Status OK!\n");
            This->status = STATUS_OK;
            break;
        default:
            break;
    }
    return This->status;
}

RobotStatus d_robot_open_com(struct DRobot * This) {
    struct termios options;

    int fd; /* File descriptor for the port */
    fd = open(RobotComFilename, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        /* Could not open the port. */
        fprintf(stderr, "[Error %d] RobotOpenCom: Can't open device file %s\n", STATUS_ERR_NO_FILE, RobotComFilename);
        return STATUS_ERR_NO_FILE;
    } else {
        if (verbose) printf("Opened %s successfully\n", RobotComFilename);
    }
    fcntl(fd, F_SETFL, FNDELAY);
    /* Get the current options for the port...  */
    tcgetattr(fd, &options);
    /*Set the baud rates to 9600 bauds 8N1, no hardware flow control... */
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~CRTSCTS;
    /* Enable the receiver and set local mode...  */
    options.c_cflag |= (CLOCAL | CREAD);
    /* Set the new options for the port... */
    tcsetattr(fd, TCSANOW, &options);
    com_fd = fd;
    /* Initialize the file descriptor set. */
    FD_ZERO(&set);
    FD_SET(fd, &set);
    /* Initialize the timeout data structure. */
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    This->status = STATUS_OK;
    return This->status;
}

RobotStatus d_robot_close_com(struct DRobot * This) {
    close(com_fd);
    This->status = STATUS_OK;
    return This->status;
}

RobotStatus d_robot_start(struct DRobot * This) {
    sprintf(buffer_out, "%c\r", ROBOT_CMD_START_WATCHDOG);
    This->status = (d_robot_manage_cmd(This, buffer_out, buffer_in));
    return This->status;
}

RobotStatus d_robot_start_insecurely(struct DRobot * This) {
    sprintf(buffer_out, "%c\r", ROBOT_CMD_START_INSECURELY);
    This->status = (d_robot_manage_cmd(This, buffer_out, buffer_in));
    return This->status;
}

RobotStatus d_robot_stop(struct DRobot * This) {
    sprintf(buffer_out, "%c\r", ROBOT_CMD_RESET);
    This->status = (d_robot_manage_cmd(This, buffer_out, buffer_in));
    return This->status;
}

RobotStatus d_robot_set_motors(struct DRobot * This, int motor_left, int motor_right) {
    sprintf(buffer_out, "%c=%d,%d\r", ROBOT_CMD_SET_MOTORS, motor_left, motor_right);
    This->status = (d_robot_manage_cmd(This, buffer_out, buffer_in));
    return This->status;
}

RobotStatus d_robot_reload_wdt(struct DRobot * This) {
    sprintf(buffer_out, "%c\r", ROBOT_CMD_RELOAD_WATCHDOG);
    This->status = (d_robot_manage_cmd(This, buffer_out, buffer_in));
    return This->status;
}

RobotStatus d_robot_get_sensor(struct DRobot * This, int *sensor) {
    *sensor = -1;
    sprintf(buffer_out, "%c\r", ROBOT_CMD_GET_SENSOR);
    This->status = d_robot_manage_cmd(This, buffer_out, buffer_in);
    if (This->status != STATUS_OK) return This->status;
    sscanf(buffer_in, "O:%d", sensor);
    if (verbose) printf("Sensor = %d\n", *sensor);
    This->status = STATUS_OK;
    return This->status;
}

RobotStatus d_robot_get_odo(struct DRobot * This, int *odo_left, int *odo_right) {
    *odo_left = -1;
    *odo_right = -1;
    sprintf(buffer_out, "%c\r", ROBOT_CMD_GET_ODO);
    This->status = d_robot_manage_cmd(This, buffer_out, buffer_in);
    if (This->status != STATUS_OK) return This->status;
    sscanf(buffer_in, "O:%d,%d", odo_left, odo_right);
    if (verbose) printf("Odo_left = %d, Odo_right= %d\n", *odo_left, *odo_right);
    This->status = STATUS_OK;
    return This->status;
}

RobotStatus d_robot_get_vbat(struct DRobot * This, int *vbat) {
    *vbat = -1;
    sprintf(buffer_out, "%c\r", ROBOT_CMD_GET_VBAT);
    This->status = d_robot_manage_cmd(This, buffer_out, buffer_in);
    if (This->status != STATUS_OK) return This->status;
    sscanf(buffer_in, "O:%d", vbat);
    if (verbose) printf("Vbat = %d\n", *vbat);
    This->status = STATUS_OK;
    return This->status;
}

RobotStatus d_robot_get_version(struct DRobot * This, int *versionMajor, int *versionMinor) {
    *versionMajor = 0;
    *versionMinor = 0;
    sprintf(buffer_out, "%c\r", ROBOT_CMD_GET_VERSION);
    This->status = d_robot_manage_cmd(This, buffer_out, buffer_in);
    if (This->status != STATUS_OK) return This->status;
    sscanf(buffer_in, "O:%d,%d", versionMajor, versionMinor);
    if (verbose) printf("Version = %d.%d\n", *versionMajor, *versionMinor);
    This->status = STATUS_OK;
    return This->status;
}

RobotStatus d_robot_get_status(struct DRobot * This) {
    return This->status;
}

RobotStatus d_robot_move(struct DRobot * This, int range) {
int nb_tour_moteur;

	/* 
	 * Rapport de réduction des roues 1/87
	 * On a deux point par tour du rotor, donc 174 par tour de roue
	 * Rayon des roues = 35mm
	 * Circonférence = 220 mm
	 */
	
	nb_tour_moteur = (range * 174) / 220;
    sprintf(buffer_out, "%c=%d\r", ROBOT_CMD_MOVE, nb_tour_moteur);
    This->status = (d_robot_manage_cmd(This, buffer_out, buffer_in));
    return This->status;
}

RobotStatus d_robot_turn(struct DRobot * This, int angle, int direction) {
int nb_tour_moteur;

	/* Rapport de réduction des roues 1/87
	 * On a deux point par tour du rotor, donc 174 par tour de roue
	 * Rayon des roues = 35mm
	 * Circonférence = 22cm
	 * 
	 * Empatement du robot: 100 mm
	 * Distance de roue à parcourir pour un tour complet (360°) : 314 mm
	 * nb de tour moteur pour un tour complet (360°) : 248 tours
	 */
	 
	if (angle <0) angle = -angle;
	
	if (direction == HORAIRE)
	{
		/* Conversion de l'angle en nombre de tour moteur, pour une rotation horaire */
		nb_tour_moteur = (angle * 248) / 360;
	}
	else
	{
		/* Conversion de l'angle en nombre de tour moteur, pour une rotation anti-horaire */
		nb_tour_moteur = (angle * 248) / 360;
		nb_tour_moteur = -nb_tour_moteur;
	}
	
    sprintf(buffer_out, "%c=%d,%d\r", ROBOT_CMD_TURN, angle, direction);
    This->status = (d_robot_manage_cmd(This, buffer_out, buffer_in));
    return This->status;
}

RobotStatus d_robot_is_busy(DRobot* This, int *busy){
    *busy = 0;
    sprintf(buffer_out, "%c\r", ROBOT_CMD_IS_BUSY);
    This->status = d_robot_manage_cmd(This, buffer_out, buffer_in);
    if (This->status != STATUS_OK) return This->status;
    sscanf(buffer_in, "O:%d", busy);
    if (verbose) printf("Busy = %d\n", *busy);
    This->status = STATUS_OK;
    return This->status;
}