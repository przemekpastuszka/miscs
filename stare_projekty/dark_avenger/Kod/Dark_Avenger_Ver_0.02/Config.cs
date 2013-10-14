using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    enum Difficulty { easy, medium, hard };
    enum GameState { menu=0, inProgress, ended, paused };
    enum Menusy { main, difficulty, resolution, about };
    class Config
    {
        public static Random nadwornyRandomizer;
        public static Difficulty trudnosc;
        public static Menusy menu = Menusy.main;
        public static int wynik;
        public static GameState statusGry;
        //gdzie znajduje sie gracz, rozmiar pola gry, rozdzielczosc ekranu, pozycja kursora
        public static Vector2 pozycjaGracza=new Vector2(), rozmiarPlanszy=new Vector2(), rozdzielczosc=new Vector2(), pozycjaCelownika=new Vector2();
        public static List<Sprite> noweObiekty = new List<Sprite>(); //lista obiektów, które mają zostać dodane do gry
        public static void Inicjalizuj()
        {
            nadwornyRandomizer = new Random((int)DateTime.Now.Ticks);
            wynik = 0;
            rozmiarPlanszy=new Vector2(2560,2048); 
            pozycjaCelownika =  new Vector2(rozmiarPlanszy.X / 2, rozmiarPlanszy.Y / 2); //celownik na środku
        }
    }
}
