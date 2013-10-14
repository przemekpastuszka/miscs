using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace QuintaEssentia
{
    class Globals
    {
        public static float heightMultiplier = 1;
        public static int mapSize;
        private static Vertex[,] map;
        public static int GetHeight(int x, int y)
        {
            return (int)map[x, y].position.Y;
        }
        public static void SetMap(Vertex[,] data)
        {
            map=data;
        }
    }
}
