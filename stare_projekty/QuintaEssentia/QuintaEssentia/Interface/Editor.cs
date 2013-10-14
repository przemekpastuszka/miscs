using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Text;

namespace QuintaEssentia
{
    class Editor : InterfaceModule
    {
        //variables
        EditorForm form; //main editor window
        Dictionary<String, TreeNode> nodes; //here are kept nodes with string keys (it allows to find them easily)
        Form lastSelected; //last showed window (will be hidden before the next is shown)
        bool wasNodeClicked = false; //choosing the node occurs only when user clicks control, so this variable is needed to check this

        //constructor
        public Editor()
        {
            name = "Editor";
            isOn = true;

            nodes = new Dictionary<string, TreeNode>();

            form = new EditorForm();
            form.TopMost = true;
            
            InOut.mainIO.CaptureForm(form);
            form.treeView1.AfterSelect += new TreeViewEventHandler(treeView1_AfterSelect);
            form.treeView1.Click += new EventHandler(treeView1_Click);
            form.Show();
        }

        //selection handling
        /// <summary>
        /// This function only sets the wasNodeClicked variable to true (after user click of course)
        /// </summary>
        /// <param name="sender">Sth</param>
        /// <param name="e">Sth</param>
        void treeView1_Click(object sender, EventArgs e)
        {
            wasNodeClicked = true;
        }
        void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (wasNodeClicked) //so only when user has clicked the treeNode! Other forms of selecting are not allowed
            {
                Form temp = (Form)e.Node.Tag;
                if (lastSelected != null)
                {
                    temp.Location = lastSelected.Location; //same location as the las window
                    lastSelected.Hide();
                }
                else
                    temp.Location = new System.Drawing.Point(form.Location.X, form.Location.Y + form.Height+10);
                lastSelected = temp;
                temp.Show();
            }
        }
        
        //forms controling
        public void AddNewForm(Form newForm,String name, String parent)
        {
            TreeNode temp = new TreeNode(name);
            temp.Tag = newForm;
            if (nodes.ContainsKey(parent)) //if there is a appropriate node it will be the parent
                nodes[parent].Nodes.Add(temp);
            else
                form.treeView1.Nodes.Add(temp); //elsewhere treeView is a parent
            nodes.Add(name, temp);
            InOut.mainIO.CaptureForm(newForm);
            newForm.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle; //cannot change its size
            newForm.TopMost = true;
            newForm.Show();
            newForm.Hide();
        }

        //usual functions
        public override void Update()
        {
            wasNodeClicked = false;
        }
    }
}
