using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    class Bullet : Sprite
    {
        int obrazenia,predkosc=1400;
        Vector2 cel;
        public Bullet(int sila, float odchylenie)
        {
            obrazenia = sila;
            pozycja = new Vector2();
            pozycja = Config.pozycjaGracza;
            float a = (pozycja.Y - Config.pozycjaCelownika.Y) / (pozycja.X - Config.pozycjaCelownika.X);
            a = (float)Math.Tan(Math.Atan(a)+odchylenie);
            float b = pozycja.Y - a * pozycja.X;
            if (Config.pozycjaCelownika.X > pozycja.X)
                cel.X = Config.rozdzielczosc.X+Config.rozmiarPlanszy.X+20;
            else
                cel.X = -Config.rozdzielczosc.X-20;
            cel.Y = a * cel.X + b;
            nrWarstwy = 4;
            wielkoscObiektu = new Vector2(1, 1);
            typ = Type.bullet;
        }
        public override void Draw(SpriteBatch sprite)
        {
            if (Config.statusGry != GameState.menu)
            {
                if (Camera.CzyWWidoku(this))
                    Rysuj(sprite, teksturaPodstawowa, Camera.PozycjaWgKamery(pozycja), wielkoscObiektu);
            }
        }
        public override void LoadContent(ContentManager content)
        {
            teksturaPodstawowa = content.Load<Texture2D>("slug");
        }
        public override void CollisionCheck(Sprite a)
        {
            if (Config.statusGry == GameState.inProgress&&Obliczarka.KolizjaDlaOkregow(this, a) && a.typ == Type.monster && a.stan == State.alive)
            {
                a.PrzyjmijUderzenie(obrazenia);
                stan = State.toDelete;
            }
        }
        public override void Update(GameTime time)
        {
            if (Config.statusGry == GameState.inProgress)
            {
                pozycja = Obliczarka.PunktDocelowy(pozycja, cel, predkosc * (float)time.ElapsedGameTime.TotalSeconds);
                if (pozycja.X < -Config.rozdzielczosc.X / 2 || pozycja.Y < -Config.rozdzielczosc.Y / 2 ||
                    pozycja.X > Config.rozmiarPlanszy.X + Config.rozdzielczosc.X ||
                    pozycja.Y > Config.rozmiarPlanszy.Y + Config.rozdzielczosc.Y)
                    stan = State.toDelete;
            }
           
        }
    }
}
