/* Quinta Essentia by Przemysław Pastuszka
 * file:InfoPanel.cs
 * Contains: Class InterfaceModule->InfoPanel
 * InfoPanel: class to display diverse informations on the screen
 * Last modification:22-07-2009
 */
using System;
using System.Drawing;
using D3D10 = SlimDX.Direct3D10;

namespace QuintaEssentia
{
    class InfoPanel : InterfaceModule
    {
        //variables
        private String info=""; //all informations

        //constructor
        public InfoPanel()
        {
            name = "InfoPanel";
            isOn = true;

            //loading font
            D3D10.FontDescription desc = new SlimDX.Direct3D10.FontDescription();
            desc.FaceName = "Arial";
            desc.Height = 7;
            desc.Width = 10;
            try
            {
                font = ResourceManager.mainManager.LoadFont(desc.FaceName + "H" + desc.Height + "W" + desc.Width, desc);
            }
            catch (System.Exception ex)
            {
                Log.mainLog.AddEvent("Font " + desc.FaceName + " cannot be loaded. InfoPanel module will not be visible. " +
                    "(error message: '" + ex.Message + "')",Priority.medium);
            }
        }

        //InfoPanel support
        /// <summary>
        /// Adds information to the panel
        /// </summary>
        /// <param name="text">New information to be added</param>
        public void SetInfo(String text)
        {
            info += text+"\n\n";
        }

        //Functions which implements/overrides the InterfaceModule virtual methods
        /// <summary>
        /// Performs update (it is mainly used to turn on/off the panel
        /// </summary>
        public override void Update()
        {
            info = "";
            if (((Console)Interface.mainInterface.GetModuleByIndex(0)).LastCommand() == "INFOPANEL")
            {
                if (isOn)
                    ((Console)Interface.mainInterface.GetModuleByIndex(0)).AddLine("InfoPanel off");
                else
                    ((Console)Interface.mainInterface.GetModuleByIndex(0)).AddLine("InfoPanel on");
                isOn = !isOn;
            }
        }
        /// <summary>
        /// Draws the panel
        /// </summary>
        /// <param name="sprite">Sprite for better performance</param>
        public override void Draw(D3D10.Sprite sprite)
        {
            
            if (isOn&&font!=null)
                font.Draw(sprite, info, new Rectangle(0, 0, 130,300),
                    SlimDX.Direct3D10.FontDrawFlags.Left, (uint)Color.AliceBlue.ToArgb());
            
        }

    }
}
