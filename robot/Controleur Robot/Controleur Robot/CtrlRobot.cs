using System;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;

namespace Controleur_Robot
{
    public class CtrlRobot: SerialPort 
    {
        private const string CMD_PING="p";
        private const string CMD_SET_MOTORS="m";
        private const string CMD_START_WATCHDOG="W";
        private const string CMD_RELOAD_WATCHDOG="w";
        private const string CMD_GET_SENSOR="s";
        private const string CMD_GET_VBAT="v";
        private const string CMD_GET_ODO="o";
        private const string CMD_RESET="r";

        private const int MAX_RETRY = 3;

        private const char ANS_OK = 'o';
        private const char ANS_OK_UPPERCASE = 'O';
        private const char ANS_UNKNOWN_CMD='c';
        private const char ANS_UNKNOWN_CMD_UPPERCASE='C';
        private const char ANS_INVALID_PARAMS='e';
        private const char ANS_INVALID_PARAMS_UPPERCASE='E';

        public enum MOTORS
        {
            Arret=0,
            AvantNormal=1,
            AvantRapide=2,
            ArriereNormal=-1,
            ArriereRapide=-2
        };

        public enum VBAT
        {
            Normal=2,
            Faible=1,
            Vide=0,
            Inconnu=-1
        };

        public enum SENSOR
        {
            Inconnu=-1,
            Vide=0,
            Present=1
        };

        new public void Open()
        {
            Open(SerialPort.GetPortNames()[0]);
        }

        public void Open(string PortName)
        {
            base.NewLine = "\n";
            base.ReadBufferSize = 100;
            base.WriteBufferSize = 100;
            base.ReadTimeout = 50;
            base.WriteTimeout = SerialPort.InfiniteTimeout;

            base.BaudRate = 9600;
            base.DataBits = 8;
            base.Parity = Parity.None;
            base.PortName = PortName;

            base.Open();

            // Test si un robot est bien present (commande ping)
            Ping();
        }

        /*
         * SendCommand
         * Envoi une commande au robot
         * Forme 1 : Retourne la reponse
         */
        private void SendCommand(string command, out string answer)
        {
	        int nb_retry=0;
            bool cmd_succeed=false;
            string ans;

            ans = "";

            do
            {
                cmd_succeed = true;

                /* Envoi de la commande */
                Console.WriteLine("Envoi de la commande: " + command);
                base.WriteLine(command);

                /* Recuperation de la reponse */
                try
                {
                    ans = base.ReadLine();
                }
                catch (TimeoutException)
                {
                    /* Probleme de timeout */
                    Console.WriteLine("[ERR] Timeout lors de la reponse à la commande " + command);
                    nb_retry++;
                    cmd_succeed = false;
                }
            }
            while ((nb_retry < MAX_RETRY) && (cmd_succeed == false));

	        if (nb_retry >= MAX_RETRY)
	        {
		        throw new System.TimeoutException("La commande a dépassé son nombre maximal de re-essais");
	        }

            Console.WriteLine("Reponse recue: " + ans);

	        /* Analyse de la reponse */
            switch (ans[0])
	        {
	        case ANS_UNKNOWN_CMD:
	        case ANS_UNKNOWN_CMD_UPPERCASE:
		        Console.WriteLine ("[ERR] Attention: commande inconnue ou non reconnue");
		        //throw new System.NotImplementedException("La commande " + command + " n'est pas supportée");
		        break;
	        case ANS_INVALID_PARAMS:
	        case ANS_INVALID_PARAMS_UPPERCASE:
		        Console.WriteLine ("[ERR] Attention: parametres invalides ou non reconnus");
		        //throw new System.ArgumentException("Les parametres de la commande " + command + " ne sont pas valides");
		        break;
	        case ANS_OK:
	        case ANS_OK_UPPERCASE:
		        Console.WriteLine ("OK");
		        break;
	        default:
		        break;
	        }

            answer = ans;
        }

        /*
         * SendCommand
         * Envoi une commande au robot
         * Forme 2 : Ne retourne pas la reponse
         */
        private void SendCommand(string command)
        {
	        int nb_retry=0;
            bool cmd_succeed=true;

            string answer="";

	        do
	        {
                cmd_succeed = true;

		        /* Envoi de la commande */
                Console.WriteLine("Envoi de la commande: " + command);
                base.WriteLine(command);
                
                /* Recuperation de la reponse */
                try
                {
                    answer = base.ReadLine();
                }
                catch (TimeoutException)
                {
                    /* Probleme de timeout */
                    Console.WriteLine("[ERR] Timeout lors de la reponse à la commande " + command); 
                    nb_retry ++;
                    cmd_succeed = false;
                }
	        }
            while ((nb_retry < MAX_RETRY) && (cmd_succeed == false)) ;

	        if (nb_retry >= MAX_RETRY)
	        {
		        throw new System.TimeoutException("La commande a dépassé son nombre maximal de re-essais");
	        }

	        Console.WriteLine ("reponse recue: " + answer);

	        /* Analyse de la reponse */
	        switch (answer[0])
	        {
	        case ANS_UNKNOWN_CMD:
	        case ANS_UNKNOWN_CMD_UPPERCASE:
		        Console.WriteLine ("[ERR] Attention: commande inconnue ou non reconnue");
		        //throw new System.NotImplementedException("La commande " + command + " n'est pas supportée");
		        break;
	        case ANS_INVALID_PARAMS:
	        case ANS_INVALID_PARAMS_UPPERCASE:
		        Console.WriteLine ("[ERR] Attention: parametres invalides ou non reconnus");
		        //throw new System.ArgumentException("Les parametres de la commande " + command + " ne sont pas valides");
		        break;
	        case ANS_OK:
	        case ANS_OK_UPPERCASE:
		        Console.WriteLine ("OK");
		        break;
	        default:
		        break;
	        }
        }

        /*
         * Commande PING
         * Teste la presence du robot
         */
        public void Ping()
        {
            SendCommand(CMD_PING);  
        }

        /*
         * Commande START_WATCHDOG
         * Demarre le watchdog du robot (et fait clignoter la led)
         */
        public void Start()
        {
	        SendCommand(CMD_START_WATCHDOG);
        }

        /*
         * Commande RESET
         * Arrete le watchdog du robot (et allume la led en continu)
         */
        public void Stop()
        {
            SendCommand(CMD_RESET);
        }

        /*
         * Commande SET_MOTORS
         * Commande les moteurs
         */
        public void SetMotors(MOTORS moteurGauche, MOTORS moteurDroit)
        {
            int valMoteurGauche, valMoteurDroit;

            valMoteurGauche = 0;
            valMoteurDroit = 0;

            switch (moteurGauche)
            {
                case MOTORS.Arret:
                    valMoteurGauche = 0;
                    break;
                case MOTORS.ArriereNormal:
                    valMoteurGauche = -1;
                    break;
                case MOTORS.ArriereRapide:
                    valMoteurGauche = -2;
                    break;
                case MOTORS.AvantNormal:
                    valMoteurGauche = 1;
                    break;
                case MOTORS.AvantRapide:
                    valMoteurGauche = 2;
                    break;
            }

            switch (moteurDroit)
            {
                case MOTORS.Arret:
                    valMoteurDroit = 0;
                    break;
                case MOTORS.ArriereNormal:
                    valMoteurDroit = -1;
                    break;
                case MOTORS.ArriereRapide:
                    valMoteurDroit = -2;
                    break;
                case MOTORS.AvantNormal:
                    valMoteurDroit = 1;
                    break;
                case MOTORS.AvantRapide:
                    valMoteurDroit = 2;
                    break;
            }

            SendCommand(CMD_SET_MOTORS + "=" + valMoteurGauche + "," + valMoteurDroit);
        }

        /*
         * Commande CMD_RELOAD_WATCHDOG
         * Relacne le Watchdog
         */
        public void ReloadWdt()
        {
            SendCommand(CMD_RELOAD_WATCHDOG);
        }

        /*
         * Commande CMD_GET_SENSOR
         * Recupere l'etat du capteur
         */
        public SENSOR GetSensor()
        {
            SENSOR etat;
            string answer;
            string[] items;

            int valeur;

            SendCommand(CMD_GET_SENSOR, out answer);

            items = answer.Split(':');

            try
            {
                valeur = Convert.ToInt32(items[1]);
            }
            catch (Exception e)
            {
                Console.WriteLine("[ERR] Probleme lors de l'analyse de la reponse " + answer + "\r\nException: " + e);
                valeur =-1;
            }

            switch (valeur)
            {
                case 0:
                    etat = SENSOR.Vide;
                    break;
                case 1:
                    etat = SENSOR.Present;
                    break;
                case -1:
                    etat = SENSOR.Inconnu;
                    break;
                default:
                    //throw new System.ArgumentException("Reponse inconnue: " + answer);
                    Console.WriteLine("[ERR] Attention: reponse inconnue (" + answer + ")");
                    etat = SENSOR.Inconnu;
                    break;
            }

            return etat;
        }

        /*
         * Commande CMD_GET_ODO
         * Recupere l'odometrie du robot
         */
        public void GetOdometrie(out int odometrieGauche, out int odometrieDroite)
        {
            string answer;
            string[] items;

            SendCommand(CMD_GET_ODO, out answer);

            items = answer.Split(new Char[] { ':', ','});

            try
            {
                odometrieGauche = Convert.ToInt32(items[1]);
                odometrieDroite = Convert.ToInt32(items[2]);
            }
            catch (Exception e)
            {
                Console.WriteLine("[ERR] Probleme lors de l'analyse de la reponse " + answer + "\r\nException: " + e);
                odometrieGauche = 0;
                odometrieDroite = 0;
            }
        }

        /*
         * Commande CMD_GET_VBAT
         * Recupere l'etat de la batterie
         */
        public VBAT GetVBat()
        {
            string answer;
            string[] items;
            int valeur;
            VBAT etat;

            SendCommand(CMD_GET_VBAT, out answer);

            items = answer.Split(':');

            try
            {
                valeur = Convert.ToInt32(items[1]);
            }
            catch (Exception e)
            {
                Console.WriteLine("[ERR] Probleme lors de l'analyse de la reponse " + answer + "\r\nException: " + e);
                valeur = -1;
            }

            switch (valeur)
            {
                case 0:
                    etat = VBAT.Vide;
                    break;
                case 1:
                    etat = VBAT.Faible;
                    break;
                case 2:
                    etat = VBAT.Normal;
                    break;
                case -1:
                    etat = VBAT.Inconnu;
                    break;
                default:
                    //throw new System.ArgumentException("Reponse inconnue: " + answer);
                    Console.WriteLine("[ERR] Attention: Reponse inconnue (" + answer + ")");
                    etat = VBAT.Inconnu;
                    break;
            }

            return etat;
        }
    }
}
