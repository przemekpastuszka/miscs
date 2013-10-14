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
    public partial class CameraForm : Form
    {
        public CameraForm()
        {
            InitializeComponent();
            radioButton1.CheckedChanged += new EventHandler(radioButton1_CheckedChanged);
            radioButton2.CheckedChanged+=new EventHandler(radioButton1_CheckedChanged);
            try
            {
                this.label1.Text = Lang.main.GetText(24);
                this.label2.Text = Lang.main.GetText(25);
                this.label3.Text = Lang.main.GetText(26);
                this.radioButton1.Text = Lang.main.GetText(27);
                this.label4.Text = Lang.main.GetText(28);
                this.radioButton2.Text = Lang.main.GetText(29);
                this.label5.Text = Lang.main.GetText(30);
                this.label6.Text = Lang.main.GetText(31);
                this.label10.Text = Lang.main.GetText(32);
                this.Text = Lang.main.GetText(33);
            }
            catch { }
        }

        void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton1.Checked)
                MakeVisible(true);
            else
                MakeVisible(false);
        }
        private void MakeVisible(bool b)
        {
            textBox2.Enabled = b;
            textBox3.Enabled = !b;
        }
    }
}
