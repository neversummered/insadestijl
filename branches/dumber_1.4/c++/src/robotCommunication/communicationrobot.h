/*
 ============================================================================
 Name        : robot.h
 Author      : Sebastien DI MERCURIO
 Version     :
 Copyright   :
 Description : Interface de la bibliotheque de commande du robot
 ============================================================================
 */

#ifndef ROBOT_H__
#define ROBOT_H__

/*
 * Code de retour des fonctions
 */
#define STATUS_OK 					0
#define STATUS_ERR_NO_FILE			1
#define STATUS_ERR_TIMEOUT			2
#define STATUS_ERR_UNKNOWN_CMD		3
#define STATUS_ERR_INVALID_PARAMS	4
#define STATUS_ERR_WDT_EXPIRED		5
#define STATUS_ERR_SELECT			6	
#define STATUS_ERR_UNKNOWN			0xFF

/*
 * Constantes pour la fonction RobotSetMotors
 *
 * Ces constantes servent à commander les moteurs du robot
 */
#define MOTEUR_STOP					0
#define MOTEUR_AVANT_LENT			1
#define MOTEUR_AVANT_RAPIDE			2
#define MOTEUR_ARRIERE_LENT			-1
#define MOTEUR_ARRIERE_RAPIDE		-2

/*
 * Constantes pour la fonction RobotGetSensor
 */
#define SENSOR_EMPTY				0
#define SENSOR_FULL					1

/*
 * Constantes pour la fonction RobotGetVBat
 */
#define BATTERY_OK					2
#define BATTERY_LOW					1
#define BATTERY_OFF					0

extern char *RobotComFilename;
typedef int RobotStatus;

namespace robotInsa {

    class CommunicationRobot {
    public:

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
        RobotStatus RobotOpenCom(void);

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
        RobotStatus RobotCloseCom(void);

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
        RobotStatus RobotStart(void);

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
		RobotStatus RobotStartInsecurely(void);

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
        RobotStatus RobotStop(void);

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
        RobotStatus RobotSetMotors(int motor_left, int motor_right);

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
        RobotStatus RobotReloadWdt(void);

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
        RobotStatus RobotGetSensor(int *sensor);

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
        RobotStatus RobotGetOdo(int *odo_left, int *odo_right);

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
        RobotStatus RobotGetVBat(int *vbat);

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
		RobotStatus RobotGetVersion(int *versionMajor, int *versionMinor)

        RobotStatus getStatus() {
            return status;
        }

    private:
        RobotStatus status;

		RobotStatus RobotSendChar(char c);
		RobotStatus RobotGetChar(char *c);
        RobotStatus RobotManageCmd(const char *cmd, char *ans);
    };
}

#endif
