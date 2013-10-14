/* Quinta Essentia by Przemysław Pastuszka
 * file:Console.cs
 * Contains: Class InterfaceModule->Console
 * Console: class to draw on the screen and support the console during runtime
 * Last modification:22-07-2009
 */
using System;
using System.Drawing;
using System.Windows.Forms;
using D3D10 = SlimDX.Direct3D10;

namespace QuintaEssentia
{
    class Console : InterfaceModule
    {
        //variables
        private String[] lines; //it keeps string lines to be displayed (the old ones)
        private String currentLine="", lastCommand=""; //as the names says
        private int counter = 0; //current position in 'lines' array

        //constructor
        /// <summary>
        /// Preparing console - loading font and creating 'space' for messages
        /// </summary>
        public Console()
        {
            name = "Console";
            isOn = false;

            //font loading
            D3D10.FontDescription desc = new SlimDX.Direct3D10.FontDescription();
            desc.FaceName = "Arial";
            desc.Height = 7;
            desc.Width = 10;
            try
            {
                font = ResourceManager.mainManager.LoadFont(desc.FaceName+"H"+desc.Height+"W"+desc.Width, desc);
            }
            catch (System.Exception ex)
            {
                Log.mainLog.AddEvent("Font " + desc.FaceName + " cannot be loaded. Console module will not be visible. " +
                    "(error message: '" + ex.Message + "')", Priority.medium);
            }

            //allocating memory for text
            lines = new String[10];
            for (int i = 0; i < 10; i++)
                lines[i] = "";
        }

        //Console support
        /// <summary>
        /// As the name says
        /// </summary>
        /// <returns>Last command set by the user</returns>
        public String LastCommand()
        {
            return lastCommand;
        }
        /// <summary>
        /// It adds line to the console
        /// </summary>
        /// <param name="text">Text to be displayed</param>
        public void AddLine(String text)
        {
            lines[counter] = text;
            counter = (counter + 1) % 10;
        }

        //Functions which implements/overrides the InterfaceModule virtual methods
        /// <summary>
        /// Reads strings from the keyboard
        /// </summary>
        public override void Update()
        {
            lastCommand = ""; //command from last frame is out of date

            //check tilde
            if (InOut.mainIO.IsPressedNow(Keys.Oemtilde))
            {
                isOn = !isOn;
                return;
            }

            if (!isOn)
                return;

            if (InOut.mainIO.IsPressedNow(InOut.mainIO.LastPressedKey()))
            {
                Keys key = InOut.mainIO.LastPressedKey();
                if (key == Keys.Enter)
                {
                    lastCommand = currentLine;
                    lines[counter] = currentLine;
                    currentLine = "";
                    counter = (counter + 1) % 10;
                }
                else
                    if (key == Keys.Back)
                    {
                        if (currentLine.Length > 0)
                            currentLine = currentLine.Remove(currentLine.Length - 1, 1);
                    }
                    else
                        if (key == Keys.Space)
                            currentLine += " ";
                        else
                            currentLine += (char)key;
            }
        }
        /// <summary>
        /// Draws the console
        /// </summary>
        /// <param name="sprite">Sprite for better performance</param>
        public override void Draw(D3D10.Sprite sprite)
        {
            if (isOn&&font!=null)
            {   //old ones
                for (int i = 0; i < 10; i++)
                {
                    font.Draw(sprite, lines[(i + counter) % 10], new Rectangle(140, i * 10, 600, 20),
                        SlimDX.Direct3D10.FontDrawFlags.Left, (uint)Color.AliceBlue.ToArgb());
                } //and the current one
                font.Draw(sprite, ">" + currentLine, new Rectangle(140, 105, 600, 20),
                        SlimDX.Direct3D10.FontDrawFlags.Left, (uint)Color.AliceBlue.ToArgb());
            }
        }
    }
}
