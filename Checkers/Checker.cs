using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Checkers
{
    class Checker
    {
        public bool Color { get; set; }// 0 - white, 1 - black

        public int X { get; set; }
        public int Y { get; set; }


        public Checker(bool color, int x, int y)
        {
            Color = color;
            X = x;
            Y = y;
        }
    }
}
