using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Checkers
{
    class Field
    {
        List<Node> arrayOfElements = new List<Node>();

        public List<Checker> Black = new List<Checker>();
        public List<Checker> White = new List<Checker>();

        public Field()
        {

            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    arrayOfElements.Add(new Node(i,j));
                }
            }


            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if(i < 3)
                    {
                        if((i + j)% 2 == 0)
                        {
                            Checker obj = new Checker(false, i, j);
                            arrayOfElements.Where((x) => x.X == i && x.Y == j).First().inside = obj;
                            White.Add(obj);
                        }
                    }

                    if (i > 4)
                    {
                        if ((i + j) % 2 == 0)
                        {
                            Checker obj = new Checker(true, i, j);
                            arrayOfElements.Where((x) => x.X == i && x.Y == j).First().inside = obj;
                            Black.Add(obj);
                        }
                    }
                }
            }
        }

        public void MoveLeft(int i, int j)
        {
            Checker moving = arrayOfElements.Where(o => o.X == i && o.Y == j).First().inside;
            if (moving == null)
            {
                Console.WriteLine("no element");
                throw new ArgumentException();
            }

            //if(arrayOfElements.Where(o => o.X == i  && o.Y == j).First().inside == null)
            //{

            //}
            int direction = moving.Color ? -1 : 1;
            
            if (i + direction < 0 || j + (1 * direction) < 0 || j + (1 * direction) > 7 || i + direction > 7) { throw new ArgumentException(); }
            //whether next  node is empty
            if(arrayOfElements.Where(o => o.X == i+direction && o.Y == j + (1*direction)).First().inside == null)
            {            
                arrayOfElements.Where(o => o.X == i && o.Y == j).First().inside = null;
                arrayOfElements.Where(o => o.X == i + direction && o.Y == j + (1 * direction)).First().inside = moving;
                moving.X = i + direction;
                moving.Y = j + (1 * direction);
                return;
               // if(X < 0 || Y < 0)
            }

            if (i + 2 * direction < 0 || j + (2 * direction) < 0 || j + (2 * direction) > 7 || i +  2 *direction > 7) { throw new ArgumentException(); }

            //attack
            if (arrayOfElements.Where(o => o.X == i + direction && o.Y == j + (1 * direction)).First().inside != null &&
                arrayOfElements.Where(o => o.X == i + direction && o.Y == j + (1 * direction)).First().inside.Color != moving.Color
                && arrayOfElements.Where(o => o.X == i + 2 * direction && o.Y == j + (2 * direction)).First().inside == null)
            {
                arrayOfElements.Where(o => o.X == i && o.Y == j).First().inside = null;

                Checker Deleted = arrayOfElements.Where(o => o.X == i + direction && o.Y == j + (1 * direction)).First().inside;
                arrayOfElements.Where(o => o.X == i + direction && o.Y == j + (1 * direction)).First().inside = null;
                if (Deleted.Color) { Black.Remove(Deleted); }
                else { White.Remove(Deleted); }

                arrayOfElements.Where(o => o.X == i + 2 * direction && o.Y == j + (2 * direction)).First().inside = moving;
                moving.X = moving.X = i + 2 * direction;
                moving.Y = j + (2 * direction);
            }

        }


        public void MoveRight(int i, int j)
        {
            Checker moving = arrayOfElements.Where(o => o.X == i && o.Y == j).First().inside;
            if (moving == null)
            {
                Console.WriteLine("no element");
                throw new ArgumentException();
            }

            //if(arrayOfElements.Where(o => o.X == i  && o.Y == j).First().inside == null)j
            //{

            //}
            int direction = moving.Color ? -1 : 1;//cgange

            if (i + direction < 0 || j - (1 * direction) > 7 || j - (1 * direction) < 0 || i + direction > 7) { throw new ArgumentException(); }
            //whether next  node is empty
            if (arrayOfElements.Where(o => o.X == i + direction && o.Y == j - (1 * direction)).First().inside == null)
            {
                arrayOfElements.Where(o => o.X == i && o.Y == j).First().inside = null;
                arrayOfElements.Where(o => o.X == i + direction && o.Y == j - (1 * direction)).First().inside = moving;
                moving.X = i + direction;
                moving.Y = j - (1 * direction);
                return;
                // if(X < 0 || Y < 0)
            }

            if (i + 2 * direction < 0 || j - (2 * direction) > 7 || i + 2 * direction > 7 || j - (2 * direction) < 0) { throw new ArgumentException(); }

            //attack
            if (arrayOfElements.Where(o => o.X == i + direction && o.Y == j - (1 * direction)).First().inside != null &&
                arrayOfElements.Where(o => o.X == i + direction && o.Y == j - (1 * direction)).First().inside.Color != moving.Color
                && arrayOfElements.Where(o => o.X == i + 2 * direction && o.Y == j - (2 * direction)).First().inside == null)
            {
                arrayOfElements.Where(o => o.X == i && o.Y == j).First().inside = null;
                if (moving.Color) { White.Remove(arrayOfElements.Where(o => o.X == i + direction && o.Y == j - (1 * direction)).First().inside); }
                else { Black.Remove(arrayOfElements.Where(o => o.X == i + direction && o.Y == j - (1 * direction)).First().inside); }
                arrayOfElements.Where(o => o.X == i + direction && o.Y == j - (1 * direction)).First().inside = null;
                arrayOfElements.Where(o => o.X == i + 2 * direction && o.Y == j - (2 * direction)).First().inside = moving;
                moving.X = moving.X = i + 2 * direction;
                moving.Y = j - (2 * direction);
            }

        }


        public void print()
        {
            for (int i = 0; i < 8; i++)
            {
                Console.Write(i);
                for (int j = 0; j < 8; j++)
                {
                    Checker moving = arrayOfElements.Where(o => o.X == i && o.Y == j).First().inside;
                    if(moving != null)
                    {
                        if (moving.Color) { Console.ForegroundColor = ConsoleColor.Red; }
                        //Console.Clear();
                        Console.Write("x");
                        Console.ResetColor();

                    }
                    else
                    { Console.Write(" "); }
                }
                Console.WriteLine();
            }
            Console.WriteLine();
        }

        public bool AbleToMakeMoveLeft(Checker c)
        {
            int direction = c.Color ? -1 : 1;
            if ((c.X + direction) > 0 && (c.Y + (1 * direction)) > 0 && (c.Y + (1 * direction)) < 7 && (c.X + direction) < 7)
            { 
                if (arrayOfElements.Where(o => o.X == c.X + direction && o.Y == c.Y + (1 * direction)).First().inside == null)
                { return true; }
            }
            if (c.X + 2 * direction > 0 && c.Y + (2 * direction) > 0 && c.Y + (2 * direction) < 7 && c.X + 2 * direction < 7)
            {

                if (arrayOfElements.Where(o => o.X == c.X + direction && o.Y == c.Y + (1 * direction)).First().inside != null &&
                    arrayOfElements.Where(o => o.X == c.X + direction && o.Y == c.Y + (1 * direction)).First().inside.Color != c.Color
                    && arrayOfElements.Where(o => o.X == c.X + 2 * direction && o.Y == c.Y + (2 * direction)).First().inside == null)
                {
                    return true;
                }
            }
                return false;
        }

        public bool AbleToMakeMoveRight(Checker c)
        {
            int direction = c.Color ? -1 : 1;

            if (c.X + direction > 0 && c.Y - (1 * direction) < 7 && c.Y - (1 * direction) > 0 && c.X + direction < 7)
                if (arrayOfElements.Where(o => o.X == c.X + direction && o.Y == c.Y - (1 * direction)).First().inside == null) { return true; }

            if (c.X + 2 * direction > 0 && c.Y - (2 * direction) < 7 && c.X + 2 * direction < 7 && c.Y - (2 * direction) > 0)
            {
                if (arrayOfElements.Where(o => o.X == c.X + direction && o.Y == c.Y - (1 * direction)).First().inside != null &&
                arrayOfElements.Where(o => o.X == c.X + direction && o.Y == c.Y - (1 * direction)).First().inside.Color != c.Color
                && arrayOfElements.Where(o => o.X == c.X + 2 * direction && o.Y == c.Y - (2 * direction)).First().inside == null)
                { return true; }
            }
                return false;

        }
    }
}
