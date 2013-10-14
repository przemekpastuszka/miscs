using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    class TextSprite
    {
        const float maxSize = 50f;
        SpriteFont czcionka;
        public void LoadContent(ContentManager content)
        {
            czcionka=content.Load<SpriteFont>("Regular");
        }
        public void Draw(SpriteBatch sprite, String text, int size, Vector2 position)
        {
            Draw(sprite, text, size, position, Color.White);
        }
        public void Draw(SpriteBatch sprite, String text, int size, Vector2 position, Color kolor)
        {
            float wielkosc=size/maxSize;
            Vector2 skala = new Vector2(Config.rozdzielczosc.X / 1280f, Config.rozdzielczosc.Y / 1024f);
            sprite.DrawString(czcionka, text, position * skala, kolor, 0,
                czcionka.MeasureString(text) * wielkosc / 2, skala * wielkosc, SpriteEffects.None, 0);
        }
        public Vector2 Miara(String text)
        {
            return czcionka.MeasureString(text);
        }
    }
}
