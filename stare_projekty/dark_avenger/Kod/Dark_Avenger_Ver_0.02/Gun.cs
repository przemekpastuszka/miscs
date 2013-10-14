using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    enum Action { strzelam, laduje, oczekuje };
    class Gun : Sprite
    {
        Action akcja;
        Item glowny;
        Texture2D naboj;
        Texture2D brakNaboju;
        TextSprite magazynek=new TextSprite();
        float czasPrzeladowania, czasStrzalu, deltaPrzeladowania=0, deltaStrzalu=0;
        int pojemnoscMagazynka;
        public Gun(Item a, Vector2 b)
        {
            glowny = a;
            pozycja = b;
            wielkoscObiektu = new Vector2(1, 1);
            if (glowny.typItemu == ItemType.pistol)
            {
                czasPrzeladowania = 1.5f;
                czasStrzalu = 0.3f;
                pojemnoscMagazynka = 18;
            }
            if (glowny.typItemu == ItemType.machineGun)
            {
                czasPrzeladowania = 2f;
                czasStrzalu = 0.08f;
                pojemnoscMagazynka = 40;
            }
            if (glowny.typItemu == ItemType.shotgun)
            {
                czasPrzeladowania = 3f;
                czasStrzalu = 1f;
                pojemnoscMagazynka = 5;
            }
            if (glowny.typItemu == ItemType.laser)
            {
                czasPrzeladowania = 7f;
                czasStrzalu = 1f;
                pojemnoscMagazynka = 4;
            }
            if (glowny.typItemu == ItemType.grenades)
            {
                czasPrzeladowania = 1.5f;
                czasStrzalu = 0.5f;
                pojemnoscMagazynka = 8;
            }
            if (glowny.typItemu == ItemType.rocketLauncher)
            {
                czasPrzeladowania = 1f;
                czasStrzalu = 1f;
                pojemnoscMagazynka = 1;
            }
            akcja = Action.oczekuje;
        }
        public override void LoadContent(ContentManager content)
        {
            if (glowny.typItemu == ItemType.unknown)
            {
                teksturaPodstawowa = content.Load<Texture2D>("unknown");
                naboj = content.Load<Texture2D>("unknown");
            }
            if (glowny.typItemu == ItemType.pistol)
                teksturaPodstawowa = content.Load<Texture2D>("pistol");
            if (glowny.typItemu == ItemType.machineGun)
                teksturaPodstawowa = content.Load<Texture2D>("machine_gun");
            if (glowny.typItemu == ItemType.shotgun)
                teksturaPodstawowa = content.Load<Texture2D>("shotgun");
            if (glowny.typItemu == ItemType.pistol || glowny.typItemu == ItemType.machineGun || glowny.typItemu == ItemType.shotgun)
            {
                naboj = content.Load<Texture2D>("bullet");
                brakNaboju = content.Load<Texture2D>("no_bullet");
            }
            if (glowny.typItemu == ItemType.laser)
            {
                teksturaPodstawowa = content.Load<Texture2D>("laser");
                naboj = content.Load<Texture2D>("laser_bullet");
                brakNaboju = content.Load<Texture2D>("laser_noBullet");
            }
            if (glowny.typItemu == ItemType.grenades)
            {
                teksturaPodstawowa = content.Load<Texture2D>("grenade");
                naboj = content.Load<Texture2D>("little_grenade");
                brakNaboju = content.Load<Texture2D>("no_grenade");
            }
            if (glowny.typItemu == ItemType.rocketLauncher)
            {
                teksturaPodstawowa = content.Load<Texture2D>("bazooka");
                naboj = content.Load<Texture2D>("rocket");
                brakNaboju = content.Load<Texture2D>("no_rocket");
            }
            magazynek.LoadContent(content);
        }
        public override void Draw(SpriteBatch sprite)
        {
            if (Config.statusGry != GameState.menu)
            {
                Rysuj(sprite, teksturaPodstawowa, new Vector2(740, pozycja.Y + teksturaPodstawowa.Height / 2), wielkoscObiektu);
                if (glowny.typItemu != ItemType.rocketLauncher)
                    pozycja.X = 810;
                else
                    pozycja.X = 910;
                if (glowny.typItemu == ItemType.unknown)
                {
                    for (int i = 0; i < 10; i++)
                    {
                        Rysuj(sprite, naboj, new Vector2(pozycja.X, pozycja.Y + naboj.Height / 4 + 5), new Vector2(0.5f, 0.5f));
                        pozycja.X += naboj.Width / 2 + 2;
                    }
                }
                else
                {
                    for (int i = 0; i < glowny.info1; i++)
                    {
                        Rysuj(sprite, naboj, new Vector2(pozycja.X, pozycja.Y + naboj.Height / 2 + 5), wielkoscObiektu);
                        pozycja.X += naboj.Width + 2;
                    }
                    for (int i = glowny.info1; i < pojemnoscMagazynka; i++)
                    {
                        Rysuj(sprite, brakNaboju, new Vector2(pozycja.X, pozycja.Y + naboj.Height / 2 + 5), wielkoscObiektu);
                        pozycja.X += naboj.Width + 2;
                    }
                }
                if (glowny.typItemu != ItemType.pistol)
                {
                    if (glowny.info2 < 10)
                        magazynek.Draw(sprite, "0" + glowny.info2.ToString(), 25, new Vector2(1200, pozycja.Y + naboj.Height / 2));
                    else
                        magazynek.Draw(sprite, glowny.info2.ToString(), 25, new Vector2(1200, pozycja.Y + naboj.Height / 2));
                }
                else
                    magazynek.Draw(sprite, "--", 25, new Vector2(1200, pozycja.Y + naboj.Height / 2));
            }
        }
        public ItemType ZwrocTyp()
        {
            return glowny.typItemu;
        }
        public void DodajAmunicje(Item a)
        {
            glowny.info2 += a.info2;
        }
        public void WyrzucSie()
        {
            glowny.lockTime = 1.5f;
            glowny.stan = State.alive;
            if (glowny.typItemu == ItemType.pistol)
                glowny.timeOfExistence = 32000;
            else
                glowny.timeOfExistence = 15;
            glowny.pozycja = Config.pozycjaGracza;
            Config.noweObiekty.Add(glowny);
        }
        public void Strzelaj()
        {
            if (akcja == Action.oczekuje)
            {
                if (glowny.info1 != 0)
                {
                    akcja = Action.strzelam;
                    deltaStrzalu = czasStrzalu;
                    glowny.info1--;
                    if (glowny.typItemu == ItemType.pistol || glowny.typItemu == ItemType.machineGun)
                        Config.noweObiekty.Add(new Bullet(50, 0));
                    if (glowny.typItemu == ItemType.shotgun)
                    {
                        Config.noweObiekty.Add(new Bullet(100, -0.1f));
                        Config.noweObiekty.Add(new Bullet(100, 0));
                        Config.noweObiekty.Add(new Bullet(100, 0.1f));
                    }
                    if (glowny.typItemu == ItemType.laser)
                        Config.noweObiekty.Add(new Ray());
                    if (glowny.typItemu == ItemType.grenades)
                        Config.noweObiekty.Add(new Grenade());
                    if (glowny.typItemu == ItemType.rocketLauncher)
                        Config.noweObiekty.Add(new Rocket());
                }
            }
        }
        public override void Update(GameTime time)
        {
            if (Config.statusGry == GameState.inProgress)
            {
                if (akcja != Action.laduje && glowny.info1 == 0)
                {
                    deltaPrzeladowania = czasPrzeladowania;
                    akcja = Action.laduje;
                }
                if (akcja == Action.strzelam)
                {
                    deltaStrzalu -= (float)time.ElapsedGameTime.TotalSeconds;
                    if (deltaStrzalu < 0)
                        akcja = Action.oczekuje;
                }
                if (akcja == Action.laduje)
                {
                    deltaPrzeladowania -= (float)time.ElapsedGameTime.TotalSeconds;
                    if (deltaPrzeladowania < 0)
                    {
                        if (glowny.info2 > 0 || glowny.typItemu == ItemType.pistol)
                        {
                            glowny.info1 = pojemnoscMagazynka;
                            glowny.info2--;
                            akcja = Action.oczekuje;
                        }
                    }
                }
            }
        }
    }
}
