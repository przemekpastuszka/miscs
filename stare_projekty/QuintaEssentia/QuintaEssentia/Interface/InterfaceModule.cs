/* Quinta Essentia by Przemysław Pastuszka
 * file:InterfaceModule.cs
 * Contains: Class InterfaceModule
 * InterfaceModule: class which is only a base for other classes
 * Last modification:22-07-2009
 */
using System;
using D3D10 = SlimDX.Direct3D10;

namespace QuintaEssentia
{
    class InterfaceModule
    {
        //variables
        protected D3D10.Font font; //each module got the font
        protected bool isOn; //each module can be turned on/off
        public String name; //used for GetModuleByName in Interface Class

        //Virtual methods
        public virtual void Update() { }
        public virtual void Draw(D3D10.Sprite sprite) { }
    }
}
