/* Quinta Essentia by Przemysław Pastuszka
 * file:Terrain.cs
 * Contains: Class Terrain
 * Terrain: renders one quad of terrain
 * Last modification:24-07-2009
 */
using SlimDX;
using D3D10 = SlimDX.Direct3D10;
using System.IO;
using System;
using System.Drawing;
using System.Collections.Generic;

namespace QuintaEssentia
{
    class Vertex //it used to represent one vertex of terrain
    {
        public Vector3 position, normal;
        public VertexTexture tex = new VertexTexture();
        public int nr; //nr of vertex in terrain grid
        public Vertex(Vector3 pos, Vector3 norm)
        {
            position = pos; normal = norm;
            nr = vertexNum++;
        }
        public Vertex()
        {
            nr = vertexNum++;
        }
        public static int size = sizeof(float) * 6;
        private static int vertexNum = 0;
    }
    class VertexTexture
    {
        public Vector3 texNumbers = new Vector3(0, 1, 2), blending = new Vector3(1, 0, 0);
        public void Add(int texNr, float blend)
        {
            float min = 1;
            int nr = -1;
            for (int i = 0; i < 3; i++)
            {
                if (texNumbers[i] == texNr)
                {
                    blend += blending[i];
                    if (blend >= 1)
                    {
                        blending.X = blending.Y = blending.Z = 0;
                        blending[i] = 1;
                        return;
                    }
                    min = blending[i];
                    nr = i;
                    break;
                }
                else
                    if (blending[i] < min)
                    {
                        min = blending[i];
                        nr = i;
                    }
            }

            if (blend <= min)
                return;

            float l = (float)Math.Sqrt((1 - blend * blend) / (1 - min * min));
            blending *= l;
            blending[nr] = blend;
            texNumbers[nr] = texNr;
        }
    }
    class TextureInfo
    {
        public String name;
        public static float[] texCoordMul = new float[12];
    }
    class SobelMasks
    {
        static int[,] Sx5 = {{1,2,0,-2,-1},
                                   {4,8,0,-8,-4},
                                   {6, 12, 0, -12, -6},
                                   {4,8,0,-8,-4},
                                   {1,2,0,-2,-1}};
        static int[,] Sy5 ={{-1,-4,-6,-4,-1},
                                  {-2,-8,-12,-8,-2},
                                  {0,0,0,0,0},
                                  {2,8,12,8,2},
                                  {1,4,6,4,1}};

        static int[,] Sx3 = {{-1,0,1},
                           {-2,0,2},
                           {-1,0,1}};
        static int[,] Sy3 = {{-1,-2,-1},
                           {0,0,0},
                           {1,2,1}};
        public static Vector2 Get(int x, int y, bool is5x5)
        {
            if (is5x5)
                return new Vector2(Sx5[x, y], Sy5[x, y]);
            return new Vector2(Sx3[x, y], Sy3[x, y]);
        }

    }
    class Terrain
    {
       
        Vertex[,] map; //terrain grid
        Vector4[] instances;
        int counter = 0;
        //variables
        D3D10.InputLayout layout;
        D3D10.Effect effect;
        D3D10.EffectPass pass, passEdit;
        D3D10.EffectTechnique technique, techniqueEdit;
        D3D10.Buffer vertexBuffer;

        D3D10.EffectVectorVariable mousePick, pickOptions, orientations; //used only for edit mode
        D3D10.EffectScalarVariable heightMul;
        D3D10.Texture2D normalTexture, normalTexUpdater;
        //D3D10.Texture2D textures;
        D3D10.VertexBufferBinding vertexBufferBinding;
     //   TextureInfo[] texInfos;
        TerrainPicker pickerForm;
        NormalForm normalForm;
        TerrainForm form; //only when edit mode is on

        DataRectangle normalData;
        private int quadSize;

        //creation functions
        public Terrain(int q, StreamReader file)
        {
            quadSize = q;
            //loading effect
            effect = ResourceManager.mainManager.LoadEffect("Resources\\Effects\\Terrain.fx", ResourceManager.mainManager.GetDefaultEffectPool());
            technique = effect.GetTechniqueByName("Render");
            pass = technique.GetPassByIndex(0);
            //creating layout
            D3D10.InputElement[] elements = new SlimDX.Direct3D10.InputElement[1];
            elements[0] = new SlimDX.Direct3D10.InputElement("TEXCOORD", 0, SlimDX.DXGI.Format.R32G32_Float, 0, 0, D3D10.InputClassification.PerVertexData, 0);
            layout = new SlimDX.Direct3D10.InputLayout(Game.gameClass.GetDevice(), elements, pass.Description.Signature);

            //loading texture
            D3D10.ImageLoadInformation load = new SlimDX.Direct3D10.ImageLoadInformation();
            load.BindFlags = D3D10.BindFlags.ShaderResource;
            load.CpuAccessFlags = D3D10.CpuAccessFlags.None;
            load.MipLevels = 1;
            load.Usage=D3D10.ResourceUsage.Default;
            load.OptionFlags = D3D10.ResourceOptionFlags.None;
            load.FilterFlags = D3D10.FilterFlags.Point;
            load.Format = SlimDX.DXGI.Format.R8G8B8A8_UNorm;
            normalTexture = D3D10.Texture2D.FromFile(Game.gameClass.GetDevice(), Path.GetDirectoryName(Game.gameClass.GetLvLFilePath()) + "\\normals.png", load);
            Globals.mapSize = normalTexture.Description.Width;
      

            if (Game.gameClass.GetEngineState() != EngineState.play)
            {
                D3D10.Texture2DDescription desc = new SlimDX.Direct3D10.Texture2DDescription();
                desc.ArraySize = 1;
                desc.BindFlags = D3D10.BindFlags.None;
                desc.CpuAccessFlags = D3D10.CpuAccessFlags.Write;
                desc.Format = SlimDX.DXGI.Format.R8G8B8A8_UNorm;
                desc.Height = Globals.mapSize;
                desc.Width = Globals.mapSize;
                desc.Usage = D3D10.ResourceUsage.Staging;
                desc.MipLevels = 1;
                SlimDX.DXGI.SampleDescription sampleDescription = new SlimDX.DXGI.SampleDescription();
                sampleDescription.Count = 1;
                sampleDescription.Quality = 0;
                desc.SampleDescription = sampleDescription;
                normalTexUpdater = new SlimDX.Direct3D10.Texture2D(Game.gameClass.GetDevice(), desc);
                normalData = normalTexUpdater.Map(0, D3D10.MapMode.Write, D3D10.MapFlags.DoNotWait);
                normalTexUpdater.Unmap(0);
            }

           // LoadTextureArray(file);
            //setting up vertices and creating vertex buffer
            LoadVertexInfo();
            CreateVertexBuffer();

            //constant buffer variables
            effect.GetVariableByName("mapSize").AsScalar().Set(Globals.mapSize);
            using(D3D10.ShaderResourceView normalView=new D3D10.ShaderResourceView(Game.gameClass.GetDevice(),normalTexture))
                effect.GetVariableByName("normalMap").AsResource().SetResource(normalView);
          //  using (D3D10.ShaderResourceView texturesView = new D3D10.ShaderResourceView(Game.gameClass.GetDevice(), textures))
            //    effect.GetVariableByName("textures").AsResource().SetResource(texturesView);

            orientations=effect.GetVariableByName("orientations").AsVector();
           // effect.GetVariableByName("texCoordMul").AsScalar().Set(TextureInfo.texCoordMul);
            heightMul = effect.GetVariableByName("heightMul").AsScalar();
            heightMul.Set(Globals.heightMultiplier);

            //handles edit mode
            if (Game.gameClass.GetEngineState() != EngineState.play)
            {
                techniqueEdit = effect.GetTechniqueByName("Edit");
                passEdit = techniqueEdit.GetPassByIndex(0);
                mousePick = effect.GetVariableByName("mousePick").AsVector();
                pickOptions = effect.GetVariableByName("pickOpt").AsVector();
            }
            Globals.SetMap(map);
        }
      /*  private void LoadTextureArray(StreamReader file)
        {
            int nrTex = Convert.ToInt32(file.ReadLine());
            D3D10.ImageLoadInformation loadInfo = new SlimDX.Direct3D10.ImageLoadInformation();
            loadInfo.BindFlags = D3D10.BindFlags.None;
            loadInfo.CpuAccessFlags = D3D10.CpuAccessFlags.None;
            loadInfo.Usage = D3D10.ResourceUsage.Default;
            loadInfo.MipLevels = 2;
            loadInfo.FirstMipLevel = 0;
            loadInfo.Format = SlimDX.DXGI.Format.R8G8B8A8_UNorm;
            texInfos = new TextureInfo[nrTex];
            for (int i = 0; i < nrTex; i++)
            {
                texInfos[i] = new TextureInfo();

                TextureInfo.texCoordMul[i] = (float)Convert.ToDouble(file.ReadLine());
                texInfos[i].name = file.ReadLine();
                using (D3D10.Texture2D temp = D3D10.Texture2D.FromFile(Game.gameClass.GetDevice(), "Resources\\Textures\\" + texInfos[i].name, loadInfo))
                    textures = ResourceManager.mainManager.AddToTexture2D("textures", temp, i, nrTex);
            }
        }*/
        private void LoadVertexInfo()
        {
            //vertex buffer
            using (Bitmap normalMap = ResourceManager.mainManager.LoadBitmap(Path.GetDirectoryName(Game.gameClass.GetLvLFilePath()) + "\\normals.png"))
            {
                map = new Vertex[Globals.mapSize + 1, Globals.mapSize + 1];
                    for (int i = 0; i <= Globals.mapSize; i++)
                    {
                        for (int j = 0; j <= Globals.mapSize; j++)
                        {
                            Color temp;
                            if (i == Globals.mapSize || j == Globals.mapSize)
                                temp = new Color4(0, 0, 1, 0).ToColor();
                            else
                                temp = normalMap.GetPixel(i, j);
                            map[i, j] = new Vertex(new Vector3(i, temp.A, j), new Vector3((temp.R - 127) / 127f, (temp.G - 127) / 127f, (temp.B - 127) / 127f));
                        }
                    }
            }
        }
        private void CreateVertexBuffer()
        {
                //create vertex buffer
                int size = 8 * quadSize * quadSize * 6; //end size of each quad will be quadSize+1
                DataStream stream = new DataStream(size, true, true);
                for (int i = 0; i < quadSize; i++)
                    for (int j = 0; j < quadSize; j++)
                    {
                        stream.Write(new Vector2(i, j));
                        stream.Write(new Vector2(i, j + 1));
                        stream.Write(new Vector2(i + 1, j + 1));

                        stream.Write(new Vector2(i + 1, j + 1));
                        stream.Write(new Vector2(i + 1, j));
                        stream.Write(new Vector2(i, j));
                    }
                stream.Position = 0;

                vertexBuffer = new SlimDX.Direct3D10.Buffer(Game.gameClass.GetDevice(), stream, size, SlimDX.Direct3D10.ResourceUsage.Immutable,
                    SlimDX.Direct3D10.BindFlags.VertexBuffer, SlimDX.Direct3D10.CpuAccessFlags.None, SlimDX.Direct3D10.ResourceOptionFlags.None);
                stream.Close();
            
            instances = new Vector4[(Globals.mapSize / quadSize) * (Globals.mapSize / quadSize)];
            vertexBufferBinding = new SlimDX.Direct3D10.VertexBufferBinding(vertexBuffer, sizeof(float) * 2, 0);
        }


        private void CalculateTerrainNormalsSobel(Vector2 min, Vector2 max)
        {
            bool is5x5=true;
            int offset=2;
            if (normalForm.radioButton1.Checked)
            {
                offset = 1;
                is5x5 = false;
            }
            
             for (int i = (int)min.X; i <= max.X; i++)
                for (int j = (int)min.Y + 1; j <= max.Y; j++)
                {
                    Vector2 sum = new Vector2();
                    for(int x=-offset;x<=offset;x++)
                        for (int y = -offset; y <= offset; y++)
                        {
                            if (i + x < 0 || j + y < 0 || i + x > Globals.mapSize || j + y > Globals.mapSize)
                                continue;
                            sum.X += map[i + x, j + y].position.Y * Globals.heightMultiplier * SobelMasks.Get(x + offset, y + offset, is5x5).X;
                            sum.Y += map[i + x, j + y].position.Y * Globals.heightMultiplier * SobelMasks.Get(x + offset, y + offset, is5x5).Y;
                        }
                    map[i, j].normal.X = -sum.Y;
                    map[i, j].normal.Z = -sum.X;
                    map[i, j].normal.Y = 255*Globals.heightMultiplier*(normalForm.trackBar1.Value/10f);
                    map[i, j].normal.Normalize();
                }
        }
        private void SaveNormalBitmap(Bitmap normalMap,Vector2 min, Vector2 max)
        {       
            for (int i = (int)min.X; i <= max.X; i++)
                for (int j = (int)min.Y; j <= max.Y; j++)
                {
                    Color color = Color.FromArgb((int)map[i,j].position.Y, (int)(map[i, j].normal.X * 127 + 127),
                        (int)(map[i, j].normal.Y * 127 + 127), (int)(map[i, j].normal.Z * 127 + 127));
                    normalMap.SetPixel(i, j, color);
                }
            normalMap.Save(Path.GetDirectoryName(Game.gameClass.GetLvLFilePath()) + "\\normals.png", System.Drawing.Imaging.ImageFormat.Png); //saved as png. I hope it's lossless
        }
 
        //edit mode functions
        public void MakeForm(StreamReader editOpt)
        {
            normalForm = new NormalForm();
            if (Convert.ToBoolean(editOpt.ReadLine()))
                normalForm.radioButton1.Checked = true;
            else
                normalForm.radioButton2.Checked = true;
            normalForm.trackBar1.Value = Convert.ToInt32(editOpt.ReadLine());
            normalForm.button1.Click += new EventHandler(button1_Click);
            form = new TerrainForm();
            form.trackBar1.Value = Convert.ToInt32(editOpt.ReadLine());
            pickerForm = new TerrainPicker();
            pickerForm.trackBar2.Value = Convert.ToInt32(editOpt.ReadLine());
            pickerForm.trackBar2.ValueChanged += new EventHandler(trackBar2_ValueChanged);
            pickerForm.trackBar3.Value = Convert.ToInt32(editOpt.ReadLine());
            pickerForm.trackBar3.ValueChanged += new EventHandler(trackBar2_ValueChanged);
            pickerForm.trackBar1.ValueChanged += new EventHandler(trackBar2_ValueChanged);
            pickerForm.trackBar1.Value = Convert.ToInt32(editOpt.ReadLine());     
            try { ((Editor)Interface.mainInterface.GetModuleByIndex(2)).AddNewForm(form, Lang.main.GetText(36), Lang.main.GetText(35)); }
            catch { ((Editor)Interface.mainInterface.GetModuleByIndex(2)).AddNewForm(form, "Modeler", "Terrain"); }
            try { ((Editor)Interface.mainInterface.GetModuleByIndex(2)).AddNewForm(pickerForm, Lang.main.GetText(43), Lang.main.GetText(35)); }
            catch { ((Editor)Interface.mainInterface.GetModuleByIndex(2)).AddNewForm(pickerForm, "Mouse Picker", "Terrain"); }
            ((Editor)Interface.mainInterface.GetModuleByIndex(2)).AddNewForm(normalForm, "Terrain normals", "Terrain");
        }

        void button1_Click(object sender, EventArgs e)
        {
            Vector2 min = new Vector2(0, 0), max = new Vector2(Globals.mapSize - 1, Globals.mapSize - 1);
            CalculateTerrainNormalsSobel(min, max);
            UpdateNormalTex(min, max);
        }
        public void Save(StreamWriter file)
        {
            using (Bitmap normalMap = new Bitmap(Globals.mapSize, Globals.mapSize, System.Drawing.Imaging.PixelFormat.Format32bppArgb))
                SaveNormalBitmap(normalMap, new Vector2(0, 0), new Vector2(Globals.mapSize - 1, Globals.mapSize - 1));
           /* file.WriteLine(texInfos.Length.ToString());
            for (int i = 0; i < texInfos.Length; i++)
            {
                file.WriteLine(TextureInfo.texCoordMul[i].ToString());
                file.WriteLine(texInfos[i].name);
            }*/
        }
        public void SaveForm(StreamWriter file)
        {
            file.WriteLine(normalForm.radioButton1.Checked.ToString());
            file.WriteLine(normalForm.trackBar1.Value.ToString());
            file.WriteLine(form.trackBar1.Value.ToString());
            file.WriteLine(pickerForm.trackBar2.Value.ToString());
            file.WriteLine(pickerForm.trackBar3.Value.ToString());
            file.WriteLine(pickerForm.trackBar1.Value.ToString());
        }
        void trackBar2_ValueChanged(object sender, EventArgs e)
        {
            float first=pickerForm.trackBar2.Value / 10f;
            float second=(100-pickerForm.trackBar3.Value)*first/100f;
            pickOptions.Set(new Vector4(first, second, first-second, pickerForm.trackBar1.Value/100f));
        }
        private void UpdateNormalTex(Vector2 min, Vector2 max)
        {                
                for (int i = (int)min.X; i <= max.X; i++)
                    for (int j = (int)min.Y; j <= max.Y; j++)
                    {
                        Color color = Color.FromArgb((int)map[i, j].position.Y, (int)(map[i, j].normal.X * 127 + 127),
                            (int)(map[i, j].normal.Y * 127 + 127), (int)(map[i, j].normal.Z * 127 + 127));
                        normalData.Data.Position = (j * Globals.mapSize + i) * 4;
                        normalData.Data.WriteByte(color.R);
                        normalData.Data.WriteByte(color.G);
                        normalData.Data.WriteByte(color.B);
                        normalData.Data.WriteByte(color.A);
                    }
                normalData.Data.Position = 0;
                
                D3D10.ResourceRegion temp = new SlimDX.Direct3D10.ResourceRegion();
                temp.Left = (int)min.X;
                temp.Right = (int)max.X;
                temp.Top = (int)min.Y;
                temp.Bottom = (int)max.Y;
                temp.Front = 0;
                temp.Back = 1;
                Game.gameClass.GetDevice().CopySubresourceRegion(normalTexUpdater, 0, temp, normalTexture, 0, (int)min.X, (int)min.Y, 0);
        }
        //usual functions
        /// <summary>
        /// Draws quad on the screen
        /// </summary>
        /// <param name="x">Position of quad on x axis</param>
        /// <param name="y">Position of quad on z axis</param>
        public void DrawAll()
        {
            Game.gameClass.GetDevice().InputAssembler.SetInputLayout(layout);
            Game.gameClass.GetDevice().InputAssembler.SetPrimitiveTopology(SlimDX.Direct3D10.PrimitiveTopology.TriangleList);
            Game.gameClass.GetDevice().InputAssembler.SetVertexBuffers(0, vertexBufferBinding);
            orientations.Set(instances);
            
            if (Game.gameClass.GetEngineState() != EngineState.play)
            {
                heightMul.Set(Globals.heightMultiplier);
                mousePick.Set(new Vector4(MousePicker.main.GetRayTerrainIntersection().X, MousePicker.main.GetRayTerrainIntersection().Z, 1, 1));
            }
                if (Game.gameClass.GetEngineState() != EngineState.play && (form.Visible || pickerForm.Visible))
                    passEdit.Apply();
                else
                    pass.Apply();
                Game.gameClass.GetDevice().DrawInstanced(quadSize * quadSize * 6, counter, 0, 0);
                counter = 0;
        }
        public void AddToDraw(int x, int y)
        {
            instances[counter++] = new Vector4(x, y,1,1);
        }
        public void Update(double time)
        {
            UpdateModeler(time);
        }
        public void UpdateModeler(double time)
        {
            //here goes meat and potatoes of terrain modeling
            if (form != null && form.Visible && (InOut.mainIO.IsLeftPressed() || InOut.mainIO.IsRightPressed()))
            {
                if (MousePicker.main.GetRayTerrainIntersection().X < 0 || MousePicker.main.GetRayTerrainIntersection().Z < 0 ||
                    MousePicker.main.GetRayTerrainIntersection().X > Globals.mapSize || MousePicker.main.GetRayTerrainIntersection().Z > Globals.mapSize)
                    return;
                //initialize variables
                float r = pickerForm.trackBar2.Value / 10f;
                float heightValue = form.trackBar1.Value;
                float smooth = (100 - pickerForm.trackBar3.Value) * r / 100f;
                Vector2 point = new Vector2(MousePicker.main.GetRayTerrainIntersection().X, MousePicker.main.GetRayTerrainIntersection().Z);
                Vector2 min = new Vector2(Math.Max(0, point.X - r - 1), Math.Max(0, point.Y - r - 1));
                Vector2 max = new Vector2(Math.Min(Globals.mapSize - 1, point.X + r + 1), Math.Min(Globals.mapSize - 1, point.Y + r + 1));
                r *= r;
                smooth *= smooth;

                //calculate average height
                float averageHeight = 0;
                if (form.radioButton2.Checked)
                {
                    int counter = 0;
                    float sum = 0;
                    for (int i = (int)min.X; i <= max.X; i++)
                        for (int j = (int)min.Y; j <= max.Y; j++)
                        {
                            float sqDist = (i - point.X) * (i - point.X) + (j - point.Y) * (j - point.Y);
                            if (sqDist < r)
                            {
                                sum += map[i, j].position.Y;
                                counter++;
                            }
                        }
                    averageHeight = sum / counter;
                }

                //model terrain
                for (int i = (int)min.X; i <= max.X; i++)
                    for (int j = (int)min.Y; j <= max.Y; j++)
                    {
                        float sqDist = (i - point.X) * (i - point.X) + (j - point.Y) * (j - point.Y);
                        float value = 1;
                        if (sqDist < r)
                        {
                            if (sqDist > smooth && smooth != r)
                                value = (r - sqDist) / (r - smooth);
                            if (form.radioButton1.Checked) //changing height
                            {
                                if (InOut.mainIO.IsLeftPressed())
                                    map[i, j].position.Y += (float)(heightValue * value * time);
                                else
                                    map[i, j].position.Y -= (float)(heightValue * value * time);
                                map[i, j].position.Y = Math.Max(0, Math.Min(255, map[i, j].position.Y));
                            }
                            else //smoothing the terrain
                            {
                                if (map[i, j].position.Y < averageHeight)
                                {
                                    map[i, j].position.Y += (float)(heightValue * value * time);
                                    map[i, j].position.Y = Math.Min(averageHeight, map[i, j].position.Y);
                                }
                                if (map[i, j].position.Y > averageHeight)
                                {
                                    map[i, j].position.Y -= (float)(heightValue * value * time);
                                    map[i, j].position.Y = Math.Max(averageHeight, map[i, j].position.Y);
                                }
                            }
                        }
                    }

                //recalculating normals and updating the buffer
                CalculateTerrainNormalsSobel(min, max);
                UpdateNormalTex(min, max);
            }
        }
        public void Dispose()
        {
            if (layout != null)
                layout.Dispose();
            if (vertexBuffer != null)
                vertexBuffer.Dispose();
            if (normalTexture != null)
                normalTexture.Dispose();
            if (normalTexUpdater != null)
                normalTexUpdater.Dispose();
        }
    }
}
