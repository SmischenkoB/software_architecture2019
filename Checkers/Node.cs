using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Checkers
{
    class Node
    {
        public Checker inside { get;set; }

        public int X { get; set; }
        public int Y { get; set; }

        public Node(int x, int y)
        {
            X = x;
            Y = y;
        }

        public Node(Checker inside, int x, int y)
        {
            this.inside = inside;
            X = x;
            Y = y;
        }
    }
}
