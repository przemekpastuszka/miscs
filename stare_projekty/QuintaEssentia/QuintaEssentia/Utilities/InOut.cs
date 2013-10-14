/* Quinta Essentia by Przemysław Pastuszka
 * file:InOut.cs
 * Contains: Class InOut
 * InOut: class is used to provide informations about what user has done
 * Last modification:28-07-2009
 */
using System.Windows.Forms;
using SlimDX;

namespace QuintaEssentia
{
    class InOut
    {
        //variables
        private int[] keyState; //it keeps states of all keys (whether they are pressed or not)
        private int counter = 1, mouseWheelDelta=0; //it is number of a current frame
        private Keys lastPressedKey;
        private bool left = false, right = false;
        private Vector2 mousePos=new Vector2(), mouseDelta=new Vector2();
        public static InOut mainIO; //singleton

        //creation methods
        /// <summary>
        /// Creates the only instance of 'InOut' class
        /// </summary>
        public static void Create(Form window)
        {
            if (mainIO == null)
            {
                mainIO = new InOut();
                mainIO.CaptureForm(window);
                window.MouseWheel += new MouseEventHandler(mainIO.MouseWheel);
                window.MouseMove += new MouseEventHandler(mainIO.window_MouseMove);
                window.MouseDown += new MouseEventHandler(mainIO.window_MouseDown);
            }
        }

        private void window_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
                left = true;
            if (e.Button == MouseButtons.Right)
                right = true;
        }


        public void CaptureForm(Form window)
        {
            window.KeyDown += new KeyEventHandler(mainIO.KeyPressed);
            window.KeyUp += new KeyEventHandler(mainIO.KeyUp);
            window.MouseUp += new MouseEventHandler(window_MouseUp);
            CaptureControls(window.Controls);
        }

        void window_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
                left = false;
            if (e.Button == MouseButtons.Right)
                right = false;
        }

        void window_MouseMove(object sender, MouseEventArgs e)
        {
            mouseDelta.X = e.X - mousePos.X;
            mouseDelta.Y = e.Y - mousePos.Y;
            mousePos.X = e.X; mousePos.Y = e.Y;
        }
        public Vector2 GetMousePosition()
        {
            return mousePos;
        }
        public Vector2 GetMouseDelta()
        {
            return mouseDelta;
        }
        private void CaptureControls(Control.ControlCollection collection)
        {
            foreach (Control c in collection)
            {
                c.KeyDown += new KeyEventHandler(mainIO.KeyPressed);
                c.KeyUp += new KeyEventHandler(mainIO.KeyUp);
                c.MouseUp += new MouseEventHandler(window_MouseUp);
                CaptureControls(c.Controls);
            }
        }
        /// <summary>
        /// Initialization
        /// </summary>
        private InOut()
        {
            keyState = new int[300];
            for (int i = 0; i < 300; i++)
                keyState[i] = 0;
        }

        //handling keyboard 
        private void KeyPressed(object sender, KeyEventArgs arg)
        {        
            keyState[arg.KeyValue] = counter;
            lastPressedKey = arg.KeyData;
        }
        private void KeyUp(object sender, KeyEventArgs arg)
        {
            keyState[arg.KeyValue] = 0;
        }

        //give informations about keyboard state
        /// <summary>
        /// check if the key is pressed (we don't care if it was pressed this frame or earlier)
        /// </summary>
        /// <param name="key">Desired key</param>
        /// <returns>Is it pressed or not</returns>
        public bool IsPressed(Keys key)
        {                               
            return keyState[(int)key] != 0;
        }
        /// <summary>
        /// check if it has been pressed just now (it was 'up' before current frame)
        /// </summary>
        /// <param name="key">Desired key</param>
        /// <returns>I hate comments</returns>
        public bool IsPressedNow(Keys key) 
        {
            return keyState[(int)key] == counter; //got the same number current frame? So it has been already pressed!
        }
        public Keys LastPressedKey()
        {
            return lastPressedKey;
        }

        //handling mouse
        private void MouseWheel(object sender, MouseEventArgs arg)
        {
            mouseWheelDelta = arg.Delta;
        }

        //gives informations about mouse state
        public int GetWheelDelta()
        {
            return mouseWheelDelta / 120;
        }
        public bool IsLeftPressed()
        {
            return left;
        }
        public bool IsRightPressed()
        {
            return right;
        }

        //usual methods
        /// <summary>
        /// It counts frames. The counter is used to indicate if key has been already pressed or it was pressed earlier
        /// </summary>
        public void Update()
        {
            mouseWheelDelta = 0;
            mouseDelta = Vector2.Zero;
            counter++;
        }
    }
}
