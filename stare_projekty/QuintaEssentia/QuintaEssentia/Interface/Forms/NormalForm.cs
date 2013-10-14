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
    public partial class NormalForm : Form
    {
        public NormalForm()
        {
            InitializeComponent();
            trackBar1.ValueChanged += new EventHandler(trackBar1_ValueChanged);
        }

        void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            label3.Text = (trackBar1.Value / 10f).ToString();
        }
    }
}
