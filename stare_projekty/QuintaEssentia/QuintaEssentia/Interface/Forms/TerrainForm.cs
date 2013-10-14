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
    public partial class TerrainForm : Form
    {
        public TerrainForm()
        {
            InitializeComponent();
            
            
            trackBar1.ValueChanged += new EventHandler(trackBar1_ValueChanged);
            radioButton1.Checked = true;
            try
            {
                Text = Lang.main.GetText(36);
                label5.Text = Lang.main.GetText(35) + " " + Lang.main.GetText(36);
                
                label1.Text = Lang.main.GetText(39);
                radioButton1.Text = Lang.main.GetText(40);
                radioButton2.Text = Lang.main.GetText(41);
                label2.Text = Lang.main.GetText(42);
            }
            catch { }
        }

        void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            label3.Text = (trackBar1.Value / 5f).ToString();
        }
    }
}
