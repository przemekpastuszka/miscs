using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    enum MonsterState { attack, move };
    class Monster : Sprite
    {
        MonsterState monsterowy = MonsterState.move;
        Texture2D teksturaSmierci;
        float zycie, timeofExistence = 15,timeOfAttack=1;
        int predkosc,obrazenia;
        public Monster(float skala, int pr, int sila, int life)
        {
            wielkoscObiektu = new Vector2(skala, skala);
            zycie = life;
            predkosc = pr;
            obrazenia = sila;
            typ = Type.monster;
            stan = State.alive;
            nrWarstwy = 3;
            if (Config.nadwornyRandomizer.Next(0, 10) < 5)
            {
                if (Config.nadwornyRandomizer.Next(0, 10) < 5)
                {
                    pozycja.X = -700;
                    pozycja.Y = Config.nadwornyRandomizer.Next(0, (int)Config.rozmiarPlanszy.Y);
                }
                else
                {
                    pozycja.X = Config.rozmiarPlanszy.X+740;
                    pozycja.Y = Config.nadwornyRandomizer.Next(0, (int)Config.rozmiarPlanszy.Y);
                }
            }
            else
            {
                if (Config.nadwornyRandomizer.Next(0, 10) < 5)
                {
                    pozycja.Y = -700;
                    pozycja.X = Config.nadwornyRandomizer.Next(0, (int)Config.rozmiarPlanszy.X);
                }
                else
                {
                    pozycja.Y = Config.rozmiarPlanszy.Y+700;
                    pozycja.X = Config.nadwornyRandomizer.Next(0, (int)Config.rozmiarPlanszy.X);
                }
            }
        }
        public override void Initialize()
        {
        }
        public override void LoadContent(ContentManager content)
        {
            teksturaPodstawowa = content.Load<Texture2D>("monster");
            teksturaSmierci = content.Load<Texture2D>("dead");
        }
        public override void Draw(SpriteBatch sprite)
        {
            if (Config.statusGry != GameState.menu)
            {
                if (stan == State.alive)
                    Rysuj(sprite, teksturaPodstawowa, Camera.PozycjaWgKamery(pozycja), wielkoscObiektu);
                else
                    Rysuj(sprite, teksturaSmierci, Camera.PozycjaWgKamery(pozycja), wielkoscObiektu);
            }
        }
        public override void PrzyjmijUderzenie(float obrazenia)
        {
            zycie -= obrazenia;
            if (zycie < 0)
            {
                stan = State.dead;
                nrWarstwy = 1;
                Config.wynik++;
                if (Config.nadwornyRandomizer.Next(1, 100) < 12)
                {
                    stan = State.toDelete;
                    int wynik = Config.nadwornyRandomizer.Next(1, 100);
                    if (wynik <= 25)
                        Config.noweObiekty.Add(new Item(pozycja, 15, 0, ItemType.shotgun,
                            Config.nadwornyRandomizer.Next(1, 5), Config.nadwornyRandomizer.Next(2, 5)));
                    if (wynik >25&&wynik<=45)
                        Config.noweObiekty.Add(new Item(pozycja, 15, 0, ItemType.machineGun,
                            Config.nadwornyRandomizer.Next(10, 40), Config.nadwornyRandomizer.Next(2, 10)));
                    if (wynik > 45 && wynik <= 60)
                        Config.noweObiekty.Add(new Item(pozycja, 15, 0, ItemType.health,
                            Config.nadwornyRandomizer.Next(1, 4)*25, 0));
                    if (wynik > 60 && wynik <= 70)
                        Config.noweObiekty.Add(new Item(pozycja, 15, 0, ItemType.shield,
                            0, 0));
                    if (wynik > 70 && wynik <= 80)
                        Config.noweObiekty.Add(new Item(pozycja, 15, 0, ItemType.grenades,
                            Config.nadwornyRandomizer.Next(1,8), 0));
                    if (wynik > 80 && wynik <= 90)
                        Config.noweObiekty.Add(new Item(pozycja, 15, 0, ItemType.rocketLauncher,
                            1, 0));
                    if (wynik > 90 && wynik <= 95)
                        Config.noweObiekty.Add(new Item(pozycja, 15, 0, ItemType.laser,
                            Config.nadwornyRandomizer.Next(1, 4), Config.nadwornyRandomizer.Next(1, 3)));
                    if (wynik >95)
                        Config.noweObiekty.Add(new Item(pozycja, 15, 0, ItemType.pistol,
                            Config.nadwornyRandomizer.Next(1, 18), 0));
                }
            }
        }
        public override void CollisionCheck(Sprite a)
        {
            if (stan==State.alive&&a.typ == Type.player&&monsterowy==MonsterState.move&&Obliczarka.KolizjaDlaOkregow(a,this))
            {
                monsterowy = MonsterState.attack;
                timeOfAttack = 1;
                a.PrzyjmijUderzenie(obrazenia);
            }
        }
        public override void Update(GameTime time)
        {
            if (Config.statusGry == GameState.inProgress)
            {
                if (stan == State.alive)
                {
                    if (monsterowy == MonsterState.move)
                        pozycja = Obliczarka.PunktDocelowy(pozycja, Config.pozycjaGracza, predkosc * (float)time.ElapsedGameTime.TotalSeconds);
                    else
                    {
                        timeOfAttack -= (float)time.ElapsedGameTime.TotalSeconds;
                        if (timeOfAttack < 0)
                            monsterowy = MonsterState.move;
                    }
                }
                else
                {
                    timeofExistence -= (float)time.ElapsedGameTime.TotalSeconds;
                    if (timeofExistence < 0)
                    {
                        stan = State.toDelete;
                    }
                }
            }
        }
    }
}
