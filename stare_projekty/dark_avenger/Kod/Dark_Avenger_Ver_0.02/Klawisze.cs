using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    class Klawisze
    {
        private static List<Keys> wcisniete = new List<Keys>();
        private static ButtonState lewy = ButtonState.Released;
        public static bool CzyNacisniety(Keys klucz)
        {      
            if (!wcisniete.Contains(klucz) && Keyboard.GetState().IsKeyDown(klucz))
            {
                wcisniete.Add(klucz);
                return true;
            }
            return false;
        }
        public static bool LewyPrzycisk()
        {
            if (lewy == ButtonState.Released && Mouse.GetState().LeftButton == ButtonState.Pressed)
            {
                lewy = ButtonState.Pressed;
                return true;
            }
            return false;
        }
        public static void Update()
        {
            for (int i = 0; i < wcisniete.Count; )
            {
                if (Keyboard.GetState().IsKeyUp(wcisniete[i]))
                    wcisniete.RemoveAt(i);
                else
                    i++;
            }
            if (Mouse.GetState().LeftButton == ButtonState.Released)
                lewy = ButtonState.Released;
        }
    }
}
