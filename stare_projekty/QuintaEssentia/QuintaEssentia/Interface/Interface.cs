/* Quinta Essentia by Przemysław Pastuszka
 * file:Interface.cs
 * Contains: Class Interface
 * Interface: class to handle all the Interface modules
 * Last modification:22-07-2009
 */
using System;
using System.Collections.Generic;
using D3D10 = SlimDX.Direct3D10;

namespace QuintaEssentia
{
    class Interface
    {
        //Variables
        private D3D10.Sprite sprite; //2D sprite which is used to draw interface modules (only to speed up the whole process)
        private StdVector<InterfaceModule> modules; //modules indexed by int
        private Dictionary<String, InterfaceModule> modulesNames; //modules indexed by String
        public static Interface mainInterface; //for singleton

        //Interface creation classes
        /// <summary>
        /// Private constructor, 'cos it is a singleton. It makes basic preparations for drawing/using the user interface
        /// </summary>
        /// <param name="d">Instance of DX10 device</param>
        private Interface()
        {
            sprite = new SlimDX.Direct3D10.Sprite(Game.gameClass.GetDevice(), 100);
            modules = new StdVector<InterfaceModule>(10);
            modulesNames = new Dictionary<string, InterfaceModule>();
        }
        /// <summary>
        /// It allows you tu create one instance of Interface
        /// </summary>
        /// <param name="d">Instance of DX10 device</param>
        public static void Create()
        {
            if (mainInterface == null)
                mainInterface = new Interface();
        }

        //Modules support
        /// <summary>
        /// It returns interface module with specified number (faster than searching by the name)
        /// </summary>
        /// <param name="index">Index of the module in the array</param>
        /// <returns>Desired InterfaceModule</returns>
        public InterfaceModule GetModuleByIndex(int index)
        {
            return modules[index];
        }
        /// <summary>
        /// It returns interface module with specified name (slower than searching by the number)
        /// </summary>
        /// <param name="index">Name of the module in the dictionary</param>
        /// <returns>Desired InterfaceModule</returns>
        public InterfaceModule GetModuleByName(String name)
        {
            return modulesNames[name];
        }
        /// <summary>
        /// Add module to interface
        /// </summary>
        /// <param name="module">Module to be added</param>
        public void AddModule(InterfaceModule module)
        {
            modules.Add(module);
            modulesNames.Add(module.name, module);
        }

        //Usual functions
        /// <summary>
        /// Updates all components of interface (for example - system console)
        /// </summary>
        public void Update()
        {
            for (int i = 0; i < modules.Length; i++)
                modules[i].Update();
        }
        /// <summary>
        /// It draws all components on the screen.
        /// </summary>
        public void Draw()
        {
            sprite.Begin(SlimDX.Direct3D10.SpriteFlags.None);
            for (int i = 0; i < modules.Length; i++)
                modules[i].Draw(sprite);
            sprite.End();
        }
        public void Dispose()
        {
            if(sprite!=null)
                sprite.Dispose();
        }
        
    }
}
