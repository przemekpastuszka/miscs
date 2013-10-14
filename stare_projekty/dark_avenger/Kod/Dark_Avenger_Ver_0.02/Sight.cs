using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    class Sight : Sprite
    {
        public override void Initialize()
        {
            nrWarstwy = 6;
            base.Initialize();
        }
        public override void LoadContent(ContentManager content)
        {
            teksturaPodstawowa = content.Load<Texture2D>("sight");
        }
        public override void Draw(SpriteBatch sprite)
        {
            Rysuj(sprite, teksturaPodstawowa, Camera.PozycjaWgKamery(Config.pozycjaCelownika), wielkoscObiektu);
        }
        public override void Update(GameTime time)
        {
            pozycja = Config.pozycjaCelownika;
            Vector2 skala = new Vector2(1280f / Config.rozdzielczosc.X, 1024f / Config.rozdzielczosc.Y);
            Config.pozycjaCelownika.X = Mouse.GetState().X * skala.X;
            Config.pozycjaCelownika.Y = Mouse.GetState().Y * skala.Y;
            Config.pozycjaCelownika += Camera.pozycja - new Vector2(640, 512);
        }
        public override void CollisionCheck(Sprite a)
        {
            if (Obliczarka.KolizjaDlaOkregow(this, a))
            {
                a.WyswietlajSie();
            }
        }
    }
}
