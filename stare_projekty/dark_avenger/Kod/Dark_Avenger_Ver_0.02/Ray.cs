using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    class Ray : Sprite
    {
        float timeOfExistence = 0.5f;
        float a,b;
        bool czy = false;
        int obrazenia = 1000;
        float lastTime=0;
        public Ray()
        {   
            nrWarstwy = 4;
            typ = Type.bullet;
            wielkoscObiektu = new Vector2(1, 1);
        }
        public override void CollisionCheck(Sprite obiekt)
        {
            if (Config.statusGry == GameState.inProgress&&Obliczarka.DPunktuIProstej(a, -1, b, obiekt.pozycja) < obiekt.teksturaPodstawowa.Width*obiekt.wielkoscObiektu.X / 2 + teksturaPodstawowa.Height / 2
                && obiekt.typ == Type.monster && obiekt.stan == State.alive)
            {
                if ((czy && obiekt.pozycja.X > Config.pozycjaGracza.X) || (!czy && obiekt.pozycja.X < Config.pozycjaGracza.X))
                    obiekt.PrzyjmijUderzenie(obrazenia*lastTime);
            }
        }
        private void PoliczAll()
        {
            pozycja = Config.pozycjaGracza;
            if (Config.pozycjaCelownika.X > pozycja.X)
                czy = true;
            else
                czy = false;
            a = (pozycja.Y - Config.pozycjaCelownika.Y) / (pozycja.X - Config.pozycjaCelownika.X);
            b = pozycja.Y - a * pozycja.X;
        }
        private float PrzerobKat(float kat)
        {
            if (czy)
                return kat+(float)Math.PI;
            return kat;
        }
        public override void LoadContent(ContentManager content)
        {
            teksturaPodstawowa = content.Load<Texture2D>("ray");
            wielkoscObiektu = new Vector2(1, 1);
        }
        public override void Update(GameTime time)
        {
            if (Config.statusGry == GameState.inProgress)
            {
                lastTime = (float)time.ElapsedGameTime.TotalSeconds;
                PoliczAll();
                timeOfExistence -= (float)time.ElapsedGameTime.TotalSeconds;
                if (timeOfExistence < 0)
                    stan = State.toDelete;
            }
        }
        public override void Draw(SpriteBatch sprite)
        {
            if (Config.statusGry != GameState.menu)
            {
                Vector2 skala = new Vector2(Config.rozdzielczosc.X / 1280f, Config.rozdzielczosc.Y / 1024f);
                sprite.Draw(teksturaPodstawowa, Camera.PozycjaWgKamery(pozycja) * skala, null, Color.White, PrzerobKat((float)Math.Atan(a)),
                    new Vector2(teksturaPodstawowa.Width, teksturaPodstawowa.Height) * wielkoscObiektu, skala * wielkoscObiektu, SpriteEffects.None, 0);
            }
        }
    }
}
