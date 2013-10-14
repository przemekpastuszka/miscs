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
    public partial class EditorForm : Form
    {
        public EditorForm()
        {
            InitializeComponent();
            try { this.Text = Lang.main.GetText(16); }
            catch { }
        }
    }
}
