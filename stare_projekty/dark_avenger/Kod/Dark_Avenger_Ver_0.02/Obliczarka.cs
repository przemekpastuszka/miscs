using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    class Obliczarka
    {
        //zwraca punkt na prostej |pozycja, cel|, oddalony o 'dlugoscPrzesuniecia' od pozycja
        public static Vector2 PunktDocelowy(Vector2 pozycja, Vector2 cel, float dlugoscPrzesuniecia)
        {
            return PunktDocelowy(pozycja, cel, dlugoscPrzesuniecia, true);
        }
        public static Vector2 PunktDocelowy(Vector2 pozycja, Vector2 cel, float dlugoscPrzesuniecia, bool czyCelOstateczny)
        {
            if (czyCelOstateczny&&Odleglosc(pozycja, cel) <= dlugoscPrzesuniecia)
                return cel;
            if (pozycja.X - cel.X == 0)
            {
                if (cel.Y > pozycja.Y)
                    return new Vector2(pozycja.X, pozycja.Y += dlugoscPrzesuniecia);
                else
                    return new Vector2(pozycja.X, pozycja.Y -= dlugoscPrzesuniecia);
            }
            float a = (pozycja.Y - cel.Y) / (pozycja.X - cel.X);
            float x = (float)Math.Sqrt(Math.Pow(dlugoscPrzesuniecia, 2) / (1 + Math.Pow(a, 2)));
            if (cel.X > pozycja.X && a >= 0)
                return new Vector2(pozycja.X += x, pozycja.Y += a * x);
            if (cel.X > pozycja.X && a < 0)
                return new Vector2(pozycja.X += x, pozycja.Y += a * x);
            if (cel.X < pozycja.X && a >= 0)
                return new Vector2(pozycja.X -= x, pozycja.Y -= a * x);
            return new Vector2(pozycja.X -= x, pozycja.Y -= a * x);
        }
        public static float Odleglosc(Vector2 a, Vector2 b)
        {
            return (float)Math.Sqrt(Math.Pow(a.X - b.X, 2) + Math.Pow(a.Y - b.Y, 2));
        }
        public static bool KolizjaDlaOkregow(Sprite a, Sprite b)
        {
            float d = Odleglosc(a.pozycja, b.pozycja);
            if ((a.teksturaPodstawowa.Height * a.wielkoscObiektu.Y + +b.teksturaPodstawowa.Height * b.wielkoscObiektu.Y) / 2 > d)
                return true;
            return false;
        }
        public static float DPunktuIProstej(float wspA, float wspB, float wspC, Vector2 punkt)
        {
            float d=(float)(Math.Abs(wspA*punkt.X+wspB*punkt.Y+wspC) / Math.Sqrt((float)(Math.Pow(wspA, 2) + Math.Pow(wspB, 2))));
            return d;
        }
    }
}
