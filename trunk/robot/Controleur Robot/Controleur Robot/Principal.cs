using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace Controleur_Robot
{
    public partial class Principal : Form
    {
        CtrlRobot ctrlRobot;
        Log log;

        public Principal()
        {
            InitializeComponent();

            log = new Log();

            ctrlRobot = new CtrlRobot();

            DesactivationElements();
        }

        private void DesactivationElements()
        {
            groupBox2.Enabled = false;
            groupBox3.Enabled = false;
            groupBox4.Enabled = false;
            groupBox5.Enabled = false;
            groupBox6.Enabled = false;
        }

        private void ActivationElements()
        {
            groupBox2.Enabled = true;
            groupBox3.Enabled = true;
            groupBox4.Enabled = true;
            groupBox5.Enabled = true;
            groupBox6.Enabled = true;
        }

        private void buttonDeplacement_Click(object sender, EventArgs e)
        {
            int vitesse;

            vitesse = trackBarVitesse.Value;

            if (sender == buttonArret)
            {
                labelCommandeCourante.Text = "Arret";
                ctrlRobot.SetMotors(CtrlRobot.MOTORS.Arret, CtrlRobot.MOTORS.Arret);
            }
            else if (sender == buttonArriere)
            {
                if (vitesse == 1) /* vitesse normale */
                {
                    labelCommandeCourante.Text = "Arriere normal";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.ArriereNormal, 
                                        CtrlRobot.MOTORS.ArriereNormal);
                }
                else /* vitesse rapide */
                {
                    labelCommandeCourante.Text = "Arriere rapide";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.ArriereRapide,
                                        CtrlRobot.MOTORS.ArriereRapide);
                }
            }
            else if (sender == buttonAvant)
            {
                if (vitesse == 1) /* vitesse normale */
                {
                    labelCommandeCourante.Text = "Avant normal";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.AvantNormal,
                                        CtrlRobot.MOTORS.AvantNormal);
                }
                else /* vitesse rapide */
                {
                    labelCommandeCourante.Text = "Avant rapide";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.AvantRapide,
                                        CtrlRobot.MOTORS.AvantRapide);
                }
            }
            else if (sender == buttonDroite)
            {
                if (vitesse == 1) /* vitesse normale */
                {
                    labelCommandeCourante.Text = "Droite normal";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.AvantNormal,
                                        CtrlRobot.MOTORS.Arret);
                }
                else /* vitesse rapide */
                {
                    labelCommandeCourante.Text = "Droite rapide";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.AvantRapide,
                                        CtrlRobot.MOTORS.Arret);
                }
            }
            else /* Forcement, c'est le gauche ! */
            {
                if (vitesse == 1) /* vitesse normale */
                {
                    labelCommandeCourante.Text = "Gauche normal";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.Arret,
                                        CtrlRobot.MOTORS.AvantNormal);
                }
                else /* vitesse rapide */
                {
                    labelCommandeCourante.Text = "Gauche rapide";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.Arret,
                                        CtrlRobot.MOTORS.AvantRapide);
                }
            }
        }

        private void buttonEnvoyer_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Ne soit pas aussi impatient, petit scarabée !", "Fonction non implementée",
                 MessageBoxButtons.OK, MessageBoxIcon.Asterisk);  
        }

        private void buttonEnregistrer_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Ne soit pas aussi impatient, petit scarabée !", "Fonction non implementée",
                 MessageBoxButtons.OK, MessageBoxIcon.Asterisk);  
        }

        private void trackBarMoteurs_Scroll(object sender, EventArgs e)
        {
            if (sender == trackBarDroitNormal)
                    labelDroitNormal.Text = Convert.ToString(trackBarDroitNormal.Value);
            else if (sender == trackBarDroitRapide)
                    labelDroitRapid.Text = Convert.ToString(trackBarDroitRapide.Value);
            else if (sender == trackBarGaucheNormal)
                    labelGaucheNormal.Text = Convert.ToString(trackBarGaucheNormal.Value);
            else /* forcement c'est trackBarGaucheRapide */
                    labelGaucheRapide.Text = Convert.ToString(trackBarGaucheRapide.Value);
        }

        private void buttonLed_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Ne soit pas aussi impatient, petit scarabée !", "Fonction non implementée",
                 MessageBoxButtons.OK, MessageBoxIcon.Asterisk);  
        }

        private void buttonConnecter_Click(object sender, EventArgs e)
        {
            string portname;
            string[] portlist;

            if (ctrlRobot.IsOpen == false)
            {
                portlist = SerialPort.GetPortNames();
                portname = portlist[portlist.Length - 1];

                /* Ouverture du port et test de la presence du robot */
                try
                {
                    ctrlRobot.Open(portname);
                }
                catch (TimeoutException)
                {
                    MessageBox.Show("Aucun robot sur le port " + portname,
                                    "Erreur port serie", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                    return;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Erreur lors de l'ouverture du port " + portname + "\r\nException: " + ex,
                                   "Erreur port serie", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                    return;
                }

                /* Si tout s'est bien passé */
                ActivationElements();

                labelConnecter.Text = "connecté (" + portname + ")";
                buttonConnecter.Text = "Deconnecter";
                timerCommande.Enabled = true;
            }
            else
            {
                timerCommande.Enabled = false;
                ctrlRobot.Close();

                DesactivationElements();

                labelConnecter.Text = "non connecté";
                buttonConnecter.Text = "Connecter";
            }
        }

        private void parametresToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Ne soit pas aussi impatient, petit scarabée !", "Fonction non implementée",
                 MessageBoxButtons.OK, MessageBoxIcon.Asterisk);  
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            /*MessageBox.Show("Ne soit pas aussi impatient, petit scarabée !", "Fonction non implementée",
                 MessageBoxButtons.OK, MessageBoxIcon.Asterisk);  */
            About about = new About();

            about.Show();
        }

        private void timerCommande_Tick(object sender, EventArgs e)
        {
            CtrlRobot.VBAT vbat;
            CtrlRobot.SENSOR sensor;

            int odometrieGauche, odometrieDroite;

            /* Recuperation de l'etat de la batterie */
            vbat = ctrlRobot.GetVBat();

            switch (vbat)
            {
                case CtrlRobot.VBAT.Inconnu:
                    labelBatterie.Text = "Inconnu";
                    break;
                case CtrlRobot.VBAT.Vide:
                    labelBatterie.Text = "Vide";
                    break;
                case CtrlRobot.VBAT.Faible:
                    labelBatterie.Text = "Faible";
                    break;
                case CtrlRobot.VBAT.Normal:
                    labelBatterie.Text = "Normale";
                    break;
            }

            /* Recuperation de l'etat du capteur */
            sensor = ctrlRobot.GetSensor();

            switch (sensor)
            {
                case CtrlRobot.SENSOR.Inconnu:
                    labelCapteur.Text = "Inconnu";
                    break;
                case CtrlRobot.SENSOR.Vide:
                    labelCapteur.Text = "Vide";
                    break;
                case CtrlRobot.SENSOR.Present:
                    labelCapteur.Text = "Present";
                    break;
            }

            /* Recuperation de l'odometrie*/
            ctrlRobot.GetOdometrie(out odometrieGauche, out odometrieDroite);

            labelOdoGauche.Text = Convert.ToString(odometrieGauche);
            labelOdoDroit.Text = Convert.ToString(odometrieDroite);
        }

        private void timerWatchdog_Tick(object sender, EventArgs e)
        {

        }

        private void logToolStripMenuItem_Click(object sender, EventArgs e)
        {
            log.Show();
        }
    }
}
