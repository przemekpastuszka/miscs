using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    class Camera
    {
        //pozycja kamery (początkowo na środku)
        private static Vector2 cel;
        public static Vector2 pozycja = new Vector2(Config.rozmiarPlanszy.X / 2, Config.rozmiarPlanszy.Y / 2);
       // public static Vector2 pozycja = new Vector2(0,0);
        static float stalaPredkosci = 0.7f; //stała, która pozwala na dopasowanie predkosci kamery
        public static bool CzyWWidoku(Sprite a) //czy obiekt lezy w widoku kamery
        {
            if (Obliczarka.Odleglosc(pozycja, a.pozycja) - a.teksturaPodstawowa.Height / 2 < 1820)
                return true;
            return false;
        }
        public static Vector2 PozycjaWgKamery(Vector2 miejsce) //zwróc współrzędne obiektu w widoku kamery
        {
            return miejsce - pozycja+new Vector2(640,512);
        }
        
        public static void Update(GameTime time) 
        {
            cel = pozycja;
            if (Config.statusGry == GameState.inProgress)
            {
                stalaPredkosci = 0.7f;
                cel = Config.pozycjaGracza;
            }
            if(Config.statusGry==GameState.paused||Config.statusGry==GameState.ended)
            {
                cel = Config.pozycjaCelownika;
                stalaPredkosci = 0.9f;
            }
            float d=Obliczarka.Odleglosc(pozycja, cel);
            if(d>5)
                pozycja=Obliczarka.PunktDocelowy(pozycja, cel, (float)time.ElapsedGameTime.TotalSeconds * d * stalaPredkosci);
        }
    }
}
