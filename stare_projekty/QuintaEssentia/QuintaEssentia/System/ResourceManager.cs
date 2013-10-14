/* Quinta Essentia by Przemysław Pastuszka
 * file:ResourceManager.cs
 * Contains: Class Resource Manager
 * ResourceManager: class is used to load resources (thanks to it each resource will be loaded only once)
 * Last modification:24-07-2009
 */
using System;
using System.Drawing;
using System.Collections.Generic;
using D3D10 = SlimDX.Direct3D10;

namespace QuintaEssentia
{ 
    class ResourceManager
    {
        //variables
        private Dictionary<String,D3D10.Texture2D> textures; //collection of textures
        private Dictionary<String, Bitmap> bitmaps; //collection of bitmaps (if we want to change them on CPU)
        private Dictionary<String, D3D10.Effect> effects;
        private Dictionary<String, D3D10.Font> fonts;
        private D3D10.EffectPool defaultEffectPool; //no more effect pools at this time
        public static ResourceManager mainManager;

        //Creation methods
        /// <summary>
        /// Private constructor (class is a singleton)
        /// </summary>
        private ResourceManager()
        {
            textures = new Dictionary<String, D3D10.Texture2D>();
            bitmaps = new Dictionary<string, Bitmap>();
            effects = new Dictionary<string, SlimDX.Direct3D10.Effect>();
            fonts=new Dictionary<string,SlimDX.Direct3D10.Font>();
            //loading default effect pool (which as far provides viewProjection and lightDir constants)
            String error = "";
            defaultEffectPool = D3D10.EffectPool.FromFile(Game.gameClass.GetDevice(), "Resources\\Effects\\EffectPoolDefault.fx", "fx_4_0",
                D3D10.ShaderFlags.Debug, D3D10.EffectFlags.None,out error);
        }
        public static void Create()
        {
            if (mainManager == null)
                mainManager = new ResourceManager();
        }
      
        public D3D10.EffectPool GetDefaultEffectPool()
        {
            return defaultEffectPool;
        }

        //Loading functions
        /// <summary>
        /// It allows to load a texture (each will be loaded once, no matter how many times this function will be called)
        /// </summary>
        /// <param name="name">Path of the texture</param>
        /// <returns>Loaded texture</returns>
        public D3D10.Texture2D LoadTexture(String name)
        {
            if (textures.ContainsKey(name))
                return textures[name];
            D3D10.Texture2D temp = D3D10.Texture2D.FromFile(Game.gameClass.GetDevice(), name);
            textures.Add(name, temp);
            return temp;
        }
        public D3D10.Texture2D AddToTexture2D(String name, D3D10.Texture2D file, int nr ,int maxTextures)
        {
            D3D10.Texture2D temp;
            if (textures.ContainsKey(name))
                temp = textures[name];
            else
            {
                D3D10.Texture2DDescription desc = new SlimDX.Direct3D10.Texture2DDescription();
                desc.ArraySize = maxTextures;
                desc.BindFlags = D3D10.BindFlags.ShaderResource;
                desc.CpuAccessFlags = D3D10.CpuAccessFlags.None;
                desc.Format = file.Description.Format;
                desc.Height = file.Description.Height;
                desc.Width = file.Description.Width;
                desc.MipLevels = file.Description.MipLevels;
                desc.OptionFlags = D3D10.ResourceOptionFlags.None;
                SlimDX.DXGI.SampleDescription sampleDesc = new SlimDX.DXGI.SampleDescription();
                sampleDesc.Count = 1;
                sampleDesc.Quality = 0;
                desc.SampleDescription = sampleDesc;
                desc.Usage = D3D10.ResourceUsage.Default;
                temp = new SlimDX.Direct3D10.Texture2D(Game.gameClass.GetDevice(), desc);
                textures.Add(name, temp);
            }
            D3D10.ResourceRegion region = new SlimDX.Direct3D10.ResourceRegion();
            region.Back = 1;
            region.Front = 0;
            region.Left = region.Top = 0;
            region.Right = file.Description.Width;
            region.Bottom = file.Description.Height;
            for (int i = 0, q = 1; i < file.Description.MipLevels; i++, q *= 2)
            {
                region.Right /= q;
                region.Bottom /= q;
                Game.gameClass.GetDevice().CopySubresourceRegion(file, i, region, temp, i + (nr * file.Description.MipLevels), 0, 0, 0);
            }
            return temp;
        }
        /// <summary>
        /// Allows to load a bitmap. The main difference between Bitmap and Texture is that Bitmap can be easily
        /// manipulated by CPU, when Textures are designed as shader resources for GPU usage
        /// </summary>
        /// <param name="name">Path of the bitmap</param>
        /// <returns>Loaded bitmap</returns>
        public Bitmap LoadBitmap(String name)
        {
            if (bitmaps.ContainsKey(name))
                return bitmaps[name];
            Bitmap temp = new Bitmap(name);
            bitmaps.Add(name,temp);
            return temp;
        }
        /// <summary>
        /// Loads effect
        /// </summary>
        /// <param name="name">Effect name</param>
        /// <param name="pool">If effect is to be a 'child effect' then recived 'pool' variable
        /// will be used as a parent. In other case type 'null'</param>
        /// <returns>Desired effect</returns>
        public D3D10.Effect LoadEffect(String name, D3D10.EffectPool pool)
        {
            if (effects.ContainsKey(name))
                return effects[name];
            string error="";
            D3D10.Effect temp;
            if(pool==null)
                temp = D3D10.Effect.FromFile(Game.gameClass.GetDevice(), name, "fx_4_0", SlimDX.Direct3D10.ShaderFlags.Debug, SlimDX.Direct3D10.EffectFlags.None, null, null, out error);
            else
                temp = D3D10.Effect.FromFile(Game.gameClass.GetDevice(), name, "fx_4_0", SlimDX.Direct3D10.ShaderFlags.Debug, SlimDX.Direct3D10.EffectFlags.ChildEffect, pool, null, out error);
            effects.Add(name, temp);
            
            return temp;
        }
        public D3D10.Font LoadFont(String name, D3D10.FontDescription desc)
        {
            if (fonts.ContainsKey(name))
                return fonts[name];
            D3D10.Font temp = new SlimDX.Direct3D10.Font(Game.gameClass.GetDevice(), desc);
            fonts.Add(name,temp);
            return temp;
        }

        public void Dispose()
        {
            foreach (D3D10.Texture2D a in textures.Values)
                if(a!=null)
                    a.Dispose();
            foreach (Bitmap a in bitmaps.Values)
                if(a!=null)
                    a.Dispose();
            foreach (D3D10.Effect a in effects.Values)
                if (a != null)
                    a.Dispose();
            foreach (D3D10.Font a in fonts.Values)
                if (a != null)
                    a.Dispose();
            if (defaultEffectPool != null)
                defaultEffectPool.Dispose();
        }
    }
}
