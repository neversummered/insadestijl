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
#define ROBOT_CMD_START_INSECURELY		'u'
#define ROBOT_CMD_RELOAD_WATCHDOG	    'w'
#define ROBOT_CMD_GET_SENSOR            's'
#define ROBOT_CMD_GET_VBAT              'v'
#define ROBOT_CMD_GET_ODO               'o'
#define ROBOT_CMD_RESET                 'r'
#define ROBOT_CMD_GET_VERSION           'V'

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


	/********* Fonctions privées *********/


	/*
	 * Fonction privée pour usage interne
	 * ne pas utiliser
	 */
    RobotStatus CommunicationRobot::RobotSendChar(char c) {
        write(com_fd, &c, 1);
        status = STATUS_OK;
        return status;
    }

	/*
	 * Fonction privée pour usage interne
	 * ne pas utiliser
	 */
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

	/*
	 * Fonction privée pour usage interne
	 * ne pas utiliser
	 *
	 * Permet d'envoyer une commande et de recevoir la reponse
	 * Gere aussi le re-envoi de commande si elle est perdue (timeout)
	 */
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

	/********* Fonctions publiques *********/


	/*
	 * RobotOpencom
	 * permet d'ouvrir le port de communication (defini dans RobotComFilename)
     * pour permettre la communication avec le robot
	 *
	 * A APPELER EN PRIORITE !!!
	 *
	 * Renvoi STATUS_OK si tout s'est bien passé, STATUS_ERR_NO_FILE si le fichier indiqué
	 * dans RobotComFilename n'existe pas ou ne peut pas etre ouvert.
	 *
	 * Remarque: si le status est STATUS_OK, ca n'indique en rien que le robot est pret a
	 * recevoir des commande (est allumé, à de la batterie ...)
	 * Cela permet juste de pouvoir envoyer des commandes ...
	 *
	 * Entrée:
	 *		Rien
	 * Sortie: 
	 *		STATUS_OK => Sortie sans erreur
	 *		STATUS_ERR_NO_FILE => Le fichier indiqué par RobotComFilename n'existe pas ou ne peut pas etre ouvert
	 */
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

	/*
	 * RobotCloseCom
	 * Ferme le port de communication avec le robot.
	 *
	 * attention ! Cela ne fait aucun "reset" sur le robot !
	 *
	 * Entrée:
	 *		Rien
	 * Sortie: 
	 *		STATUS_OK => Sortie sans erreur
	 */
    RobotStatus CommunicationRobot::RobotCloseCom(void) {
        close(com_fd);

        status = STATUS_OK;
        return status;
    }

    /* 
	 * RobotStart
	 * Demarre le robot, et active le watchdog interne
	 *
	 * Après cette commande, il faut penser à remettre le watchdog à zero toute les 500 ms 
	 *
	 * Le robot passe dans l'etat "ACTIF"
	 *
	 * Entrée:
	 *		Rien
	 * Sortie: 
	 *		STATUS_OK => Sortie sans erreur
	 *		STATUS_ERR_TIMEOUT => Erreur de timeout dans l'envoi de la comamnde (le robot est allumé ?)
     *		STATUS_ERR_UNKNOWN_CMD => commande inconnue (le firmware du robot est a jour ? parasitage de la com ?)
     *		STATUS_ERR_INVALID_PARAMS => Parametre ou commande rejetée (le watchdog a expiré? Le firmware est a jour ? les parametres sont les bons ?)
	 */
    RobotStatus CommunicationRobot::RobotStart(void) {
        sprintf(buffer_out, "%c\r", ROBOT_CMD_START_WATCHDOG);

        status = (RobotManageCmd(buffer_out, buffer_in));
        return status;
    }

	/* 
	 * RobotStartInsecurely
	 * Demarre le robot sans activer le watchdog interne
	 *
	 * Il n'est donc pas necessaire de remettre le watchdog à zero toute les 500 ms par la suite
	 *
	 * Le robot passe dans l'etat "ACTIF"
	 *
	 * Entrée:
	 *		Rien
	 * Sortie: 
	 *		STATUS_OK => Sortie sans erreur
	 *		STATUS_ERR_TIMEOUT => Erreur de timeout dans l'envoi de la comamnde (le robot est allumé ?)
     *		STATUS_ERR_UNKNOWN_CMD => commande inconnue (le firmware du robot est a jour ? parasitage de la com ?)
     *		STATUS_ERR_INVALID_PARAMS => Parametre ou commande rejetée (le watchdog a expiré? Le firmware est a jour ? les parametres sont les bons ?)
	 */
	RobotStatus CommunicationRobot::RobotStartInsecurely(void) {
        sprintf(buffer_out, "%c\r", ROBOT_CMD_START_INSECURELY);

        status = (RobotManageCmd(buffer_out, buffer_in));
        return status;
    }

	/* 
	 * RobotStop
	 * Arrete le robot, le watchdog et stoppe les moteurs
	 *
	 * Le robot passe dans l'etat "ATTENTE"
	 *
	 * Entrée:
	 *		Rien
	 * Sortie: 
	 *		STATUS_OK => Sortie sans erreur
	 *		STATUS_ERR_TIMEOUT => Erreur de timeout dans l'envoi de la comamnde (le robot est allumé ?)
     *		STATUS_ERR_UNKNOWN_CMD => commande inconnue (le firmware du robot est a jour ? parasitage de la com ?)
     *		STATUS_ERR_INVALID_PARAMS => Parametre ou commande rejetée (le watchdog a expiré? Le firmware est a jour ? les parametres sont les bons ?)
	 */
    RobotStatus CommunicationRobot::RobotStop(void) {
        sprintf(buffer_out, "%c\r", ROBOT_CMD_RESET);

        status = (RobotManageCmd(buffer_out, buffer_in));
        return status;
    }

	/* 
	 * RobotSetMotors
	 * Commande les moteurs
	 *
	 * Entrées: les commandes pour le moteur gauche et le moteur droit
	 *		Valeurs pour les commandes de moteur:
	 *		MOTEUR_STOP					0
	 *		MOTEUR_AVANT_LENT			1
	 *		MOTEUR_AVANT_RAPIDE			2
	 *		MOTEUR_ARRIERE_LENT			-1
	 *		MOTEUR_ARRIERE_RAPIDE		-2
	 * 
	 * Sortie: 
	 *		STATUS_OK => Sortie sans erreur
	 *		STATUS_ERR_TIMEOUT => Erreur de timeout dans l'envoi de la comamnde (le robot est allumé ?)
     *		STATUS_ERR_UNKNOWN_CMD => commande inconnue (le firmware du robot est a jour ? parasitage de la com ?)
     *		STATUS_ERR_INVALID_PARAMS => Parametre ou commande rejetée (le watchdog a expiré? Le firmware est a jour ? les parametres sont les bons ?)
	 */
    RobotStatus CommunicationRobot::RobotSetMotors(int motor_left, int motor_right) {
        sprintf(buffer_out, "%c=%d,%d\r", ROBOT_CMD_SET_MOTORS, motor_left, motor_right);

        status = (RobotManageCmd(buffer_out, buffer_in));
        return status;
    }

	/* 
	 * RobotReloadWdt
	 * Remet le watchdog a zero
	 *
	 * Le watchdog doit etre remit à zero entre 450 ms et 550 ms, trop tôt ou trop tard
	 * ce n'est pas bon
	 * Remarque, si le robot a été lancé avec la commande RobotStartInsecurely, cette commande
	 * est sans effet.

	 * Entrées: 
	 *		rien
	 * Sortie: 
	 *		STATUS_OK => Sortie sans erreur
	 *		STATUS_ERR_TIMEOUT => Erreur de timeout dans l'envoi de la comamnde (le robot est allumé ?)
     *		STATUS_ERR_UNKNOWN_CMD => commande inconnue (le firmware du robot est a jour ? parasitage de la com ?)
     *		STATUS_ERR_INVALID_PARAMS => Parametre ou commande rejetée (le watchdog a expiré? Le firmware est a jour ? les parametres sont les bons ?)
	 */
    RobotStatus CommunicationRobot::RobotReloadWdt(void) {
        sprintf(buffer_out, "%c\r", ROBOT_CMD_RELOAD_WATCHDOG);

        status = (RobotManageCmd(buffer_out, buffer_in));
        return status;
    }

	/* 
	 * RobotGetSensor
	 * Retourne l'etat du capteur de balle/palet
	 *
	 * Le watchdog doit etre remit à zero entre 450 ms et 550 ms, trop tôt ou trop tard
	 * ce n'est pas bon
	 * Remarque, si le robot a été lancé avec la commande RobotStartInsecurely, cette commande
	 * est sans effet.

	 * Entrées: 
	 *		*sensor: 
	 *			pointeur sur int pour recuperer l'etat du capteur(le pointeur doit pointer sur
	 *          un espace memoire alloué avant l'appel)
	 * Sortie:
	 *		*sensor:pointeur sur int, l'espace memoire contenant l'etat du capteur
	 *			SENSOR_EMPTY: aucun objet n'est dasn la "bouche" du robot
	 *			SENSOR_FULL:  un objet est present dans la "bouche"
	 *
	 *      retour de fonction:
	 *		STATUS_OK => Sortie sans erreur
	 *		STATUS_ERR_TIMEOUT => Erreur de timeout dans l'envoi de la comamnde (le robot est allumé ?)
     *		STATUS_ERR_UNKNOWN_CMD => commande inconnue (le firmware du robot est a jour ? parasitage de la com ?)
     *		STATUS_ERR_INVALID_PARAMS => Parametre ou commande rejetée (le watchdog a expiré? Le firmware est a jour ? les parametres sont les bons ?)
	 */
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

	/* 
	 * RobotGetOdo
	 * Retourne la distance parcourue par chaque roue 
	 *
	 * Entrées: 
	 *		*odo_left,*odo_right: 
	 *             pointeurs sur int pour recuperer l'etat de l'odometrie 
	 *             (les pointeurs doivent pointer sur des espaces memoires alloués avant l'appel)
	 * Sortie: 
	 *      *odo_left,*odo_right: 
	 *             pointeurs sur int, l'espace memoire pointé contenant les distances parcourues 
	 *             (avant le firmware 1.4, retourne toujours 0)
	 *
	 *      retour de fonction:
	 *		STATUS_OK => Sortie sans erreur
	 *		STATUS_ERR_TIMEOUT => Erreur de timeout dans l'envoi de la comamnde (le robot est allumé ?)
     *		STATUS_ERR_UNKNOWN_CMD => commande inconnue (le firmware du robot est a jour ? parasitage de la com ?)
     *		STATUS_ERR_INVALID_PARAMS => Parametre ou commande rejetée (le watchdog a expiré? Le firmware est a jour ? les parametres sont les bons ?)
	 */
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

	/* 
	 * RobotGetVBat
	 * Lit l'etat de la batterie
	 *
	 * Seul trois niveau de batterie sont remontés: BATTERY_OK, si la batterie est assez chargée
	 * BATTERY_LOW, si le niveau commence a etre faible et BATTERY_STOP, si le niveau est trop faible.
	 * Dans ce dernier cas, le robot passe en mode "BATTERY_OFF" et refuse de commander les moteurs.
	 *
	 * Entrées: 
	 *		*vbat: pointeur sur int pour recuperer l'etat de la batterie (le pointeur doit pointer sur
	 *             un espace memoire alloué avant l'appel)
	 * Sortie: 
	 *      *vbat: pointeur sur int, l'espace memoire contenant l'etat de la batterie
	 *			BATTERY_OK => batterie assez chargé, fonctionnement normal
	 *			BATTERY_LOW => batterie faible, mais fonctionnement normal
	 *			BATTERY_OFF => batterie vide, moteurs arretés
	 *
	 *      retour de fonction:
	 *		STATUS_OK => Sortie sans erreur
	 *		STATUS_ERR_TIMEOUT => Erreur de timeout dans l'envoi de la comamnde (le robot est allumé ?)
     *		STATUS_ERR_UNKNOWN_CMD => commande inconnue (le firmware du robot est a jour ? parasitage de la com ?)
     *		STATUS_ERR_INVALID_PARAMS => Parametre ou commande rejetée (le watchdog a expiré? Le firmware est a jour ? les parametres sont les bons ?)
	 */
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

	/* 
	 * RobotGetVersion
	 * Retourne la version du firmware du robot
	 *
	 * Entrées: 
	 *		*versionMajor, *versionMinor: 
	 *			   pointeurs sur int pour recuperer le numero de version 
	 *            (les pointeurs doivent pointer sur un espace memoire alloué avant l'appel)
	 * Sortie: 
	 *      *versionMajor, *versionMinor:  
	 *			pointeurs sur int, l'espace memoire contenant la version du firmware
	 *
	 *      retour de fonction:
	 *		STATUS_OK => Sortie sans erreur
	 *		STATUS_ERR_TIMEOUT => Erreur de timeout dans l'envoi de la comamnde (le robot est allumé ?)
     *		STATUS_ERR_UNKNOWN_CMD => commande inconnue (le firmware du robot est a jour ? parasitage de la com ?)
     *		STATUS_ERR_INVALID_PARAMS => Parametre ou commande rejetée (le watchdog a expiré? Le firmware est a jour ? les parametres sont les bons ?)
	 */
    RobotStatus CommunicationRobot::RobotGetVersion(int *versionMajor, int *versionMinor) {
        *versionMajor = 0;
		*versionMinor = 0;

        sprintf(buffer_out, "%c\r", ROBOT_CMD_GET_VERSION);

        status = RobotManageCmd(buffer_out, buffer_in);

        if (status != STATUS_OK) return status;

        sscanf(buffer_in, "O:%d,%d", versionMajor, versionMinor);
        printf("Version = %d.%d\n", *versionMajor, *versionMinor);
        status = STATUS_OK;
        return status;
    }
}
