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
    public partial class TerrainPicker : Form
    {
        public TerrainPicker()
        {
            InitializeComponent();
            trackBar2.ValueChanged += new EventHandler(trackBar2_ValueChanged);
            trackBar3.ValueChanged += new EventHandler(trackBar3_ValueChanged);
            trackBar1.ValueChanged += new EventHandler(trackBar1_ValueChanged);
            try
            {
                label6.Text = Lang.main.GetText(37);
                label10.Text = Lang.main.GetText(38);
                Text = Lang.main.GetText(43);
                label4.Text = Lang.main.GetText(44);
            }
            catch { }
        }

        void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            label3.Text = trackBar1.Value + "%";
        }
        void trackBar3_ValueChanged(object sender, EventArgs e)
        {
            label11.Text = trackBar3.Value + "%";
        }

        void trackBar2_ValueChanged(object sender, EventArgs e)
        {
            label7.Text = (trackBar2.Value / 10f).ToString();
        }
    }
}
