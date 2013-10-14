using SlimDX;
using D3D10 = SlimDX.Direct3D10;
using System.IO;
using System;
using System.Drawing;
using System.Collections.Generic;

namespace QuintaEssentia
{
    class Quad
    {
        static public D3D10.Buffer buffer, bufferWithTexCoord;
        static public D3D10.VertexBufferBinding binding, bindingWithTexCoord;
        static public D3D10.InputElement[] elements = new SlimDX.Direct3D10.InputElement[1];
        static public D3D10.InputElement[] elementsWithTexCoord = new SlimDX.Direct3D10.InputElement[2];
        static public void Create()
        {
            //without texcoord
            DataStream stream = new DataStream(12 * 6, true, true);
            stream.Write<Vector3>(new Vector3(-1, -1, 1));
            stream.Write<Vector3>(new Vector3(-1, 1, 1));
            stream.Write<Vector3>(new Vector3(1, 1, 1));
            stream.Write<Vector3>(new Vector3(1, 1, 1));
            stream.Write<Vector3>(new Vector3(1, -1, 1));
            stream.Write<Vector3>(new Vector3(-1, -1, 1));
            elements[0] = new SlimDX.Direct3D10.InputElement("POSITION", 0, SlimDX.DXGI.Format.R32G32B32_Float, 0, 0);
            stream.Position = 0;
            buffer = new SlimDX.Direct3D10.Buffer(Game.gameClass.GetDevice(), stream, (int)stream.Length, D3D10.ResourceUsage.Immutable,
                D3D10.BindFlags.VertexBuffer, D3D10.CpuAccessFlags.None, D3D10.ResourceOptionFlags.None);
            binding = new SlimDX.Direct3D10.VertexBufferBinding(buffer, 12, 0);
            stream.Close();

            //with texcoord
            stream = new DataStream(12 * 6*2, true, true);
            stream.Write<Vector3>(new Vector3(-1, -1, 1));
            stream.Write<Vector3>(Vector3.Zero);
            stream.Write<Vector3>(new Vector3(-1, 1, 1));
            stream.Write<Vector3>(Vector3.Zero);
            stream.Write<Vector3>(new Vector3(1, 1, 1));
            stream.Write<Vector3>(Vector3.Zero);
            stream.Write<Vector3>(new Vector3(1, 1, 1));
            stream.Write<Vector3>(Vector3.Zero);
            stream.Write<Vector3>(new Vector3(1, -1, 1));
            stream.Write<Vector3>(Vector3.Zero);
            stream.Write<Vector3>(new Vector3(-1, -1, 1));
            stream.Write<Vector3>(Vector3.Zero);
            
            elementsWithTexCoord[0] = new SlimDX.Direct3D10.InputElement("POSITION", 0, SlimDX.DXGI.Format.R32G32B32_Float, 0, 0);
            elementsWithTexCoord[1] = new SlimDX.Direct3D10.InputElement("TEXCOORD", 0, SlimDX.DXGI.Format.R32G32B32_Float, 12, 0);
            stream.Position = 0;
            bufferWithTexCoord = new SlimDX.Direct3D10.Buffer(Game.gameClass.GetDevice(), stream, (int)stream.Length, D3D10.ResourceUsage.Dynamic,
                D3D10.BindFlags.VertexBuffer, D3D10.CpuAccessFlags.Write, D3D10.ResourceOptionFlags.None);
            bindingWithTexCoord = new SlimDX.Direct3D10.VertexBufferBinding(bufferWithTexCoord, 24, 0);
            stream.Close();
        }
        static public D3D10.InputLayout GetLayout(D3D10.EffectPass pass)
        {
            return new SlimDX.Direct3D10.InputLayout(Game.gameClass.GetDevice(), elements, pass.Description.Signature);
        }
        static public D3D10.InputLayout GetLayoutWithTexCoord(D3D10.EffectPass pass)
        {
            return new SlimDX.Direct3D10.InputLayout(Game.gameClass.GetDevice(), elementsWithTexCoord, pass.Description.Signature);
        }
        static public void UpdateTexCoordForQuad(Matrix view, Matrix projection, Vector3 cameraPos)
        {
            DataStream stream = bufferWithTexCoord.Map(SlimDX.Direct3D10.MapMode.WriteDiscard, D3D10.MapFlags.None);
            Vector2 temp = Game.gameClass.GetWindowSize();
            stream.Position = 0;
            stream.Write<Vector3>(new Vector3(-1, -1, 1));
            stream.Write<Vector3>(Vector3.Unproject(new Vector3(0, 0, 1), Game.gameClass.GetViewport(), projection, view, Matrix.Identity) - cameraPos);
            stream.Write<Vector3>(new Vector3(-1, 1, 1));
            stream.Write<Vector3>(Vector3.Unproject(new Vector3(0, temp.Y, 1), Game.gameClass.GetViewport(), projection, view, Matrix.Identity) - cameraPos);
            stream.Write<Vector3>(new Vector3(1, 1, 1));
            stream.Write<Vector3>(Vector3.Unproject(new Vector3(temp.X, temp.Y, 1), Game.gameClass.GetViewport(), projection, view, Matrix.Identity) - cameraPos);
            stream.Write<Vector3>(new Vector3(1, 1, 1));
            stream.Write<Vector3>(Vector3.Unproject(new Vector3(temp.X, temp.Y, 1), Game.gameClass.GetViewport(), projection, view, Matrix.Identity) - cameraPos);
            stream.Write<Vector3>(new Vector3(1, -1, 1));
            stream.Write<Vector3>(Vector3.Unproject(new Vector3(temp.X, 0, 1), Game.gameClass.GetViewport(), projection, view, Matrix.Identity) - cameraPos);
            stream.Write<Vector3>(new Vector3(-1, -1, 1));
            stream.Write<Vector3>(Vector3.Unproject(new Vector3(0, 0, 1), Game.gameClass.GetViewport(), projection, view, Matrix.Identity) - cameraPos);
            bufferWithTexCoord.Unmap();
        }
        static public void Dispose()
        {
            if (buffer != null)
                buffer.Dispose();
            if (bufferWithTexCoord != null)
                bufferWithTexCoord.Dispose();
        }
    }
}
