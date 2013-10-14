using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;

namespace Dark_Avenger_Ver_0._02
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        List<Sprite> obiekty = new List<Sprite>();
        TextSprite wynik = new TextSprite();
        ElementMenu[] main = new ElementMenu[4];
        ElementMenu[] difficulty = new ElementMenu[3];
        ElementMenu[] about = new ElementMenu[2];
        ElementMenu[] resolution = new ElementMenu[6];
        float czasProdukcji,timeOfMaking,timeOfBirth=4;
        int dodatkoweMonstery = 0;
        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
           // graphics.IsFullScreen = true;
            
        }
        private void UstawRozdzielczosc(Vector2 ustaw)
        {
            Config.rozdzielczosc.X = graphics.PreferredBackBufferWidth = (int)ustaw.X;
            Config.rozdzielczosc.Y = graphics.PreferredBackBufferHeight = (int)ustaw.Y;
            graphics.ApplyChanges();
        }
        private void FabrykaMonsterow(GameTime time)
        {
            timeOfBirth -= (float)time.ElapsedGameTime.TotalSeconds;
            if (timeOfBirth < 0)
            {
                timeOfBirth = 10;
                int ile = Config.nadwornyRandomizer.Next(2 + dodatkoweMonstery, 4 + dodatkoweMonstery);
                for (int i = 0; i < ile; i++)
                {
                    float wielkosc = 1f / Config.nadwornyRandomizer.Next(1, 4);
                    Monster buu = new Monster(wielkosc, (int)((0.6 / wielkosc) * 250), (int)(wielkosc * 60), (int)(wielkosc * 200));
                    buu.LoadContent(Content);
                    obiekty.Add(buu);
                }
            }
            timeOfMaking -= (float)time.ElapsedGameTime.TotalSeconds;
            if (timeOfMaking < 0)
            {
                timeOfMaking = czasProdukcji;
                dodatkoweMonstery++;
            }
        }
        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here
            Inicjalizacja();
            Config.statusGry = GameState.menu;
            UstawRozdzielczosc(new Vector2(1024, 768));
            main[0] = new ElementMenu(this,"Play", new ElementMenu.Funkcja(Buu), new Vector2(640, 300));
            main[1] = new ElementMenu(this,"Resolution", new ElementMenu.Funkcja(Buu), new Vector2(640, 400));
            main[2] = new ElementMenu(this,"About", new ElementMenu.Funkcja(Buu), new Vector2(640, 500));
            main[3]=new ElementMenu(this,"Exit", new ElementMenu.Funkcja(Exit), new Vector2(640, 600));
            about[0] = new ElementMenu(this, "Dark Avenger ver. 0.02 \n Coded by Przemek Pastuszka", new ElementMenu.Funkcja(Pusta), new Vector2(640, 300));
            about[1] = new ElementMenu(this, "Back", new ElementMenu.Funkcja(Back), new Vector2(640, 500));
            resolution[0] = new ElementMenu(this, "800x600", new ElementMenu.Funkcja(Rozdzielczosc), new Vector2(640, 300));
            resolution[1] = new ElementMenu(this, "1024x768", new ElementMenu.Funkcja(Rozdzielczosc), new Vector2(640, 400));
            resolution[2] = new ElementMenu(this, "1280x800", new ElementMenu.Funkcja(Rozdzielczosc), new Vector2(640, 500));
            resolution[3] = new ElementMenu(this, "1280x1024", new ElementMenu.Funkcja(Rozdzielczosc), new Vector2(640, 600));
            resolution[4] = new ElementMenu(this, "Fullscreen", new ElementMenu.Funkcja(Fullscreen), new Vector2(640, 700));
            resolution[5] = new ElementMenu(this, "Back", new ElementMenu.Funkcja(Back), new Vector2(640, 800));
            difficulty[0] = new ElementMenu(this, "Easy", new ElementMenu.Funkcja(Trudnosc), new Vector2(640, 300));
            difficulty[1] = new ElementMenu(this, "Medium", new ElementMenu.Funkcja(Trudnosc), new Vector2(640, 400));
            difficulty[2] = new ElementMenu(this, "Hard", new ElementMenu.Funkcja(Trudnosc), new Vector2(640, 500));
            
            base.Initialize();
        }
        private void Inicjalizacja()
        {
            obiekty.Clear();
            Config.Inicjalizuj();
            obiekty.Add(new Player());
            obiekty.Add(new Map());
            obiekty.Add(new Sight());
            obiekty.Add(new Item(Config.rozmiarPlanszy / 2, 15, 0, ItemType.pistol, 18, 8));
            //obiekty.Add(new Item(new Vector2(1100, 1200), 15, 0, ItemType.rocketLauncher, 1, 0));
            //obiekty.Add(new Item(new Vector2(1400, 1500), 15, 0, ItemType.shield, 5, 10));
            foreach (Sprite a in obiekty)
            {
                a.Initialize();
            }
            foreach (Sprite a in obiekty)
            {
                a.LoadContent(Content);
            }
            if (Config.trudnosc == Difficulty.easy)
                czasProdukcji = 60;
            if (Config.trudnosc == Difficulty.medium)
            {
                czasProdukcji = 50;
                dodatkoweMonstery = 1;
            }
            if (Config.trudnosc == Difficulty.hard)
            {
                dodatkoweMonstery = 2;
                czasProdukcji = 40;
            }
            timeOfMaking = czasProdukcji;
            timeOfBirth = 0;
        }
        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);
            wynik.LoadContent(Content);
            foreach (ElementMenu a in main)
                a.LoadContent(Content);
            foreach (ElementMenu a in about)
                a.LoadContent(Content);
            foreach (ElementMenu a in resolution)
                a.LoadContent(Content);
            foreach (ElementMenu a in difficulty)
                a.LoadContent(Content);
            
            // TODO: use this.Content to load your game content here
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            if (Config.statusGry != GameState.menu && Klawisze.CzyNacisniety(Keys.Escape))
            {
                Config.statusGry = GameState.menu;
                Config.menu = Menusy.main;
            }
            if (Config.statusGry == GameState.menu && Config.menu != Menusy.main && Klawisze.CzyNacisniety(Keys.Escape))
                Config.menu = Menusy.main;
            if (Config.statusGry == GameState.menu && Config.menu == Menusy.main && Klawisze.CzyNacisniety(Keys.Escape))
                this.Exit();
            Klawisze.Update();
            if (Config.statusGry == GameState.inProgress && Klawisze.CzyNacisniety(Keys.Space))
                Config.statusGry = GameState.paused;
            if (Config.statusGry == GameState.paused && Klawisze.CzyNacisniety(Keys.Space))
                Config.statusGry = GameState.inProgress;
            Camera.Update(gameTime);
            foreach (Sprite a in obiekty)
            {
                a.Update(gameTime);
            }
            if (Config.statusGry == GameState.inProgress)
                FabrykaMonsterow(gameTime);
            if (Config.statusGry == GameState.menu)
            {
                if (Config.menu == Menusy.main)
                    foreach (ElementMenu a in main)
                        a.Update();
                if (Config.menu == Menusy.about)
                    foreach (ElementMenu a in about)
                        a.Update();
                if (Config.menu == Menusy.difficulty)
                    foreach (ElementMenu a in difficulty)
                        a.Update();
                if (Config.menu == Menusy.resolution)
                    foreach (ElementMenu a in resolution)
                        a.Update();
            }
            if (Config.statusGry != GameState.menu)
            {
                
                for (int i = 0; i < obiekty.Count; )
                {
                    if (obiekty[i].stan == State.toDelete)
                        obiekty.RemoveAt(i);
                    else
                        i++;
                }
                foreach (Sprite a in obiekty)
                {
                    foreach (Sprite b in obiekty)
                    {
                        if (a != b)
                        {
                            a.CollisionCheck(b);
                        }
                    }
                }
                foreach (Sprite a in Config.noweObiekty)
                {
                    a.LoadContent(Content);
                    obiekty.Add(a);
                }
                Config.noweObiekty.Clear();
            }
            // TODO: Add your update logic here

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Black);
            spriteBatch.Begin();
            if (Config.statusGry == GameState.menu)
            {
                if (Config.menu == Menusy.main)
                    foreach (ElementMenu a in main)
                        a.Draw(spriteBatch);
                if (Config.menu == Menusy.about)
                    foreach (ElementMenu a in about)
                        a.Draw(spriteBatch);
                if (Config.menu == Menusy.difficulty)
                    foreach (ElementMenu a in difficulty)
                        a.Draw(spriteBatch);
                if (Config.menu == Menusy.resolution)
                    foreach (ElementMenu a in resolution)
                        a.Draw(spriteBatch);
            }
            for(int i=1;i<=6;i++)
                foreach (Sprite a in obiekty)
                {
                    if(a.nrWarstwy==i)
                        a.Draw(spriteBatch);
                }
            if (Config.statusGry != GameState.menu)
            {
                if (Config.wynik < 10)
                    wynik.Draw(spriteBatch, "0" + Config.wynik.ToString(), 50, new Vector2(1220, 990));
                else
                    wynik.Draw(spriteBatch, Config.wynik.ToString(), 50, new Vector2(1220, 990));
            }
            if (Config.statusGry == GameState.ended)
                wynik.Draw(spriteBatch, "Game Over\nYour score is: " + Config.wynik.ToString() + "\nPress Esc to Continue", 50, new Vector2(640, 512));
            spriteBatch.End();
            // TODO: Add your drawing code here

            base.Draw(gameTime);
        }
        public static void Buu(String a,Game1 dawaj) 
        {
            switch (a)
            {
                case "Play":
                    Config.menu = Menusy.difficulty;
                    break;
                case "Resolution":
                    Config.menu = Menusy.resolution;
                    break;
                case "About":
                    Config.menu = Menusy.about;
                    break;
            }
        }
        public static void Trudnosc(String a, Game1 dawaj)
        {
            switch (a)
            {
                case "Easy":
                    Config.trudnosc = Difficulty.easy;               
                    break;
                case "Medium":
                    Config.trudnosc = Difficulty.medium;
                    break;
                case "Hard":
                    Config.trudnosc = Difficulty.hard;
                    break;
            }
            Config.statusGry = GameState.inProgress;
            dawaj.Inicjalizacja();
        }
        public static void Rozdzielczosc(String a, Game1 dawaj)
        {
            switch (a)
            {
                case "800x600":
                    dawaj.UstawRozdzielczosc(new Vector2(800, 600));
                    break;
                case "1024x768":
                    dawaj.UstawRozdzielczosc(new Vector2(1024, 768));
                    break;
                case "1280x800":
                    dawaj.UstawRozdzielczosc(new Vector2(1280, 800));
                    break;
                case "1280x1024":
                    dawaj.UstawRozdzielczosc(new Vector2(1280, 1024));
                    break;
            }
        }
        public static void Exit(String a, Game1 dawaj)
        {
            dawaj.Exit();
        }
        public static void Back(String a, Game1 dawaj)
        {
            Config.menu = Menusy.main;
        }
        public static void Fullscreen(String a, Game1 dawaj)
        {
            if (dawaj.graphics.IsFullScreen)
                dawaj.graphics.IsFullScreen = false;
            else
                dawaj.graphics.IsFullScreen = true;
            dawaj.graphics.ApplyChanges();
        }
        public static void Pusta(String a, Game1 dawaj) { }
    }
}
