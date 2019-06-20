using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Checkers
{
    class Program
    {
        static void Main(string[] args)
        {
            Field f = new Field();
            Bot b = new Bot(f, false);//1 - black 0 - false

            f.print();
            f.MoveLeft(2,2);
            b.makeMove();
           
            b.makeMove();
            f.print();
            f.MoveLeft(2, 0);
            f.print();
            b.makeMove();
            //f.MoveLeft(5, 1);
            //f.MoveRight(5, 1);
            //f.MoveLeft(2, 0);
            //f.MoveLeft(3, 1);
            //f.MoveLeft(5, 3);

            f.print();
        }
    }
}
