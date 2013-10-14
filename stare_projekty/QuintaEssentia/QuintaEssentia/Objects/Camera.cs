/* Quinta Essentia by Przemysław Pastuszka
 * file:Camera.cs
 * Contains: Class Camera
 * Camera: class is used to handle camera, its movement, making matrices and bounding frustum
 * Last modification:28-07-2009
 */
using System;
using System.Drawing;
using System.Windows.Forms;
using SlimDX;
using System.IO;

namespace QuintaEssentia
{
    enum CameraType { isometricDependant, isometricFree, free };
    class Camera
    {
        //variables
        //take care of movement
        private Vector3 position=new Vector3(0,4,0), lookVector,upVector;
        private float currentAngle = 35;
        private float cameraSpeed = 20, rotationSpeed = 1.5f;
        private Vector3 left = new Vector3(-1, 0, 0), right = new Vector3(1, 0, 0),
            top = new Vector3(0, 0, 1), bottom = new Vector3(0, 0, -1);
        private float maxHeight=40;
        private Vector2 rotation = new Vector2(); //angle of rotation in radians
        //matrices
        private Matrix viewMatrix, projectionMatrix, viewProjection;
        private BoundingFrustum frustum; //bounding frustum
        //variables needed if camera is terrain dependant
        private float aboveTheGround = 5; //if camera is terrain dependant it is the constant height value
        private CameraForm form; //used if edit mode is on
        private CameraType cameraType = CameraType.free;

        //initializng
        /// <summary>
        /// Constructor
        /// </summary>
        public Camera(StreamReader file)
        {
            Load(file);
            SetNewAngle((int)currentAngle);
            CreateProjectionMatrix();
            frustum = new BoundingFrustum();
        }
        private void Load(StreamReader file)
        {
            if (file == null)
                return;
            position.X = (float)Convert.ToDouble(file.ReadLine());
            position.Y = (float)Convert.ToDouble(file.ReadLine());
            position.Z = (float)Convert.ToDouble(file.ReadLine());
            int newAngle=Convert.ToInt32(file.ReadLine());
            float xRotation = (float)Convert.ToDouble(file.ReadLine());
            rotation.Y = (float)Convert.ToDouble(file.ReadLine());
            SetMaxHeight((float)Convert.ToDouble(file.ReadLine()));
            cameraType = (CameraType)Convert.ToInt32(file.ReadLine());
            if (cameraType != CameraType.free)
                SetNewAngle(newAngle);
            else
                rotation.X = xRotation;
            aboveTheGround = (float)Convert.ToDouble(file.ReadLine());
        }

        //edit mode
        public void MakeForm()
        {
            form = new CameraForm();
            form.label16.Text = currentAngle.ToString();
            form.trackBar1.Value = (int)currentAngle;
            form.textBox2.Text = aboveTheGround.ToString();
            form.textBox3.Text = maxHeight.ToString();
            if (cameraType==CameraType.isometricDependant)
                form.radioButton2.Checked = true;
            else
                form.radioButton2.Checked = true;
            form.trackBar1.ValueChanged += new EventHandler(trackBar1_ValueChanged);
            form.textBox2.TextChanged += new EventHandler(textBox2_TextChanged);
            form.textBox3.TextChanged += new EventHandler(textBox3_TextChanged);
            form.radioButton1.CheckedChanged += new EventHandler(radioButton1_CheckedChanged);
            form.radioButton2.CheckedChanged+=new EventHandler(radioButton1_CheckedChanged);
            try { ((Editor)Interface.mainInterface.GetModuleByIndex(2)).AddNewForm(form, Lang.main.GetText(33), ""); }
            catch { ((Editor)Interface.mainInterface.GetModuleByIndex(2)).AddNewForm(form, "Camera Editor", ""); }
        }
        void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            SetNewAngle(form.trackBar1.Value);
            form.label16.Text = currentAngle.ToString();
        }
        void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if (form.radioButton1.Checked)
                cameraType = CameraType.isometricDependant;
            else
                cameraType = CameraType.isometricFree;
        }
        void textBox3_TextChanged(object sender, EventArgs e)
        {
            try
            {
                float temp = (float)Convert.ToDouble(form.textBox3.Text);
                if (temp > 0)
                    SetMaxHeight(temp);
            }
            catch { }
        }
        void textBox2_TextChanged(object sender, EventArgs e)
        {
            try
            {
                float temp = (float)Convert.ToDouble(form.textBox2.Text);
                if (temp > 0)
                    aboveTheGround = temp;
            }
            catch { }
        }
        public void Save(StreamWriter file)
        {
            file.WriteLine(position.X.ToString());
            file.WriteLine(position.Y.ToString());
            file.WriteLine(position.Z.ToString());
            file.WriteLine(currentAngle);
            file.WriteLine(rotation.X.ToString());
            file.WriteLine(rotation.Y.ToString());
            file.WriteLine(maxHeight);
            file.WriteLine(((int)cameraType).ToString());
            file.WriteLine(aboveTheGround);
        }

        //matrices functions
        public void CreateProjectionMatrix()
        {
            projectionMatrix = Matrix.PerspectiveFovLH((float)Math.PI / 4, Game.gameClass.GetWindowSize().X / Game.gameClass.GetWindowSize().Y, 1f, 15000f);
        }
        public Matrix GetViewMatrix()
        {
            return viewMatrix;
        }
        public Matrix GetProjectionMatrix()
        {
            return projectionMatrix;
        }
        public Matrix GetViewProjectionMatrix()
        {
            return viewProjection;
        }

        //bounding frustum function
        public BoundingFrustum GetBoundingFrustum()
        {
            return frustum;
        }

        //camera position functions
        public Vector3 GetPosition()
        {
            return position;
        }
        public void SetMaxHeight(float height)
        {
            maxHeight = height;
            if (position.Y > maxHeight)
                position.Y = maxHeight;
        }
        /// <summary>
        /// Setting new angle (for isometric camera)
        /// </summary>
        /// <param name="a">Angle in degrees</param>
        public void SetNewAngle(int a)
        {
            double temp = position.Y / Math.Tan(a*Math.PI/180);
            lookVector = new Vector3(0, -position.Y, (float)temp);
            lookVector.Normalize();
            upVector = Vector3.Cross(lookVector, right);
            currentAngle = a;
        }
        
        //usual functions
        /// <summary>
        /// Updates the camera (reads commands from keyboard and creates new view matrix)
        /// </summary>
        /// <param name="time">Time between current and last frame</param>
        public void Update(double time)
        {
            if (cameraType != CameraType.free)
            {
                if (InOut.mainIO.IsPressed(Keys.Left))
                    rotation.Y -= (float)time * rotationSpeed;
                if (InOut.mainIO.IsPressed(Keys.Right))
                    rotation.Y += (float)time * rotationSpeed;
            }
            else
            {
                rotation.Y +=  InOut.mainIO.GetMouseDelta().X*0.01f;
                rotation.X += InOut.mainIO.GetMouseDelta().Y * 0.01f;
            }

            ProcessCommands();
            Matrix temp = Matrix.RotationX(rotation.X)*Matrix.RotationY(rotation.Y);
            if (InOut.mainIO.IsPressed(Keys.A))
                position += (float)time * cameraSpeed * Vector3.TransformCoordinate(left,temp);
            if (InOut.mainIO.IsPressed(Keys.D))
                position += (float)time * cameraSpeed * Vector3.TransformCoordinate(right, temp);
            if (InOut.mainIO.IsPressed(Keys.W))
                position += (float)time * cameraSpeed * Vector3.TransformCoordinate(top, temp);
            if (InOut.mainIO.IsPressed(Keys.S))
                position += (float)time * cameraSpeed * Vector3.TransformCoordinate(bottom, temp);

           

            Vector3 transformedLookV = Vector3.TransformCoordinate(lookVector, temp);
            if (cameraType!=CameraType.isometricDependant)
            {         
                position += transformedLookV * InOut.mainIO.GetWheelDelta();
                if (position.Y < 0)
                    position.Y = 0;
                if (position.Y > maxHeight)
                {
                    position -= transformedLookV * InOut.mainIO.GetWheelDelta();
                    position.Y = maxHeight;
                }

            }
            else
            {
                //reads height from bitmap and sets it (+value of 'aboveTheGround') as the camera height 
                if (position.X >= 0 && position.Z >= 0 && position.X < Globals.mapSize &&
                    position.Z < Globals.mapSize)
                    position.Y = Globals.GetHeight((int)position.X, (int)position.Z) * Globals.heightMultiplier + aboveTheGround;
            }
            
            viewMatrix=Matrix.LookAtLH(position, transformedLookV +position,
                Vector3.TransformCoordinate(upVector,temp));

            viewProjection = viewMatrix * projectionMatrix;
            frustum.CreateFrustum(viewProjection);
            if (form != null)
            {
                form.label7.Text = "X: " + position.X;
                form.label8.Text = "Y: " + position.Y;
                form.label9.Text = "Z: " + position.Z;
                form.label11.Text = "X: " + transformedLookV.X;
                form.label12.Text = "Y: " + transformedLookV.Y;
                form.label13.Text = "Z: " + transformedLookV.Z;
            }
        }
        private void ProcessCommands()
        {
            String command = ((Console)Interface.mainInterface.GetModuleByIndex(0)).LastCommand();
            if (command.StartsWith("CAMERA"))
            {
                if (command.StartsWith("CAMERA ANGLE"))
                {
                    int newAngle = Convert.ToInt32(command.Substring(13));
                    if (newAngle > 0 && newAngle < 90)
                        SetNewAngle(newAngle);
                }
                if (command.StartsWith("CAMERA MAX HEIGHT"))
                {
                    float height = (float)Convert.ToDouble(command.Substring(18));
                    if (height > 0)
                        SetMaxHeight(height);
                }
            }
        }
    }
}
