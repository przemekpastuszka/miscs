using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace QuintaEssentia
{
    public partial class MainTerrainForm : Form
    {
        public MainTerrainForm()
        {
            InitializeComponent();
            trackBar1.ValueChanged += new EventHandler(trackBar1_ValueChanged);
            try
            {
                Text = Lang.main.GetText(35);
                label1.Text = Lang.main.GetText(34);
            }
            catch { }
        }
        void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            label2.Text = (trackBar1.Value / 20f).ToString();
        }
    }
}
