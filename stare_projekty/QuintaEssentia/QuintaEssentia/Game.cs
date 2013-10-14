/* Quinta Essentia by Przemysław Pastuszka
 * file:Game.cs
 * Contains: Class Game
 * Game: main class; initializes everything and handles the message pump
 * Last modification:28-07-2009
 */
using System;
using System.Drawing;
using System.Windows.Forms;
using SlimDX;
using SlimDX.Windows;
using D3D10 = SlimDX.Direct3D10;
using DXGI = SlimDX.DXGI;
using System.IO;
using System.Threading;

namespace QuintaEssentia
{
    enum EngineState { play, edit, newLevel };
    class Game
    {
        private Form window; //main game window
        private D3D10.Device device;
        private DXGI.SwapChain swapChain;
        private D3D10.RenderTargetView renderTarget;
        private D3D10.DepthStencilView depthStencilView;
        private DXGI.ModeDescription modeDesc; //this is here because after initialization it is also used when resizing window
        private Viewport viewPort;
        private int width = 1024, height = 768;
        private bool windowed = true;
        private Timer timer; //for calculating the intervals between frames and a value of FPS
        private SceneManager sceneManager;
        public static Game gameClass;
        private EngineState engineState = EngineState.edit;
        private String lvlFilePath = @"C:\Documenty\QuintaEssentia\QuintaEssentia\bin\x86\Debug\Resources\Levels\Default\default.lvl";
        //creation methods
        /// <summary>
        /// Does nothing. But it's private 
        /// </summary>
        private Game()
        {
        }
        public static void Create()
        {
            if (gameClass == null)
            {
                gameClass = new Game();
                gameClass.Run();
            }
        }

        //running the game
        private void Run()
        {
           // try
            //{
            if (File.Exists("options.txt"))
            {
                StreamReader optReader = new StreamReader("options.txt");
                width = Convert.ToInt32(optReader.ReadLine());
                height = Convert.ToInt32(optReader.ReadLine());
                windowed = Convert.ToBoolean(optReader.ReadLine());
                optReader.Close();
            }
                Log.mainLog = new Log();
                Lang.Create();
               /* using (FormAtLoad loadForm = new FormAtLoad())
                {
                    Application.Run(loadForm);
                    if (!loadForm.radioButton3.Checked)
                        lvlFilePath = loadForm.data[0];
                    if (loadForm.radioButton1.Checked)
                        engineState = EngineState.play;
                    if (loadForm.radioButton2.Checked)
                        engineState = EngineState.edit;
                    if (loadForm.radioButton3.Checked)
                    {
                        engineState = EngineState.newLevel;
                        Directory.CreateDirectory(Application.StartupPath+"\\Resources\\Levels\\"+loadForm.data[1]);
                        lvlFilePath = Application.StartupPath + "\\Resources\\Levels\\" + loadForm.data[1] + "\\" + loadForm.data[1] + ".lvl";
                        int size=Convert.ToInt32(loadForm.data[2]);
                        using (Bitmap a = new Bitmap(size, size,System.Drawing.Imaging.PixelFormat.Format32bppArgb))
                        {
                            for (int i = 0; i < size; i++)
                                for (int j = 0; j < size; j++)
                                    a.SetPixel(i, j, Color.FromArgb(0,127,254,127));
                            a.Save(Path.GetDirectoryName(lvlFilePath) + "\\normals.png", System.Drawing.Imaging.ImageFormat.Png);
                        }
                    }

                }*/
                InitializeSystem();
                
                Application.Idle += new EventHandler(ApplicationIdle);
                Application.Run(window);
           // }
           // catch (System.Exception ex)
           // {
            //    Log.mainLog.AddEvent("Error '" + ex.Message + "' could not be fixed. Application will be closed. <br/>Stack trace: <br/>"+ex.StackTrace.Replace("\n","<br/>"), Priority.high);
           //     MessageBox.Show("Error '" + ex.Message + "' could not be fixed. Application will be closed. Check log for more info.");
           // }
            //finally
            //{
                Log.mainLog.Save("log.html");
                Dispose();
           // }
        }
        /// <summary>
        /// Supports the message pump. It is called by system, never call it by yourself
        /// </summary>
        /// <param name="sender">Never call this one</param>
        /// <param name="e">Never call this one</param>
        private void ApplicationIdle(object sender, EventArgs e)
        {
            
            while (MessagePump.IsApplicationIdle)
            {
                
                Update();
                Draw();
            }
        }

        //getting informations
        public D3D10.Device GetDevice()
        {
            return device;
        }
        public Vector2 GetWindowSize()
        {
            return new Vector2(width, height);
        }
        public String GetLvLFilePath()
        {
            return lvlFilePath;
        }
        public EngineState GetEngineState()
        {
            return engineState;
        }
        public Viewport GetViewport()
        {
            return viewPort;
        }

        public void SetDefaultRenderTargets()
        {
            device.Rasterizer.SetViewports(viewPort);
            device.OutputMerger.SetTargets(depthStencilView, renderTarget);
        }

        //Initialization process
        /// <summary>
        /// As the name says: it initializes basic game system components like log, timer, interface or InOut modules
        /// </summary>
        private void InitializeSystem()
        {
            InitializeDX();
            timer = new Timer();
            ResourceManager.Create();        
            InOut.Create(window);
            Quad.Create();
            Interface.Create();
            Interface.mainInterface.AddModule(new Console());
            Interface.mainInterface.AddModule(new InfoPanel());
            if (engineState != EngineState.play)
                Interface.mainInterface.AddModule(new Editor());
            sceneManager = new SceneManager();
        }
        private void CreateDepthStencil()
        {
            DXGI.SampleDescription sampleDesc = new SlimDX.DXGI.SampleDescription();
            sampleDesc.Count = 1;
            sampleDesc.Quality = 0;

            D3D10.Texture2DDescription texDesc = new SlimDX.Direct3D10.Texture2DDescription();
            texDesc.Width = width;
            texDesc.Height = height;
            texDesc.MipLevels = 1;
            texDesc.ArraySize = 1;
            texDesc.BindFlags = D3D10.BindFlags.DepthStencil;
            texDesc.CpuAccessFlags = D3D10.CpuAccessFlags.None;
            texDesc.Format = DXGI.Format.D32_Float;
            texDesc.SampleDescription = sampleDesc;
            texDesc.Usage = D3D10.ResourceUsage.Default;
            texDesc.OptionFlags = D3D10.ResourceOptionFlags.None;

            D3D10.DepthStencilViewDescription stencilViewDesc = new SlimDX.Direct3D10.DepthStencilViewDescription();

            stencilViewDesc.Dimension = D3D10.DepthStencilViewDimension.Texture2D;
            stencilViewDesc.Format = texDesc.Format;
            stencilViewDesc.MipSlice = 0;
            stencilViewDesc.FirstArraySlice = 0;
            stencilViewDesc.ArraySize = 1;

            using (D3D10.Texture2D stencilTex = new SlimDX.Direct3D10.Texture2D(device, texDesc))
            {
                depthStencilView = new SlimDX.Direct3D10.DepthStencilView(device, stencilTex,stencilViewDesc);
            }
        }
        /// <summary>
        /// Basic DX10 initialization. It contains: creating a window, device, swap chain, render targets, viewPort
        /// </summary>
        private void InitializeDX()
        {
            //Initializing window
            window = new Form();
            window.ClientSize = new Size(width, height);
            window.Text = "Quinta Essentia Engine ver. 0.03";

            //Creating device and swap chain (first filling the description structures)
            modeDesc = new SlimDX.DXGI.ModeDescription();
            modeDesc.Format = DXGI.Format.R8G8B8A8_UNorm;
            modeDesc.RefreshRate = new Rational(60, 1);
            modeDesc.Scaling = DXGI.DisplayModeScaling.Unspecified;
            modeDesc.ScanlineOrdering = DXGI.DisplayModeScanlineOrdering.Unspecified;
            modeDesc.Width = width;
            modeDesc.Height = height;

            DXGI.SampleDescription sampleDesc = new SlimDX.DXGI.SampleDescription();
            sampleDesc.Count = 1;
            sampleDesc.Quality = 0;

            DXGI.SwapChainDescription swapDesc = new SlimDX.DXGI.SwapChainDescription();
            swapDesc.BufferCount = 1;
            swapDesc.IsWindowed = windowed;
            swapDesc.Flags = DXGI.SwapChainFlags.AllowModeSwitch;
            swapDesc.ModeDescription = modeDesc;
            swapDesc.OutputHandle = window.Handle;
            swapDesc.SampleDescription = sampleDesc;
            swapDesc.SwapEffect = DXGI.SwapEffect.Discard;
            swapDesc.Usage = DXGI.Usage.RenderTargetOutput;
            //DEBUG FLAG ON!!! REMEMBER THIS!
            D3D10.Device.CreateWithSwapChain(null, SlimDX.Direct3D10.DriverType.Hardware,
                SlimDX.Direct3D10.DeviceCreationFlags.Debug, swapDesc, out device, out swapChain);

            //making and setting the new viewport
            viewPort = new Viewport();
            viewPort.Height = height;
            viewPort.Width = width;
            viewPort.MaxZ = 1.0f;
            viewPort.MinZ = 0.0f;
            viewPort.X = 0;
            viewPort.Y = 0;

            device.Rasterizer.SetViewports(viewPort);
            
            //making main render target (it is bind with back buffer of swap chain)
            using (D3D10.Texture2D temp = swapChain.GetBuffer<D3D10.Texture2D>(0))
            {
                renderTarget = new SlimDX.Direct3D10.RenderTargetView(device, temp);
            }
            CreateDepthStencil();
            device.OutputMerger.SetTargets(depthStencilView, renderTarget);
        }

        //other functions
        /// <summary>
        /// Allows you to change window size during runtime
        /// </summary>
        /// <param name="width">New width of the window</param>
        /// <param name="height">New height of the window</param>
        void ChangeWindowSize(int w, int h)
        {
            if (width > 0 && height > 0)
            {
                width = w;
                height = h;
                window.Width = w;
                window.Height = h;
                viewPort.Height = height;
                viewPort.Width = width;
                device.Rasterizer.SetViewports(viewPort);
                modeDesc.Width = width;
                modeDesc.Height = height;
                swapChain.ResizeTarget(modeDesc);
                renderTarget.Dispose();
                depthStencilView.Dispose();
                swapChain.ResizeBuffers(1, width, height, modeDesc.Format, DXGI.SwapChainFlags.AllowModeSwitch);
                using (D3D10.Texture2D temp = swapChain.GetBuffer<D3D10.Texture2D>(0))
                {
                    renderTarget = new SlimDX.Direct3D10.RenderTargetView(device, temp);
                }
                CreateDepthStencil();
                device.OutputMerger.SetTargets(depthStencilView, renderTarget);
                sceneManager.WindowResized();
            }
            
        }

        //update process
        /// <summary>
        /// It updates all objects/components in the actual game
        /// </summary>
        private void Update()
        {
            ProcessCommands();
            double gameTime = timer.NewFrame();
            sceneManager.Update(gameTime);
            Interface.mainInterface.Update();
            ((InfoPanel)Interface.mainInterface.GetModuleByIndex(1)).SetInfo("FPS:\n" + timer.GetFPS());
            InOut.mainIO.Update();
        }
        /// <summary>
        /// It processes all commands that come from console
        /// </summary>
        private void ProcessCommands()
        {
            String command = ((Console)Interface.mainInterface.GetModuleByIndex(0)).LastCommand();
            if (command == "")
                return;
            
            if (command.StartsWith("RESIZE"))
            {
                String[] size = command.Substring(7).Split('X');
                if (size.Length == 2)
                    ChangeWindowSize(Convert.ToInt32(size[0]), Convert.ToInt32(size[1]));
            }
            if (command == "FULLSCREEN")
            {
                windowed = !windowed;
                swapChain.SetFullScreenState(!windowed,null);
                if (windowed)
                    ChangeWindowSize(width, height);
            }
            if (command == "EXIT")
            {
                if(!windowed)
                    swapChain.SetFullScreenState(false, null);
                Application.Exit();
            }
        }

        /// <summary>
        /// It prepares drawing in DX10 and call drawing functions of objects
        /// </summary>
        private void Draw()
        {
                device.ClearRenderTargetView(renderTarget, Color.Black);
                device.ClearDepthStencilView(depthStencilView, D3D10.DepthStencilClearFlags.Depth, 1, 0);
                sceneManager.Draw();
                Interface.mainInterface.Draw();
                swapChain.Present(0, SlimDX.DXGI.PresentFlags.None);
        }
        /// <summary>
        /// It releases used unmanaged memory
        /// </summary>
        private void Dispose()
        {
            StreamWriter optWriter = new StreamWriter("options.txt");
            optWriter.WriteLine(width.ToString());
            optWriter.WriteLine(height.ToString());
            optWriter.WriteLine(windowed.ToString());
            optWriter.Close();
            if (device != null)
            {
                device.ClearState();
                device.Dispose();
            }
            if(renderTarget!=null)
                renderTarget.Dispose();
            if (depthStencilView != null)
                depthStencilView.Dispose();
            if(swapChain!=null)
               swapChain.Dispose();
            if (ResourceManager.mainManager != null)
                ResourceManager.mainManager.Dispose();
            if (Interface.mainInterface != null)
                Interface.mainInterface.Dispose();
            if (sceneManager != null)
                sceneManager.Dispose();
            Quad.Dispose();
        }
    }
}
