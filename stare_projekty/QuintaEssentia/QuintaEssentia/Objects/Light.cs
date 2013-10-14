/* Quinta Essentia by Przemysław Pastuszka
 * file:Light.cs
 * Contains: Class Light
 * Light: class is used to handle light and its movement
 * Last modification:24-07-2009
 */

using System.Windows.Forms;
using SlimDX;
using System;
using System.IO;

namespace QuintaEssentia
{
    class Light
    {

        //variables
        private Vector3 lightDir, down = new Vector3(0, -1, 0); //light direction after rotation, and down vector        
        private float rotationSpeed = 1.5f;
        public Vector2 rotation=Vector2.Zero;
        public float ambient=0;
        public bool frozen=false;//frozen light cannot be moved
        private LightForm form; //used when edit mode is on

        bool changedPosition; //if current and previous position are different
        public bool skyToBeRecalculated;
        //initializing
        public Light(StreamReader file)
        {
            Load(file);
            lightDir = Vector3.TransformCoordinate(down, Matrix.RotationX(rotation.X) * Matrix.RotationY(rotation.Y));
        }
        private void Load(StreamReader file)
        {
            if (file == null)
                return;
            //load options
            frozen = Convert.ToBoolean(file.ReadLine());
            ambient = (float)Convert.ToDouble(file.ReadLine());
            rotation.X = (float)Convert.ToDouble(file.ReadLine());
            rotation.Y = (float)Convert.ToDouble(file.ReadLine());
        }

        //edit mode
        public void MakeForm()
        {
            form = new LightForm();
            if (frozen)
                form.checkBox1.Checked = true;
            form.label7.Text = ambient.ToString();
            form.trackBar1.Value = (int)(ambient * 20);
            form.trackBar1.ValueChanged += new EventHandler(trackBar1_ValueChanged);
            form.checkBox1.CheckedChanged += new EventHandler(checkBox1_CheckedChanged);

            try { ((Editor)Interface.mainInterface.GetModuleByIndex(2)).AddNewForm(form, Lang.main.GetText(17), ""); }
            catch { ((Editor)Interface.mainInterface.GetModuleByIndex(2)).AddNewForm(form, "Global light", ""); };
        }
        public void Save(StreamWriter file)
        {
            file.WriteLine(frozen.ToString());
            file.WriteLine(ambient.ToString());
            file.WriteLine(rotation.X.ToString());
            file.WriteLine(rotation.Y.ToString());
        }
        void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (form.checkBox1.Checked)
                frozen = true;
            else
                frozen = false;
        }
        void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            ambient = form.trackBar1.Value / 20f;
            form.label7.Text = ambient.ToString();
        }

        //usual functions
        public void Update(double time)
        {          
            if (((Console)Interface.mainInterface.GetModuleByIndex(0)).LastCommand() == "LIGHT FREEZE")
                frozen = !frozen;
            if (!frozen)
            {
                Vector2 lastRot = rotation;
                if (InOut.mainIO.IsPressed(Keys.J))
                    rotation.Y += rotationSpeed * (float)time;
                if (InOut.mainIO.IsPressed(Keys.L))
                    rotation.Y -= rotationSpeed * (float)time;
                if (InOut.mainIO.IsPressed(Keys.I))
                    rotation.X += rotationSpeed * (float)time;
                if (InOut.mainIO.IsPressed(Keys.K))
                    rotation.X -= rotationSpeed * (float)time;

                skyToBeRecalculated = false;
                if (lastRot != rotation)
                    changedPosition = true;
                else
                {
                    if (changedPosition)
                        skyToBeRecalculated = true;
                    changedPosition = false;
                }

                lightDir = Vector3.TransformCoordinate(down, Matrix.RotationX(rotation.X) * Matrix.RotationY(rotation.Y));
                if (form!=null)
                {
                    form.label2.Text = "X: " + lightDir.X;
                    form.label3.Text = "Y: " + lightDir.Y;
                    form.label4.Text = "Z: " + lightDir.Z;
                }
            }
        }

        //light info
        public Vector3 GetLightDir()
        {
            return lightDir;
        }
        public float GetAmbient()
        {
            return ambient;
        }
    }
}
