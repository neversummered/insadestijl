using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Controleur_Robot
{
    public partial class About : Form
    {
        public About()
        {
            InitializeComponent();
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        public void SetVersionIHM(int majorVersion, int minorVersion)
        {
            labelIHMVersion.Text = majorVersion + "." + minorVersion;
        }

        public void SetVersionFW(int majorVersion, int minorVersion)
        {
            if (majorVersion < 0)
            {
                labelFwVersion.Text = "Inconnue";
            }
            else
            {
                labelFwVersion.Text = majorVersion + "." + minorVersion;
            }
        }
    }
}
