/* Quinta Essentia by Przemysław Pastuszka
 * file:SceneManager.cs
 * Contains: Class SceneManager
 * SceneManager: class is used to manage all drawing and updating operations
 * Last modification:25-07-2009
 */
using System;
using System.Drawing;
using System.Windows.Forms;
using SlimDX;
using D3D10 = SlimDX.Direct3D10;
using System.IO;

namespace QuintaEssentia
{
    class SceneManager
    {
        //node of the quad tree, represents some piece of the terrain
        class Node
        {
            public BoundingBox aaBB;//stores the max height of each quad (it is used for frustum culling)
        }

        //VARIABLES
        private const int quadSize = 32;
        private Node[] quadTree;
        private Camera camera;  //three special objects comes here
        private Terrain terrain;
        private Light globalLight;
        private Sky sky;
        private D3D10.EffectMatrixVariable viewProjectionHandle; //sth like pointers to shader constants
        private D3D10.EffectVectorVariable lightDirHandle;
        private D3D10.EffectScalarVariable ambientHandle;
        private int visibleQuads = 0, frustumChecks=0; //just for statistics
        private MainTerrainForm form; //used in edit mode
        private bool hasBeenSaved = false; //in newLevel mode if it hasn't been saved, all temp files will be deleted
        //Constructor
        public SceneManager()
        {
            StreamReader level;
            if (Game.gameClass.GetEngineState() != EngineState.newLevel)
            {
                level = new StreamReader(Game.gameClass.GetLvLFilePath());
                Globals.heightMultiplier = (float)Convert.ToDouble(level.ReadLine());
            }
            else
                level = null;
            
            String dirPath = Path.GetDirectoryName(Game.gameClass.GetLvLFilePath());
            
            MousePicker.Create();
            if (Game.gameClass.GetEngineState() != EngineState.play)
                MakeForm();
            globalLight = new Light(level);
            camera = new Camera(level);
            terrain = new Terrain(quadSize, level);
            sky = new Sky(globalLight.GetLightDir(), level);
            if (Game.gameClass.GetEngineState() != EngineState.play)
            {
                StreamReader editOpt = new StreamReader("editorOptions.txt");
                globalLight.MakeForm();
                camera.MakeForm();
                terrain.MakeForm(editOpt);
                sky.MakeForm();
                editOpt.Close();
            }
            CreateQuadTree();
            viewProjectionHandle = ResourceManager.mainManager.GetDefaultEffectPool().AsEffect().GetVariableByName("viewProjection").AsMatrix();
            lightDirHandle = ResourceManager.mainManager.GetDefaultEffectPool().AsEffect().GetVariableByName("lightDir").AsVector();
            ambientHandle = ResourceManager.mainManager.GetDefaultEffectPool().AsEffect().GetVariableByName("ambient").AsScalar();
            if(level!=null)
                level.Close();
            
        }

        //edit mode
        private void MakeForm()
        {
            form = new MainTerrainForm();
            form.trackBar1.Value = (int)(Globals.heightMultiplier*20);
            form.trackBar1.ValueChanged += new EventHandler(trackBar1_ValueChanged);
            try { ((Editor)Interface.mainInterface.GetModuleByIndex(2)).AddNewForm(form, Lang.main.GetText(35), ""); }
            catch { ((Editor)Interface.mainInterface.GetModuleByIndex(2)).AddNewForm(form, "Terrain", ""); }
        }
        void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            float temp = (form.trackBar1.Value / 20f) / Globals.heightMultiplier;
            Globals.heightMultiplier= form.trackBar1.Value / 20f ;
            for(int i=1;i<quadTree.Length;i++)
                quadTree[i].aaBB.Maximum.Y *= temp;
        }

        //Other functions
        public void WindowResized()
        {
            camera.CreateProjectionMatrix();
        }

        //Quad tree creation
        /// <summary>
        /// This function creates quad tree that covers all terrain map. Max height of each quad is also computed.
        /// </summary>
        /// <param name="name">Path of the heightmap</param>
        private void CreateQuadTree()
        {
            int n = (int)(4 * (Math.Pow(Globals.mapSize / quadSize, 2) - 1)) / 3;
            quadTree = new Node[n + 2];
            CreateNode(1, 0, 0, Globals.mapSize);
        }
        /// <summary>
        /// This is recursion function that initializes tree node.
        /// At the moment it computes only the max height of the node (quad)
        /// </summary>
        /// <param name="nr">Place in the array where the node lays</param>
        /// <param name="x">X position in the 2D heightmap</param>
        /// <param name="y">Y position in the 2D heightmap</param>
        /// <param name="size">Size of the current node</param>
        /// <returns></returns>
        private int CreateNode(int nr, int x, int y, int size)
        {
            quadTree[nr] = new Node();
            int maxHeight = 0;
            if (size != quadSize)
            {
                maxHeight=Math.Max(maxHeight,CreateNode(4*nr-2,x,y,size/2));
                maxHeight = Math.Max(maxHeight, CreateNode(4 * nr - 1, x+size/2, y, size / 2));
                maxHeight = Math.Max(maxHeight, CreateNode(4 * nr, x + size / 2, y+size/2, size / 2));
                maxHeight = Math.Max(maxHeight, CreateNode(4 * nr + 1, x, y + size/2, size / 2));
            }
            else
            {
                for(int i=0;i<quadSize;i++)
                    for (int j = 0; j < quadSize; j++)
                    {
                        maxHeight = Math.Max(maxHeight, Globals.GetHeight(i + x, j + y));
                    }
                maxHeight = (int)(maxHeight * Globals.heightMultiplier);
            }
            quadTree[nr].aaBB = new BoundingBox(new Vector3(x, 0, y), new Vector3(x + size+1, maxHeight, y + size+1));
            return maxHeight;
        }

        //Drawing the quad tree
        public void Draw()
        {
            terrain.DrawAll();
            sky.Draw(camera.GetPosition(), globalLight.GetLightDir());
            try
            {
                ((InfoPanel)Interface.mainInterface.GetModuleByIndex(1)).SetInfo(Lang.main.GetText(0) + visibleQuads.ToString());
                ((InfoPanel)Interface.mainInterface.GetModuleByIndex(1)).SetInfo(Lang.main.GetText(1) + frustumChecks.ToString());
            }
            catch { }
        }
        private void DrawNode(int nr, int x, int y, int size)
        {
            frustumChecks++;
            if (BoundingFrustum.Intersects(camera.GetBoundingFrustum(), quadTree[nr].aaBB))
            {
                if (size != quadSize)
                {
                    DrawNode(4 * nr - 2, x, y, size / 2);
                    DrawNode(4 * nr - 1, x + size / 2, y, size / 2);
                    DrawNode(4 * nr, x + size / 2, y + size / 2, size / 2);
                    DrawNode(4 * nr + 1, x, y + size / 2, size / 2);
                }
                else
                {
                    visibleQuads++;
                    terrain.AddToDraw(x, y);
                }
            }
        }

        //Usual functions
        public void Update(double time)
        {
            //map saving
            if (Game.gameClass.GetEngineState() != EngineState.play && ((Console)Interface.mainInterface.GetModuleByIndex(0)).LastCommand() == "SAVE")
            {
                StreamWriter writer = new StreamWriter(Game.gameClass.GetLvLFilePath());
                writer.WriteLine(Globals.heightMultiplier.ToString());
                globalLight.Save(writer);
                camera.Save(writer);
                
                terrain.Save(writer);
                writer.Close();
                ((Console)Interface.mainInterface.GetModuleByIndex(0)).AddLine("Map saved");
                hasBeenSaved = true;
            }
            
            //other updates
            camera.Update(time);
            globalLight.Update(time);
            Quad.UpdateTexCoordForQuad(camera.GetViewMatrix(), camera.GetProjectionMatrix(), camera.GetPosition());
            terrain.Update(time);
            viewProjectionHandle.SetMatrix(camera.GetViewProjectionMatrix());
            lightDirHandle.Set(new Vector4(globalLight.GetLightDir(), 1));
            ambientHandle.Set(globalLight.GetAmbient());
            frustumChecks=visibleQuads = 0;
            DrawNode(1, 0, 0, Globals.mapSize);
            if(Game.gameClass.GetEngineState()!=EngineState.play)
                MousePicker.main.Update(camera.GetViewMatrix(), camera.GetProjectionMatrix());

            String command = ((Console)Interface.mainInterface.GetModuleByIndex(0)).LastCommand();
            if (command == "SKY RECALCULATE" || globalLight.skyToBeRecalculated)
                sky.CalculateLookupOnGPU(-globalLight.GetLightDir());
            
        }
        public void Dispose()
        {
            if (Game.gameClass.GetEngineState() != EngineState.play)
            {
                StreamWriter editorOpt = new StreamWriter("editorOptions.txt");
                if (terrain != null)
                    terrain.SaveForm(editorOpt);
                editorOpt.Close();
            }
            //if it hasn't been saved, all temp files must say bye bye to sweet mama
            if (Game.gameClass.GetEngineState()==EngineState.newLevel&&(!hasBeenSaved))
                Directory.Delete(Path.GetDirectoryName(Game.gameClass.GetLvLFilePath()), true);
            if (terrain != null)
                terrain.Dispose();
            if (sky != null)
                sky.Dispose();
        }
        
    }
}
