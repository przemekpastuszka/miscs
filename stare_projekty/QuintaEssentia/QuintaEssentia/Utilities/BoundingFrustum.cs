/* Quinta Essentia by Przemysław Pastuszka
 * file:BoundingFrustum.cs
 * Contains: Class BoundingFrustum
 * BoundingFrustum: class to build a boundingFrustum to perform frustum culling
 * Last modification:21-07-2009
 */
using SlimDX;

namespace QuintaEssentia
{
    class BoundingFrustum
    {
        //variables
        private Plane[] planes;//six planes that define the frustum

        //building frustum
        /// <summary>
        /// Constructor
        /// </summary>
        public BoundingFrustum()
        {
            planes = new Plane[6];
        }
        /// <summary>
        /// Creates frustum using viewProjection (multiply of view and projection matrices) matrix.
        /// It computes six planes that define the frustum
        /// </summary>
        /// <param name="viewProjection">Combination of view and projection matrix</param>
        public void CreateFrustum(Matrix viewProjection)
        {
            //left plane
            planes[0] = new Plane(viewProjection.M14 + viewProjection.M11, viewProjection.M24 + viewProjection.M21,
                viewProjection.M34 + viewProjection.M31, viewProjection.M44 + viewProjection.M41);

            //right plane
            planes[1] = new Plane(viewProjection.M14 - viewProjection.M11, viewProjection.M24 - viewProjection.M21,
               viewProjection.M34 - viewProjection.M31, viewProjection.M44 - viewProjection.M41);

            //top plane
            planes[2] = new Plane(viewProjection.M14 - viewProjection.M12, viewProjection.M24 - viewProjection.M22,
               viewProjection.M34 - viewProjection.M32, viewProjection.M44 - viewProjection.M42);

            //bottom plane
            planes[3] = new Plane(viewProjection.M14 + viewProjection.M12, viewProjection.M24 + viewProjection.M22,
               viewProjection.M34 + viewProjection.M32, viewProjection.M44 + viewProjection.M42);
            
            //near plane
            planes[4] = new Plane(viewProjection.M13, viewProjection.M23,
               viewProjection.M33, viewProjection.M43);

            //far plane
            planes[5] = new Plane(viewProjection.M14 - viewProjection.M13, viewProjection.M24 - viewProjection.M23,
               viewProjection.M34 - viewProjection.M33, viewProjection.M44 - viewProjection.M43);

            // Normalize planes
            for (int i = 0; i < 6; i++)
                planes[i] = Plane.Normalize(planes[i]); 

        }

        //intersections
        /// <summary>
        /// Check if the bounding frustum and the bounding sphere intersect
        /// </summary>
        /// <param name="a">Bounding frustum</param>
        /// <param name="sphere">Bounding sphere</param>
        /// <returns>True or false/bounding volumes intersects or not</returns>
        public static bool Intersects(BoundingFrustum a, BoundingSphere sphere)
        {
            Vector4 vec = new Vector4(sphere.Center, 1f);
            for (int i = 0; i < 6; i++)
                if (Plane.Dot(a.planes[i], vec) + sphere.Radius < 0)
                    return false;
            return true;
        }
        /// <summary>
        /// Check if the bounding frustum and point intersect
        /// </summary>
        /// <param name="a">Bounding frustum</param>
        /// <param name="sphere">Point as Vector3</param>
        /// <returns>True or false/bounding volume an the point intersects or not/returns>
        public static bool Intersects(BoundingFrustum a, Vector3 point)
        {
            Vector4 vec = new Vector4(point, 1f);
            for (int i = 0; i < 6; i++)
                if (Plane.Dot(a.planes[i], vec) < 0)
                    return false;
            return true;
        }
        /// <summary>
        /// Check if the bounding frustum and the bounding sphere intersect
        /// </summary>
        /// <param name="a">Bounding frustum</param>
        /// <param name="sphere">Bounding box</param>
        /// <returns>True or false/bounding volumes intersects or not</returns>
        public static bool Intersects(BoundingFrustum a, BoundingBox box)
        {
            for (int i = 0; i < 6; i++)
            {
                //calculating the positive vertex 
                //(the vertex from the box that is further along the normal's direction)
                Vector4 p = new Vector4(box.Minimum,1f);
                if (a.planes[i].Normal.X >= 0)
                    p.X = box.Maximum.X;
                if (a.planes[i].Normal.Y >= 0)
                    p.Y = box.Maximum.Y;
                if (a.planes[i].Normal.Z >= 0)
                    p.Z = box.Maximum.Z;
                
                if (Plane.Dot(a.planes[i], p)<0) //if this vertex is not on a good side of the plane return false
                    return false;
            }
            return true;
        }
    }
}
