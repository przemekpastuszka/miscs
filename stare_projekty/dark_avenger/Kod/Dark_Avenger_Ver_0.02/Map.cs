using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    class Map : Sprite
    {
        public override void Initialize()
        {
            pozycja = new Vector2(0, 0);
            wielkoscObiektu = new Vector2(1, 1);
            nrWarstwy = 1;
        }
        public override void LoadContent(ContentManager content)
        {
            teksturaPodstawowa = content.Load<Texture2D>("end");
        }
        public override void Draw(SpriteBatch sprite)
        {
            if (Config.statusGry != GameState.menu)
            {
                while (pozycja.X < Config.rozmiarPlanszy.X)
                {
                    pozycja.Y = 0;
                    if (Camera.CzyWWidoku(this))
                        Rysuj(sprite, teksturaPodstawowa, Camera.PozycjaWgKamery(pozycja), wielkoscObiektu);
                    pozycja.Y = Config.rozmiarPlanszy.Y;
                    if (Camera.CzyWWidoku(this))
                        Rysuj(sprite, teksturaPodstawowa, Camera.PozycjaWgKamery(pozycja), wielkoscObiektu);
                    pozycja.X += teksturaPodstawowa.Width + 20;
                }
                pozycja = new Vector2(0, 0);
                while (pozycja.Y < Config.rozmiarPlanszy.Y)
                {
                    pozycja.X = 0;
                    if (Camera.CzyWWidoku(this))
                        Rysuj(sprite, teksturaPodstawowa, Camera.PozycjaWgKamery(pozycja), wielkoscObiektu);
                    pozycja.X = Config.rozmiarPlanszy.X;
                    if (Camera.CzyWWidoku(this))
                        Rysuj(sprite, teksturaPodstawowa, Camera.PozycjaWgKamery(pozycja), wielkoscObiektu);
                    pozycja.Y += teksturaPodstawowa.Height + 20;
                }
                pozycja = new Vector2(0, 0);
            }
        }
    }
}
