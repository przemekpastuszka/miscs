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
    public partial class LightForm : Form
    {
        public LightForm()
        {
            InitializeComponent();
            try
            {
                this.label1.Text = Lang.main.GetText(18);
                this.checkBox1.Text = Lang.main.GetText(19);
                this.label5.Text = Lang.main.GetText(20);
                this.label6.Text = Lang.main.GetText(22);
                this.Text = Lang.main.GetText(17);
                checkBox1.CheckedChanged += new EventHandler(checkBox1_CheckedChanged);
            }
            catch { }
        }

        void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
                label6.Enabled=label7.Enabled=trackBar1.Enabled = label8.Enabled = label9.Enabled = false;
            else
                label6.Enabled = label7.Enabled = trackBar1.Enabled = label8.Enabled = label9.Enabled = true;
        }

    }
}
