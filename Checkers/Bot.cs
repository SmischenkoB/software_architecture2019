using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Checkers
{
    class Bot
    {
        private Field field;
        private bool color; // 0 - white, 1 - black

        public Bot(Field field , bool color)
        {
            this.field = field;
            this.color = color;
        }

        public void makeMove()
        {
            if (color)
            {
                for (int i = 0; i < field.White.Count; i++)
                {
                    if (field.AbleToMakeMoveLeft(field.White[i])) { field.MoveLeft(field.White[i].X, field.White[i].Y); return; }
                    if (field.AbleToMakeMoveRight(field.White[i])) { field.MoveRight(field.White[i].X, field.White[i].Y); return; }
                }
            }
            else
            {
                for (int i = 0; i < field.Black.Count; i++)
                {
                    if (field.AbleToMakeMoveLeft(field.Black[i])) { field.MoveLeft(field.Black[i].X, field.Black[i].Y); return; }
                    if (field.AbleToMakeMoveRight(field.Black[i])) { field.MoveRight(field.Black[i].X, field.Black[i].Y); return; }
                }
            }
        }


    }
}
