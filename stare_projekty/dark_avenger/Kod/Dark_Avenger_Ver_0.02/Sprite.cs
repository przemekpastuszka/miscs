using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    enum State { alive, toDelete, dead }; //do przechowywania stanu obiektu (jeśli toDelete to kasujemy)
    enum Type { player, monster, item, neutral,bullet } //do przechowywania typu obiektu
    class Sprite //podstawowa klasa po której będziemy dziedziczyć
    {
        public Texture2D teksturaPodstawowa; //każdy obiekt musi mieć choć jedną teksturę :)
        public State stan;
        public bool czySieWyswietlac = false;
        public Type typ; //przydaje się chociażby do określenia zależności podczas kolizji
        protected String nazwa; //nazwa obiektu (pomocna przy wyświetleniu info na ekranie
        public int nrWarstwy; //w której kolejności obiekt zostanie wyświetlony
        public Vector2 pozycja=new Vector2();
        public Vector2 wielkoscObiektu; //rzeczywista wielkość obiektu to wielkosc*wielkość_tekstury
        public virtual void Draw(SpriteBatch sprite) { }
        protected float czasWyswietlania = 1f;
        public virtual void Initialize() 
        {
            typ = Type.neutral;
            wielkoscObiektu = new Vector2(1, 1);
        }
        public virtual void Update(GameTime time) { }
        public virtual void LoadContent(ContentManager content) { }

        //rysuje teksture na ekranie w odpowiednim miejscu i w odpowiedniej skali
        public void Rysuj(SpriteBatch sprite, Texture2D tekstura, Vector2 pozycja, Vector2 wielkosc) 
        { 
            Vector2 skala = new Vector2(Config.rozdzielczosc.X / 1280f, Config.rozdzielczosc.Y / 1024f);
            sprite.Draw(tekstura, pozycja * skala, null, Color.White, 0, new Vector2(tekstura.Width / 2, tekstura.Height / 2), skala * wielkosc, SpriteEffects.None, 0);
        }
        public virtual void CollisionCheck(Sprite a) { } //sprawdzi czy jest kolizja z obiektem a (jesli tak, to wykona akcje)
        public virtual State ZwrocStan() { return stan; }
        public virtual void PrzyjmijUderzenie(float obrazenia) { } //wykonuje akcje po ataku przeciwnika
        public void WyswietlajSie()
        {
            czasWyswietlania = 0.3f;
            czySieWyswietlac = true;
        }
    }
}
