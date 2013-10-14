using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using System.Text;

namespace Dark_Avenger_Ver_0._02
{
    enum ItemType { health, shield, unknown, pistol, machineGun, shotgun, laser, rocketLauncher, grenades };
    class Item : Sprite
    {
        public float timeOfExistence, lockTime;
        public ItemType typItemu;
        public int info1, info2;
        TextSprite about = new TextSprite();
        public Item(Vector2 miejsce, float czasPierwszy, float czasZatrzymania, ItemType coTo, int inf1, int inf2)
        {
            pozycja = miejsce;
            timeOfExistence = czasPierwszy;
            lockTime = czasZatrzymania;
            typItemu = coTo;
            info1 = inf1; info2 = inf2;
            nrWarstwy = 2;
            if (coTo == ItemType.grenades)
                nazwa = "Grenades";
            if (coTo == ItemType.health)
                nazwa = "Health";
            if (coTo == ItemType.laser)
                nazwa = "Laser Gun";
            if (coTo == ItemType.machineGun)
                nazwa = "Machine Gun";
            if (coTo == ItemType.pistol)
                nazwa = "Pistol";
            if (coTo == ItemType.rocketLauncher)
                nazwa = "Rocket Launcher";
            if (coTo == ItemType.shield)
                nazwa = "Shield";
            if (coTo == ItemType.shotgun)
                nazwa = "Shotgun";
            typ = Type.item;
            wielkoscObiektu = new Vector2(1, 1);
        }
        public override void Initialize()
        {
            
            
        }
        public override void LoadContent(ContentManager content)
        {
            if(typItemu==ItemType.health)
                teksturaPodstawowa = content.Load<Texture2D>("health_item");
            if (typItemu == ItemType.shield)
                teksturaPodstawowa = content.Load<Texture2D>("shield_item");
            if (typItemu != ItemType.health&&typItemu!=ItemType.shield)
                teksturaPodstawowa = content.Load<Texture2D>("gun_item");
            about.LoadContent(content);
        }
        public override void Draw(SpriteBatch sprite)
        {
            if (Config.statusGry != GameState.menu)
            {
                if (Camera.CzyWWidoku(this))
                    Rysuj(sprite, teksturaPodstawowa, Camera.PozycjaWgKamery(pozycja), wielkoscObiektu);
                if (czySieWyswietlac)
                    about.Draw(sprite, nazwa, 20, Camera.PozycjaWgKamery(new Vector2(pozycja.X, pozycja.Y - teksturaPodstawowa.Height)));
            }
        }
        public override void Update(GameTime time)
        {
            if (Config.statusGry == GameState.inProgress)
            {
                timeOfExistence -= (float)time.ElapsedGameTime.TotalSeconds;
                lockTime -= (float)time.ElapsedGameTime.TotalSeconds;
                if (timeOfExistence < 0)
                    stan = State.toDelete;
            }
            czasWyswietlania -= (float)time.ElapsedGameTime.TotalSeconds;
            if (czasWyswietlania < 0)
                czySieWyswietlac = false;
        }
        public override void PrzyjmijUderzenie(float obrazenia)
        {
            stan = State.toDelete;
        }
    }
}
