using System;
using System.Drawing;
using SlimDX;
using System.IO;

namespace QuintaEssentia
{
    class MousePicker
    {
        public static MousePicker main;
        private Ray ray;
        private Vector3 point;
        public static void Create()
        {
            if (main == null)
                main = new MousePicker();
        }
        public void Update(Matrix view, Matrix projection)
        {
            Vector3 near = Vector3.Unproject(new Vector3(InOut.mainIO.GetMousePosition(), 0), Game.gameClass.GetViewport(),
                projection,view, Matrix.Identity);
            Vector3 far = Vector3.Unproject(new Vector3(InOut.mainIO.GetMousePosition(), 1), Game.gameClass.GetViewport(),
                projection,view, Matrix.Identity);
            Vector3 dir = far - near;
            dir.Normalize();
            ray = new Ray(near, dir);
            DDA(far);
            
        }
        private void DDA(Vector3 end)
        {
            point = new Vector3(-100, -100, -100);
            Vector2 step=new Vector2(), rr=new Vector2(1f/ray.Direction.X,1f/ray.Direction.Z), tMax=new Vector2(), delta=new Vector2();
            Vector2 current = new Vector2((int)ray.Position.X, (int)ray.Position.Z);
            Vector2 realEnd = new Vector2((int)end.X, (int)end.Z);
            if (ray.Direction.X > 0)
            {
                step.X = 1;
                tMax.X = (current.X + 1 - ray.Position.X) * rr.X;
            }
            else
            {
                step.X = -1;
                tMax.X = (current.X - ray.Position.X) * rr.X;
            }
            if (ray.Direction.Z > 0)
            {
                step.Y = 1;
                tMax.Y = (current.Y + 1 - ray.Position.Z) * rr.Y;
            }
            else
            {
                step.Y = -1;
                tMax.Y = (current.Y - ray.Position.Z) * rr.Y;
            }
            delta.X = step.X * rr.X;
            delta.Y = step.Y * rr.Y;
            while (current != realEnd)
            {
                if (tMax.X < tMax.Y)
                {
                    tMax.X += delta.X;
                    current.X += step.X;
                }
                else
                {
                    tMax.Y += delta.Y;
                    current.Y += step.Y;
                }
                if (current.X < 0 || current.Y < 0 || current.X >= Globals.mapSize || current.Y >= Globals.mapSize)
                    break;
                float distance;
                float mapH = Globals.GetHeight((int)current.X, (int)current.Y) * Globals.heightMultiplier;
                //sth faster should be developed
                if (Ray.Intersects(ray, new BoundingBox(new Vector3(current.X, 0, current.Y), new Vector3(current.X + 1, mapH, current.Y + 1)), out distance))
                {
                    point = ray.Position + ray.Direction * distance;
                    break;
                }
            }
        }
        public Ray GetRay()
        {
            return ray;
        }
        public Vector3 GetRayTerrainIntersection()
        {
            return point;
        }
    }
}
