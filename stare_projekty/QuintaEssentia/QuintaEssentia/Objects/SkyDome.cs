using SlimDX;
using D3D10 = SlimDX.Direct3D10;
using System.IO;
using System;
using System.Drawing;
using System.Collections.Generic;

namespace QuintaEssentia
{
    class SkyDome
    {
        public D3D10.Buffer vertexBuffer;
        public Vector3[,] data;
        public int length;
         public int vertexSize = 20;
        public void CreateDome(int width, int height)
        {
            float r=14950;
            data = new Vector3[width, height];
            for (int i = 0; i < width; i++)
            {
                double angleXZ = 90 * (i / ((float)width-1)) * Math.PI / 180;
                for (int j = 0; j < height; j++)
                {
                    double angleY= 2*(j / ((float)height-1)) * Math.PI;

                    data[i, j] = new Vector3((float)(Math.Sin(angleXZ) * Math.Cos(angleY)), (float)Math.Cos(angleXZ),(float)( Math.Sin(angleXZ) * Math.Sin(angleY)));
                    data[i, j] *= r;
                }
            }
            DataStream stream = new DataStream(vertexSize * (width-1) * (height-1) * 6, true, true);
            for (int i = 0; i < width-1; i++)
                for (int j = 0; j < height-1; j++)
                {
                    stream.Write(data[i, j]);
                    stream.Write(new Vector2(i, j));
                    stream.Write(data[i, j+1]);
                    stream.Write(new Vector2(i, j+1));
                    stream.Write(data[i+1, j+1]);
                    stream.Write(new Vector2(i+1, j+1));

                    stream.Write(data[i+1, j+1]);
                    stream.Write(new Vector2(i + 1, j + 1));
                    stream.Write(data[i+1, j]);
                    stream.Write(new Vector2(i + 1, j));
                    stream.Write(data[i, j]);
                    stream.Write(new Vector2(i, j));
                }
            stream.Position = 0;
            length = (int)stream.Length/vertexSize;
            vertexBuffer = new SlimDX.Direct3D10.Buffer(Game.gameClass.GetDevice(), stream, (int)stream.Length, D3D10.ResourceUsage.Immutable,
                D3D10.BindFlags.VertexBuffer, D3D10.CpuAccessFlags.None, D3D10.ResourceOptionFlags.None);

            stream.Close();
        }
    }
}
