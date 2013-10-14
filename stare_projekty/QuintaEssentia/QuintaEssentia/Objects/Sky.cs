using SlimDX;
using D3D10 = SlimDX.Direct3D10;
using System.IO;
using System;
using System.Drawing;
using System.Collections.Generic;
namespace QuintaEssentia
{
    class Sky
    {
        SkyDome dome = new SkyDome();
        D3D10.Effect effect;
        D3D10.EffectPass pass, updatePass;
        D3D10.EffectVectorVariable translation, sunPosition;
        D3D10.InputLayout layout, layoutUpdate;

        Vector2 size = new Vector2(64,128);

        //scattering variables
        float rayleighK, mieK;
        float g;
        float innerRadius = 6356.7523142f, outerRadius, atmScale;
        Vector3 waveLength, cameraPoint=new Vector3(0,6356.7523142f,0);
        float rayPower, miePower, Is;
        float rayleighScaleDepth, mieScaleDepth;

        Vector3 lastLightDir;

        D3D10.EffectVectorVariable mieTemps, K, waveLengthRay, waveLengthMie, scaleDepth;
        D3D10.EffectScalarVariable sunIntensity;

       // float[,] rayDepthLookup = new float[100, 180];
       // float[,] mieDepthLookup = new float[100, 180];

        Viewport viewPort;
        D3D10.RenderTargetView[] renderTargets = new SlimDX.Direct3D10.RenderTargetView[2];

        D3D10.Texture2D rayLookupTexture, mieLookupTexture;
        //D3D10.Texture2D mieUpdateTexture, rayUpdateTexture;
        //DataRectangle mieUpdateTextureMap, rayUpdateTextureMap;

        SkyForm form;

        public Sky(Vector3 lightDir, StreamReader file)
        {
            ///
           // outerRadius = innerRadius * 1.0157313f;
           // atmScale = 1f / (outerRadius - innerRadius);

            dome.CreateDome((int)size.X, (int)size.Y);
           // waveLength4 = new Vector3((float)Math.Pow(waveLength.X, 4), (float)Math.Pow(waveLength.Y, 4), (float)Math.Pow(waveLength.Z, 4));
           // double pow = -0.84;
           // waveLength084 = new Vector3((float)Math.Pow(waveLength.X, pow), (float)Math.Pow(waveLength.Y, pow), (float)Math.Pow(waveLength.Z, pow));

            effect = ResourceManager.mainManager.LoadEffect("Resources\\Effects\\Sky.fx", ResourceManager.mainManager.GetDefaultEffectPool());
            pass = effect.GetTechniqueByName("Render").GetPassByIndex(0);
            updatePass = effect.GetTechniqueByName("Update").GetPassByIndex(0);
            D3D10.InputElement[] elements = new SlimDX.Direct3D10.InputElement[2];
                elements[0] = new SlimDX.Direct3D10.InputElement("POSITION", 0, SlimDX.DXGI.Format.R32G32B32_Float, 0, 0);
                elements[1] = new D3D10.InputElement("TEXCOORD", 0, SlimDX.DXGI.Format.R32G32_Float, 12, 0);
                layout = new SlimDX.Direct3D10.InputLayout(Game.gameClass.GetDevice(), elements, pass.Description.Signature);
                layoutUpdate = Quad.GetLayout(updatePass);
            translation = effect.GetVariableByName("translation").AsVector();
            sunPosition = effect.GetVariableByName("sunPos").AsVector();
           // Vector4 mieTemps = new Vector4((3 * (1 - g * g)) / (2 * (2 + g * g)), 1 + g * g, g, 1);
            mieTemps = effect.GetVariableByName("mieTemps").AsVector();
            K = effect.GetVariableByName("K").AsVector();
            waveLengthRay = effect.GetVariableByName("waveLengthRay").AsVector();
            waveLengthMie = effect.GetVariableByName("waveLengthMie").AsVector();
            scaleDepth = effect.GetVariableByName("scaleDepth").AsVector();
            sunIntensity = effect.GetVariableByName("sunIntensity").AsScalar();
            //textures
            D3D10.Texture2DDescription desc = new SlimDX.Direct3D10.Texture2DDescription();
            desc.ArraySize = 1;
            desc.BindFlags = D3D10.BindFlags.ShaderResource | SlimDX.Direct3D10.BindFlags.RenderTarget;
            desc.CpuAccessFlags = D3D10.CpuAccessFlags.None;
            desc.Format = SlimDX.DXGI.Format.R16G16B16A16_Float;
            desc.Height = (int)size.Y;
            desc.MipLevels = 1;
            desc.OptionFlags = D3D10.ResourceOptionFlags.None;
            desc.Usage = D3D10.ResourceUsage.Default;
            desc.Width = (int)size.X;

            SlimDX.DXGI.SampleDescription sampleDescription = new SlimDX.DXGI.SampleDescription();
            sampleDescription.Count = 1;
            sampleDescription.Quality = 0;
            desc.SampleDescription = sampleDescription;

            rayLookupTexture = new D3D10.Texture2D(Game.gameClass.GetDevice(), desc);
            mieLookupTexture = new D3D10.Texture2D(Game.gameClass.GetDevice(), desc);

           /* desc.BindFlags = D3D10.BindFlags.None;
            desc.CpuAccessFlags = D3D10.CpuAccessFlags.Write;
            desc.Usage = D3D10.ResourceUsage.Staging;

            mieUpdateTexture = new D3D10.Texture2D(Game.gameClass.GetDevice(), desc);
            rayUpdateTexture = new D3D10.Texture2D(Game.gameClass.GetDevice(), desc);

            mieUpdateTextureMap = mieUpdateTexture.Map(0, D3D10.MapMode.Write, D3D10.MapFlags.DoNotWait);
            mieUpdateTexture.Unmap(0);

            rayUpdateTextureMap = rayUpdateTexture.Map(0, D3D10.MapMode.Write, D3D10.MapFlags.DoNotWait);
            rayUpdateTexture.Unmap(0);*/
            using (D3D10.ShaderResourceView view = new SlimDX.Direct3D10.ShaderResourceView(Game.gameClass.GetDevice(), rayLookupTexture))
                effect.GetVariableByName("rayLookupTex").AsResource().SetResource(view);
             using (D3D10.ShaderResourceView view = new SlimDX.Direct3D10.ShaderResourceView(Game.gameClass.GetDevice(), mieLookupTexture))
                effect.GetVariableByName("mieLookupTex").AsResource().SetResource(view);


             viewPort = new Viewport();
             viewPort.Height = (int)size.Y;
             viewPort.Width = (int)size.X;
             viewPort.MaxZ = 1.0f;
             viewPort.MinZ = 0.0f;
             viewPort.X = 0;
             viewPort.Y = 0;

             renderTargets[0] = new SlimDX.Direct3D10.RenderTargetView(Game.gameClass.GetDevice(), rayLookupTexture);
             renderTargets[1] = new SlimDX.Direct3D10.RenderTargetView(Game.gameClass.GetDevice(), mieLookupTexture);

             Load(file);    

             CalculateLookupOnGPU(-lightDir);
        }
        private void Load(StreamReader file)
        {
            rayleighK = (float)Convert.ToDouble(file.ReadLine());
            mieK = (float)Convert.ToDouble(file.ReadLine());
            K.Set(new Vector4(rayleighK, mieK, 1, 1));
            Vector3 vL = new Vector3();
            vL.X = (float)Convert.ToDouble(file.ReadLine());
            vL.Y = (float)Convert.ToDouble(file.ReadLine());
            vL.Z = (float)Convert.ToDouble(file.ReadLine());
            waveLength = new Vector3(vL.X, vL.Y, vL.Z);
            rayPower = (float)Convert.ToDouble(file.ReadLine());
            waveLengthRay.Set(new Vector4((float)Math.Pow(vL.X, rayPower), (float)Math.Pow(vL.Y, rayPower), (float)Math.Pow(vL.Z, rayPower), 1));
            miePower= (float)Convert.ToDouble(file.ReadLine());
            waveLengthMie.Set(new Vector4((float)Math.Pow(vL.X, miePower), (float)Math.Pow(vL.Y, miePower), (float)Math.Pow(vL.Z, miePower), 1));
            rayleighScaleDepth = (float)Convert.ToDouble(file.ReadLine());
            mieScaleDepth = (float)Convert.ToDouble(file.ReadLine());
            scaleDepth.Set(new Vector4(rayleighScaleDepth, mieScaleDepth, 1, 1));
            Is = (float)Convert.ToDouble(file.ReadLine());
            sunIntensity.Set(Is);
            g = (float)Convert.ToDouble(file.ReadLine());
            mieTemps.Set(new Vector4((3 * (1 - g * g)) / (2 * (2 + g * g)), 1 + g * g, g, 1));
        }
        public void MakeForm()
        {
            form = new SkyForm();
            form.trackBar1.Value = (int)(waveLength.X * 1000);
            form.trackBar2.Value = (int)(waveLength.Y * 1000);
            form.trackBar3.Value = (int)(waveLength.Z * 1000);
            form.trackBar6.Value = (int)(rayPower * 100+500);
            form.trackBar5.Value = (int)(miePower * 100 + 500);
            form.trackBar7.Value = (int)(rayleighK * 10000);
            form.trackBar4.Value = (int)(mieK * 10000);
            form.trackBar9.Value = (int)(rayleighScaleDepth * 100);
            form.trackBar8.Value = (int)(mieScaleDepth * 100);
            form.trackBar11.Value = (int)(g * 1000 + 1000);
            form.trackBar10.Value = (int)Is;
            form.trackBar1.ValueChanged += new EventHandler(trackBar1_ValueChanged);
            form.trackBar2.ValueChanged += new EventHandler(trackBar1_ValueChanged);
            form.trackBar3.ValueChanged += new EventHandler(trackBar1_ValueChanged);
            form.trackBar5.ValueChanged += new EventHandler(trackBar1_ValueChanged);
            form.trackBar6.ValueChanged += new EventHandler(trackBar1_ValueChanged);
            form.trackBar7.ValueChanged += new EventHandler(trackBar7_ValueChanged);
            form.trackBar4.ValueChanged += new EventHandler(trackBar7_ValueChanged);
            form.trackBar9.ValueChanged += new EventHandler(trackBar7_ValueChanged);
            form.trackBar8.ValueChanged += new EventHandler(trackBar7_ValueChanged);
            form.trackBar10.ValueChanged += new EventHandler(trackBar7_ValueChanged);
            form.trackBar11.ValueChanged += new EventHandler(trackBar7_ValueChanged);
            ((Editor)Interface.mainInterface.GetModuleByIndex(2)).AddNewForm(form, "Sky form", "");
        }

        void trackBar7_ValueChanged(object sender, EventArgs e)
        {
            K.Set(new Vector4(form.trackBar7.Value / 10000f, form.trackBar4.Value / 10000f, 1, 1));
            scaleDepth.Set(new Vector4(form.trackBar9.Value / 100f, form.trackBar8.Value / 100f, 1, 1));
            sunIntensity.Set(form.trackBar10.Value);
            g = (form.trackBar11.Value-1000)/1000f;
            mieTemps.Set(new Vector4((3 * (1 - g * g)) / (2 * (2 + g * g)), 1 + g * g, g, 1));
            CalculateLookupOnGPU(-lastLightDir);
        }

        void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            Vector3 vL = new Vector3(form.trackBar1.Value / 1000f, form.trackBar2.Value / 1000f, form.trackBar3.Value / 1000f);
            float temp1 = (form.trackBar6.Value - 500) / 100f;
            waveLengthRay.Set(new Vector4((float)Math.Pow(vL.X, temp1), (float)Math.Pow(vL.Y, temp1), (float)Math.Pow(vL.Z, temp1), 1));
            temp1 = (form.trackBar5.Value - 500) / 100f;
            waveLengthMie.Set(new Vector4((float)Math.Pow(vL.X, temp1), (float)Math.Pow(vL.Y, temp1), (float)Math.Pow(vL.Z, temp1), 1));
            CalculateLookupOnGPU(-lastLightDir);
        }
        public void Draw(Vector3 camPos, Vector3 lightDir)
        {
            Game.gameClass.GetDevice().InputAssembler.SetInputLayout(layout);
            Game.gameClass.GetDevice().InputAssembler.SetPrimitiveTopology(SlimDX.Direct3D10.PrimitiveTopology.TriangleList);
            Game.gameClass.GetDevice().InputAssembler.SetVertexBuffers(0, new D3D10.VertexBufferBinding(dome.vertexBuffer,dome.vertexSize,0));
            translation.Set(new Vector4(camPos, 1));
            sunPosition.Set(new Vector4(lightDir, 1));
            pass.Apply();
            Game.gameClass.GetDevice().Draw(dome.length, 0);
            lastLightDir = lightDir;
            
        }
       /* private double OpticalDepth(Vector3 a, Vector3 b, int n, double scale)
        {
            float sectorLen = (b - a).Length()/n;
            Vector3 addRay = (b - a);
            addRay.Normalize();
            addRay*=sectorLen;
            Vector3 start = a + addRay * 0.5f;
            double integral = 0;
            for (int i = 0; i < n; i++)
            {
                integral += Math.Exp(-GetScaledHeight(start) / scale);
                start += addRay;
            }
            integral *= sectorLen * atmScale;
            return integral;
        }
        private double GetScaledHeight(Vector3 point)
        {
            float length = point.Length();
            return (length - innerRadius) * atmScale;
        }
        private Vector3 HitOuterSphere(Vector3 o, Vector3 dir)
        {
            dir.Normalize();
            double B = Vector3.Dot(o, dir);
            double C = Vector3.Dot(o, o) - outerRadius * outerRadius;
            double D =Math.Sqrt( B * B - C);

                return o + dir * (float)(-B + D);
        }
        class Helper
        {
            public Vector3 ray=new Vector3(), mie=new Vector3();
            public Helper(double[] inScatter)
            {
                ray.X = (float)inScatter[0];
                ray.Y = (float)inScatter[1];
                ray.Z = (float)inScatter[2];
                mie.X = (float)inScatter[3];
                mie.Y = (float)inScatter[4];
                mie.Z = (float)inScatter[5];
            }
        }
        private Helper InScattering(Vector3 target, Vector3 sunPos, int nIn, int nOut)
        {
            double[] inScatter=new double[6];
            float sectorLen = (target-cameraPoint).Length() / nIn;
            Vector3 addRay = (target - cameraPoint);
            addRay.Normalize();
            //int cameraAngle=(int)(ToDegrees((float)Math.Acos(Vector3.Dot(Vector3.UnitY, addRay))));
            addRay *= sectorLen;
            Vector3 start = cameraPoint + addRay * 0.5f;    
           // int sunAngle = (int)(ToDegrees((float)Math.Acos(Vector3.Dot(Vector3.UnitY, sunPos))));
            for (int i = 0; i < nIn; i++)
            {   
                Vector3 tempSunPos = HitOuterSphere(start, sunPos);
                double scaledHeight = GetScaledHeight(start);
               // int intAltitude = (int)(scaledHeight - cameraPoint.Y);
                double temp=-4 * Math.PI * rayleighK *
                    (OpticalDepth(start, tempSunPos, nOut, rayleighScaleDepth) + OpticalDepth(cameraPoint, start, nOut, rayleighScaleDepth));
                //double temp=-4 * Math.PI * rayleighK *
               //     (rayleighScaleDepth[intAltitude,sunAngle]+rayleighScaleDepth[0,cameraAngle]-rayleighScaleDepth[intAltitude,cameraAngle]);
                double opticalDepth = Math.Exp(-scaledHeight / rayleighScaleDepth);
                inScatter[0] += opticalDepth * Math.Exp(temp/waveLength4.X);
                inScatter[1] += opticalDepth * Math.Exp(temp / waveLength4.Y);
                inScatter[2] += opticalDepth * Math.Exp(temp / waveLength4.Z);

                temp=-4 * Math.PI * mieK *
                    (OpticalDepth(start, tempSunPos, nOut, mieScaleDepth) + OpticalDepth(cameraPoint, start, nOut, mieScaleDepth));
                opticalDepth=Math.Exp(-scaledHeight / mieScaleDepth);
                inScatter[3] += opticalDepth * Math.Exp(temp/waveLength084.X);
                inScatter[4] += opticalDepth * Math.Exp(temp/waveLength084.Y);
                inScatter[5] += opticalDepth * Math.Exp(temp/waveLength084.Z);
               // inScatter[3] = Math.Exp(-scaledHeight / mieScaleDepth) * Math.Exp(-4 * Math.PI * mieK *
                //    (mieScaleDepth[intAltitude, sunAngle] + mieScaleDepth[0, cameraAngle] - mieScaleDepth[intAltitude, cameraAngle]));
                start += addRay;
            }
            inScatter[0] *= sunIntensity * sectorLen * rayleighK / waveLength4.X*atmScale;
            inScatter[1] *= sunIntensity * sectorLen * rayleighK / waveLength4.Y * atmScale;
            inScatter[2] *= sunIntensity * sectorLen * rayleighK / waveLength4.Z * atmScale;
            inScatter[3] *= sunIntensity * sectorLen * mieK / waveLength084.X * atmScale;
            inScatter[4] *= sunIntensity * sectorLen * mieK / waveLength084.Y * atmScale;
            inScatter[5] *= sunIntensity * sectorLen * mieK / waveLength084.Z * atmScale;
            return new Helper(inScatter);
        }
        public void CalculateLookup(Vector3 sunPos, int nIn, int nOut)
        {
            rayUpdateTextureMap.Data.Position = 0;
            mieUpdateTextureMap.Data.Position = 0;
            for(int i=0;i<size.Y;i++)
                for (int j = 0; j < size.X; j++)
                {
                    Helper temp2 = InScattering(HitOuterSphere(cameraPoint, dome.data[j, i]), sunPos, nIn, nOut);
                    rayUpdateTextureMap.Data.Write<Vector3>(temp2.ray);
                    mieUpdateTextureMap.Data.Write<Vector3>(temp2.mie);
                }
            Game.gameClass.GetDevice().CopyResource(rayUpdateTexture, rayLookupTexture);
            Game.gameClass.GetDevice().CopyResource(mieUpdateTexture, mieLookupTexture);
        }*/
        public void CalculateLookupOnGPU(Vector3 sunPos)
        {
            sunPosition.Set(new Vector4(sunPos,1));
            Game.gameClass.GetDevice().Rasterizer.SetViewports(viewPort);
            Game.gameClass.GetDevice().OutputMerger.SetTargets(renderTargets);
            Game.gameClass.GetDevice().InputAssembler.SetInputLayout(layoutUpdate);
            Game.gameClass.GetDevice().InputAssembler.SetPrimitiveTopology(SlimDX.Direct3D10.PrimitiveTopology.TriangleList);
            Game.gameClass.GetDevice().InputAssembler.SetVertexBuffers(0, Quad.binding);
            updatePass.Apply();

            Game.gameClass.GetDevice().Draw(6, 0);
            
            Game.gameClass.SetDefaultRenderTargets();
        }
      /*  private float ToDegrees(float value)
        {
            return 180 * value / (float)Math.PI;
        }
        private float ToRadians(float value)
        {
            return value * (float)Math.PI / 180;
        }
        private void CreateDepthLookup(int nOut)
        {
            Vector3 point = Vector3.Zero;
            for (int i = 0; i < 100; i++)
            {
                point.Y = cameraPoint.Y + i;
                for (int j = 0; j < 180; j++)
                {
                    Vector3 dir = new Vector3((float)Math.Sin(ToRadians(j)), (float)Math.Cos(ToRadians(j)), 0);
                    Vector3 spherePoint=HitOuterSphere(point, dir);
                    rayDepthLookup[i, j] = (float)OpticalDepth(point, spherePoint, nOut, rayleighScaleDepth);
                    mieDepthLookup[i, j] = (float)OpticalDepth(point, spherePoint, nOut, mieScaleDepth);
                }
            }
        }*/
        public void Dispose()
        {
            if (rayLookupTexture != null)
                rayLookupTexture.Dispose();
            if (mieLookupTexture != null)
                mieLookupTexture.Dispose();
           /* if (rayUpdateTexture != null)
                rayUpdateTexture.Dispose();
            if (mieUpdateTexture != null)
                mieUpdateTexture.Dispose();*/
            if (layoutUpdate != null)
                layoutUpdate.Dispose();
            if (renderTargets != null && renderTargets[0] != null)
                renderTargets[0].Dispose();
            if (renderTargets != null && renderTargets[1] != null)
                renderTargets[1].Dispose();
            if (layout != null)
                layout.Dispose();
            if (dome != null && dome.vertexBuffer != null)
                dome.vertexBuffer.Dispose();
        }
    }
}
