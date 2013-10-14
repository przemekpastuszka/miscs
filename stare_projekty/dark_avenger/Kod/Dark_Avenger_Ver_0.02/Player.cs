using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    class Player : Sprite
    {
        float zycie, ochrona;
        //tekstury potrzebne do życia i ochronki
        Texture2D health, health_bar, shield, shield_bar,no_bar;
        int predkosc = 350;
        int przywracanieZycia = 2; //ile hp na sekunde sie odnawia
        Gun jeden, dwa;
        ContentManager zawartosc;
        public override void Initialize() //ustawienie podstawowych wartosci
        {
            typ = Type.player;
            stan = State.alive;
            zycie = 100;
            ochrona = 0;
            pozycja = new Vector2(Config.rozmiarPlanszy.X / 2, Config.rozmiarPlanszy.Y / 2);
            Config.pozycjaGracza = pozycja;
            wielkoscObiektu = new Vector2(1, 1);
            nrWarstwy = 5;
            jeden = new Gun(new Item(new Vector2(), 0, 0, ItemType.unknown, 12, 0), new Vector2(0, 10));
            dwa = new Gun(new Item(new Vector2(), 0, 0, ItemType.unknown, 0, 0), new Vector2(0, 60));
        }
        public override void LoadContent(ContentManager content)
        {
            zawartosc = content;
            teksturaPodstawowa = content.Load<Texture2D>("player");
            health = content.Load<Texture2D>("heart");
            health_bar = content.Load<Texture2D>("health");
            no_bar = content.Load<Texture2D>("no_health");
            shield = content.Load<Texture2D>("shield");
            shield_bar = content.Load<Texture2D>("shield_bar");
            jeden.LoadContent(content);
            dwa.LoadContent(content);
        }
        public override void Draw(SpriteBatch sprite)
        {
            if (Config.statusGry != GameState.menu)
            {
                if (Camera.CzyWWidoku(this))
                    Rysuj(sprite, teksturaPodstawowa, Camera.PozycjaWgKamery(pozycja), wielkoscObiektu);
                Rysuj(sprite, health, new Vector2(10 + health.Width / 2, 10 + health.Height / 2), wielkoscObiektu);
                Rysuj(sprite, no_bar, new Vector2(60 + health_bar.Width / 2, 20 + health_bar.Height / 2), wielkoscObiektu);
                Rysuj(sprite, health_bar, new Vector2(60 + health_bar.Width / 2, 20 + health_bar.Height / 2), new Vector2(zycie / 100f, 1));
                if (ochrona > 0)
                {
                    Rysuj(sprite, shield, new Vector2(10 + health.Width / 2, 60 + health.Height / 2), wielkoscObiektu);
                    Rysuj(sprite, no_bar, new Vector2(60 + health_bar.Width / 2, 72 + health_bar.Height / 2), wielkoscObiektu);
                    Rysuj(sprite, shield_bar, new Vector2(60 + health_bar.Width / 2, 72 + health_bar.Height / 2), new Vector2(ochrona / 100f, 1));
                }
                jeden.Draw(sprite);
                dwa.Draw(sprite);
            }
        }
        public override void Update(GameTime time)
        {
            if (stan == State.alive&&Config.statusGry == GameState.inProgress)
            {
                MakeAMove(time);
                Config.pozycjaGracza = pozycja;
                if (Keyboard.GetState().IsKeyDown(Keys.Q)&&jeden.ZwrocTyp()!=ItemType.unknown)
                {
                    jeden.WyrzucSie();
                    jeden = new Gun(new Item(new Vector2(), 0, 0, ItemType.unknown, 12, 0), new Vector2(0, 10));
                    jeden.LoadContent(zawartosc);
                }
                if (Keyboard.GetState().IsKeyDown(Keys.E) && dwa.ZwrocTyp() != ItemType.unknown)
                {
                    dwa.WyrzucSie();
                    dwa = new Gun(new Item(new Vector2(), 0, 0, ItemType.unknown, 12, 0), new Vector2(0, 60));
                    dwa.LoadContent(zawartosc);
                }
                jeden.Update(time);
                dwa.Update(time);
                if (Mouse.GetState().LeftButton == ButtonState.Pressed)
                    jeden.Strzelaj();
                if (Mouse.GetState().RightButton == ButtonState.Pressed)
                    dwa.Strzelaj();
                if (zycie < 100)
                    zycie += przywracanieZycia * (float)time.ElapsedGameTime.TotalSeconds;
                if (zycie < 0)
                {
                    Config.statusGry = GameState.ended;
                    stan = State.dead;
                }
                
            }
        }
        private void MakeAMove(GameTime time)
        {
            if (Keyboard.GetState().IsKeyDown(Keys.S))
                pozycja.Y += (float)time.ElapsedGameTime.TotalSeconds * predkosc;
            if (Keyboard.GetState().IsKeyDown(Keys.W))
                pozycja.Y -= (float)time.ElapsedGameTime.TotalSeconds * predkosc;
            if (Keyboard.GetState().IsKeyDown(Keys.D))
                pozycja.X += (float)time.ElapsedGameTime.TotalSeconds * predkosc;
            if (Keyboard.GetState().IsKeyDown(Keys.A))
                pozycja.X -= (float)time.ElapsedGameTime.TotalSeconds * predkosc;
            if (pozycja.X + teksturaPodstawowa.Width / 2 > Config.rozmiarPlanszy.X)
                pozycja.X = Config.rozmiarPlanszy.X - teksturaPodstawowa.Width / 2;
            if (pozycja.X - teksturaPodstawowa.Width / 2 < 0)
                pozycja.X = teksturaPodstawowa.Width / 2;
            if (pozycja.Y + teksturaPodstawowa.Height / 2 > Config.rozmiarPlanszy.Y)
                pozycja.Y = Config.rozmiarPlanszy.Y - teksturaPodstawowa.Height / 2;
            if (pozycja.Y - teksturaPodstawowa.Height / 2 < 0)
                pozycja.Y = teksturaPodstawowa.Height / 2;
        }
        public override void CollisionCheck(Sprite a)
        {
            if (Config.statusGry == GameState.inProgress&&Obliczarka.KolizjaDlaOkregow(this, a)&&a.typ==Type.item)
            {
                    Item temp = (Item)a;
                    if (temp.lockTime > 0)
                        return;
                    if (temp.typItemu == ItemType.shield&&ochrona!=100)
                    {
                        ochrona = 100;
                        a.PrzyjmijUderzenie(0);
                    }
                    if (temp.typItemu == ItemType.health && zycie != 100)
                    {
                        zycie = Math.Min(100, zycie + temp.info1);
                        a.PrzyjmijUderzenie(0);
                    }
                    if (temp.typItemu != ItemType.health && temp.typItemu != ItemType.shield)
                    {
                        if (jeden.ZwrocTyp() == ItemType.unknown)
                        {
                            jeden = new Gun(temp, new Vector2(0, 10));
                            a.PrzyjmijUderzenie(0);
                            jeden.LoadContent(zawartosc);
                            return;
                        }
                        if (dwa.ZwrocTyp() == ItemType.unknown)
                        {
                            dwa = new Gun(temp, new Vector2(0, 60));
                            a.PrzyjmijUderzenie(0);
                            dwa.LoadContent(zawartosc);
                            return;
                        }
                        if (jeden.ZwrocTyp() == temp.typItemu)
                        {
                            jeden.DodajAmunicje(temp);
                            a.PrzyjmijUderzenie(0);
                            return;
                        }
                        if (dwa.ZwrocTyp() == temp.typItemu)
                        {
                            dwa.DodajAmunicje(temp);
                            a.PrzyjmijUderzenie(0);
                            return;
                        }
                    }
            }
        }
        public override void PrzyjmijUderzenie(float obrazenia)
        {
            if (ochrona > 0)
            {
                ochrona -= obrazenia;
                if (ochrona < 0)
                    zycie += ochrona;
            }
            else
                zycie -= obrazenia;
        }
    }
}
