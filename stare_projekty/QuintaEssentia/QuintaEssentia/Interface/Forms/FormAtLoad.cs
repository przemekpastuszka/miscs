using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace QuintaEssentia
{
    public partial class FormAtLoad : Form
    {
        public String[] data=new String[3];
        public FormAtLoad()
        {
            InitializeComponent();
            MakeVisible(true);
            radioButton1.Click += new EventHandler(radioButton1CheckedChanged);
            radioButton2.Click += new EventHandler(radioButton2_CheckedChanged);
            radioButton3.Click += new EventHandler(radioButton3_CheckedChanged);
            button1.Click += new EventHandler(button1_Click);
            button2.Click += new EventHandler(button2_Click);
            try
            {
                this.groupBox1.Text = Lang.main.GetText(2);
                this.groupBox2.Text = Lang.main.GetText(3);
                this.radioButton1.Text = Lang.main.GetText(4);
                this.radioButton2.Text = Lang.main.GetText(5);
                this.button1.Text = Lang.main.GetText(6);
                this.label1.Text = Lang.main.GetText(7);
                this.label2.Text = Lang.main.GetText(8);
                this.label3.Text = Lang.main.GetText(9);
                this.radioButton3.Text = Lang.main.GetText(3);
                this.Text = Lang.main.GetText(10);
            }
            catch { }
        }
        void button2_Click(object sender, EventArgs e)
        {
            if (radioButton3.Checked)
            {
                int temp;
                try
                {
                    temp = Convert.ToInt32(textBox3.Text);
                }
                catch
                {
                    try { MessageBox.Show(Lang.main.GetText(14)); }
                    catch { MessageBox.Show("Size must be a positive integer"); }
                    return;
                }
                if (temp <= 0)
                {
                    try { MessageBox.Show(Lang.main.GetText(14)); }
                    catch { MessageBox.Show("Size must be a positive integer"); }
                    return;
                }
                if ((temp & (temp-1))!=0)
                {
                    try { MessageBox.Show(Lang.main.GetText(15)); }
                    catch { MessageBox.Show("Size of terrain must be a power of 2!"); }
                    return;
                }
            }
            else
            {
                if (!File.Exists(textBox1.Text))
                {
                    try { MessageBox.Show(Lang.main.GetText(13)); }
                    catch { MessageBox.Show("Chosen file not found"); }
                    return;
                }
            }
            data[0] = textBox1.Text;
            data[1] = textBox2.Text;
            data[2] = textBox3.Text;
            
            Close();
        }
        void button1_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog opener = new OpenFileDialog())
            {
                opener.InitialDirectory = Path.GetDirectoryName(Application.ExecutablePath)+"\\Resources\\Levels";
                try { opener.Title = Lang.main.GetText(11); }
                catch { opener.Title = "Choose level"; }
                opener.Filter = Lang.main.GetText(12) + " (*.lvl)|*.lvl";
                if (opener.ShowDialog() == DialogResult.OK)
                    textBox1.Text = opener.FileName;
            }
        }

        void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            radioButton1.Checked = radioButton2.Checked = false;
            MakeVisible(false);
        }
        void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            radioButton1.Checked = radioButton3.Checked = false;
            MakeVisible(true);
        }
        void radioButton1CheckedChanged(object sender, EventArgs e)
        {
            radioButton2.Checked = radioButton3.Checked = false;
            MakeVisible(true);
        }
        void MakeVisible(bool value)
        {
            textBox1.Enabled = value;
            button1.Enabled = value;
            label1.Enabled = value;
            textBox2.Enabled = !value;
            textBox3.Enabled = !value;
            label2.Enabled = !value;
            label3.Enabled = !value;
        }
    }
}
