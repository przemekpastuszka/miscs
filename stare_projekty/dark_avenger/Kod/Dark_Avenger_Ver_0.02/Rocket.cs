using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    class Rocket : Sprite
    {
        int predkosc = 1000;
        float timeOfExplosion = 0.3f, timeLeft = 0.3f;
        int obrazenia = 2000;
        float lastTime = 0;
        Texture2D eksplozja;
        Explosion materialWybuchowy;
        Vector2 cel, punktWPolowie;
        public Rocket()
        {
            pozycja = Config.pozycjaGracza;
            materialWybuchowy = Explosion.flight;
            cel = Config.pozycjaCelownika;
            float a = (pozycja.Y - Config.pozycjaCelownika.Y) / (pozycja.X - Config.pozycjaCelownika.X);
            float b = pozycja.Y - a * pozycja.X;
            if (Config.pozycjaCelownika.X > pozycja.X)
                cel.X = Config.rozdzielczosc.X + Config.rozmiarPlanszy.X + 20;
            else
                cel.X = -Config.rozdzielczosc.X - 20;
            cel.Y = a * cel.X + b;
            wielkoscObiektu = new Vector2(1, 1);
            stan = State.alive;
            nrWarstwy = 4;
            typ = Type.bullet;
            punktWPolowie = Obliczarka.PunktDocelowy(pozycja, cel, Obliczarka.Odleglosc(pozycja, cel) / 2);
        }
        public override void Initialize() { }
        public override void Draw(SpriteBatch sprite)
        {
            Rysuj(sprite, teksturaPodstawowa, Camera.PozycjaWgKamery(pozycja), wielkoscObiektu);
        }
        public override void CollisionCheck(Sprite a)
        {
            if (a.typ == Type.monster && a.stan == State.alive && materialWybuchowy != Explosion.exploded && Obliczarka.KolizjaDlaOkregow(this, a))
            {
                wielkoscObiektu = new Vector2(0, 0);
                materialWybuchowy = Explosion.exploded;
                teksturaPodstawowa = eksplozja;
            }
            if (a.typ == Type.monster && a.stan == State.alive && materialWybuchowy == Explosion.exploded && Obliczarka.KolizjaDlaOkregow(this, a))
                a.PrzyjmijUderzenie(obrazenia * lastTime);
        }
        public override void LoadContent(ContentManager content)
        {
            teksturaPodstawowa = content.Load<Texture2D>("slug");
            eksplozja = content.Load<Texture2D>("explosion");
        }
        public override void Update(GameTime time)
        {
            lastTime = (float)time.ElapsedGameTime.TotalSeconds;
            if (materialWybuchowy == Explosion.flight)
            {
                pozycja = Obliczarka.PunktDocelowy(pozycja, cel, predkosc * (float)time.ElapsedGameTime.TotalSeconds);
            }
            else
            {
                float powiekszenie = (float)time.ElapsedGameTime.TotalSeconds * (2.45f / timeOfExplosion);
                wielkoscObiektu += new Vector2(powiekszenie, powiekszenie);
                timeLeft -= (float)time.ElapsedGameTime.TotalSeconds;
                if (timeLeft < 0)
                    stan = State.toDelete;
            }
        }
    }
}
