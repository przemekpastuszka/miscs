using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    class ElementMenu
    {
        private TextSprite czcionka = new TextSprite();
        private bool czyPodswietlony = false;
        private String text;
        private Vector2 wielkosc,pozycja;
        public delegate void Funkcja(String nazwa,Game1 gejm);
        private Funkcja temp;
        Texture2D tekstura;
        Game1 temporary;
        public ElementMenu(Game1 to,String a, Funkcja delegat,Vector2 poz)
        {
            temp = delegat;
            text = a;
            temporary = to;
            wielkosc += new Vector2(10,10);
            pozycja = poz;
        }
        public void LoadContent(ContentManager content)
        {
            tekstura = content.Load<Texture2D>("kropek");
            czcionka.LoadContent(content);
            wielkosc = czcionka.Miara(text);
        }
        public void Update()
        {
            Vector2 celownik = Camera.PozycjaWgKamery(Config.pozycjaCelownika) - pozycja;
            if (Math.Abs(celownik.X) <= wielkosc.X / 2 && Math.Abs(celownik.Y) <= wielkosc.Y / 2)
            {
                czyPodswietlony = true;
                if (Klawisze.LewyPrzycisk())
                    temp(text,temporary);
            }
            else
                czyPodswietlony = false;
        }
        public void Draw(SpriteBatch sprite)
        {
            if (czyPodswietlony)
            {
                Vector2 skala = new Vector2(Config.rozdzielczosc.X / 1280f, Config.rozdzielczosc.Y / 1024f);
                sprite.Draw(tekstura, pozycja * skala, null, Color.White, 0,
                     new Vector2(0.5f,0.5f), skala * wielkosc, SpriteEffects.None, 0);
                czcionka.Draw(sprite, text, 50, pozycja, Color.Black);
            }
            else
                czcionka.Draw(sprite, text, 50, pozycja);
        }
    }
}
