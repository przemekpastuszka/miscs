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
    public partial class SkyForm : Form
    {
        public SkyForm()
        {
            InitializeComponent();
            trackBar1.ValueChanged += new EventHandler(trackBar1_ValueChanged);
            trackBar2.ValueChanged += new EventHandler(trackBar2_ValueChanged);
            trackBar3.ValueChanged += new EventHandler(trackBar3_ValueChanged);
            trackBar6.ValueChanged += new EventHandler(trackBar6_ValueChanged);
            trackBar5.ValueChanged += new EventHandler(trackBar5_ValueChanged);
            trackBar7.ValueChanged += new EventHandler(trackBar7_ValueChanged);
            trackBar4.ValueChanged += new EventHandler(trackBar4_ValueChanged);
            trackBar9.ValueChanged += new EventHandler(trackBar9_ValueChanged);
            trackBar8.ValueChanged += new EventHandler(trackBar8_ValueChanged);
            trackBar11.ValueChanged += new EventHandler(trackBar11_ValueChanged);
            trackBar10.ValueChanged += new EventHandler(trackBar10_ValueChanged);
        }

        void trackBar10_ValueChanged(object sender, EventArgs e)
        {
            label41.Text = trackBar10.Value.ToString();
        }

        void trackBar11_ValueChanged(object sender, EventArgs e)
        {
            label45.Text = ((trackBar11.Value - 1000) / 1000f).ToString();
        }

        void trackBar8_ValueChanged(object sender, EventArgs e)
        {
            label32.Text = (trackBar8.Value / 100f).ToString();
        }

        void trackBar9_ValueChanged(object sender, EventArgs e)
        {
            label36.Text = (trackBar9.Value / 100f).ToString();
        }

        void trackBar4_ValueChanged(object sender, EventArgs e)
        {
            label14.Text = (trackBar4.Value / 10000f).ToString();
        }

        void trackBar7_ValueChanged(object sender, EventArgs e)
        {
            label27.Text = (trackBar7.Value / 10000f).ToString();
        }

        void trackBar5_ValueChanged(object sender, EventArgs e)
        {
            label18.Text = ((trackBar5.Value - 500) / 100f).ToString();
        }

        void trackBar6_ValueChanged(object sender, EventArgs e)
        {
            label22.Text = ((trackBar6.Value-500) / 100f).ToString();
        }

        void trackBar3_ValueChanged(object sender, EventArgs e)
        {
            label10.Text = (trackBar3.Value / 1000f).ToString();
        }

        void trackBar2_ValueChanged(object sender, EventArgs e)
        {
            label3.Text = (trackBar2.Value / 1000f).ToString();
        }

        void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            label7.Text = (trackBar1.Value / 1000f).ToString();
        }

    }
}
