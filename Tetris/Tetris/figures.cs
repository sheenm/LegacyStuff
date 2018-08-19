// 1 - горизонтальный вверх, 2 - горизонтальный вниз. 3 - вертикальный влево. 4 - верт - вправо.
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
namespace Tetris
{
    abstract class Figures
    {
        // Поля
        public Coord Top; //вершина фигуры
        protected int Status; // 1 - горизонтальный вверх, 2 - горизонтальный вниз. 3 - вертикальный влево. 4 - верт - вправо.
        protected int color; // Цвет фигуры
        //Для прорисовки
        
        // Методы
        public Figures()
        {
            Random rnd = new Random();
            Top = new Coord();
            Status = 1;
            color = (int)rnd.Next(1, 8);
            Parameters.p = new SolidBrush(Parameters.colors[color]);
        }
        public int Color {
            get { return color; }
            private set
            {
                color = value;
            }
        }
        public abstract bool TickAction(); // Падение фигуры вниз на 1
        public abstract bool PositionChange(int pos);
        public abstract bool Move(int plus); // > 0 - вправо
    }
    
    class LineFigure : Figures
    {
        public LineFigure() : base() {
            int i;
            //Проверка, может ли фигура упасть целиком:
            for (i = Top.x; i < Top.x + 4; i++)
                if (Parameters.CellInfo(1, i) != 0)
                {
                    Program.time.Stop();
                    MessageBox.Show("Вы проиграли");
                    return;
                }
            
            //Закрашивание
            for (i = Top.x; i < Top.x + 4; i++)
                Parameters.SetCell(i, 1, color);            
        }
        //Возвращает false если переместить фигуру не удастся. Тогда нужно создать новую фигуру, потеряв ссылку на старую.
        public override bool TickAction()
        {
            if (Status < 3)
            {
                int i;
                //Проверка, может ли фигура упасть целиком:
                for (i = Top.x; i < Top.x + 4; i++)
                    if (Parameters.CellInfo(Top.y + 1, i) != 0 )
                        return false;
                //стирание
                for (i = Top.x; i < Top.x + 4; i++)
                    Parameters.NullCell(i, Top.y);                
                // закрашивание
                for (i = Top.x; i < Top.x + 4; i++)
                    Parameters.SetCell(i, Top.y + 1, color);
                Top++;
            
            }

            if (Status > 2)
            {
                int i;
                //Проверка, может ли фигура упасть целиком:
                if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                //стирание
                for (i = Top.y; i > Top.y - 4; i--)
                    Parameters.NullCell(Top.x, i);
                
                // закрашивание
                for (i = Top.y + 1; i > Top.y - 3; i--)
                    Parameters.SetCell(Top.x, i, color);
                
                Top++;
            }
            return true;
        }
        
        public override bool PositionChange(int pos)
        {
            int i;
            if (Status < 3)
            {
                //проверка, может ли фигурка повернуться
                for (i = Top.y - 3; i < Top.y; i++)
                {
                    if (Parameters.CellInfo(i,Top.x+1) != 0) return false;
                }
                // Стирание
                for (i = Top.x; i < Top.x + 4; i++)
                    Parameters.NullCell(i, Top.y);
                // Прорисовка
                for (i = Top.y - 3; i <= Top.y; i++)
                    Parameters.SetCell(Top.x + 1, i, color);
                Top.x++;
                Status = 3;
                return true;
            }
            if (Status > 2)
            {
                //проверка, может ли фигурка повернуться
                if (Top.x + 2 >= Parameters.vpravo) return false;
                if (Parameters.CellInfo(Top.y,Top.x - 1) != 0) return false;
                if (Parameters.CellInfo(Top.y,Top.x + 1) != 0) return false;
                if (Parameters.CellInfo(Top.y,Top.x + 2) != 0) return false; 
                //Cтирание
                for (i = Top.y - 3; i <= Top.y; i++)
                    Parameters.NullCell(Top.x, i);
                //Прорисовка
                for (i = Top.x-1; i < Top.x + 3; i++)
                    Parameters.SetCell(i, Top.y,color);
                Top.x--; 
                Status = 1;
                return true;
            }
            return false;
        }
        
        public override bool Move(int plus)
        {
            int i ;
            // проверка, может ли двинуться
            if (Status < 3 && (plus > 0 && Parameters.CellInfo(Top.y, Top.x + 4) != 0)) return false;
            else if (Status < 3 && (plus < 0 && Parameters.CellInfo(Top.y, Top.x - 1) != 0)) return false;
            // Проверка При вертикальном состоянии
            else if (Status > 2)
            {
                for (i = Top.y; i > Top.y - 4; i--)
                    if (Parameters.CellInfo(i, Top.x+plus) != 0) return false;  
            }
            
            // Передвижение
            if (Status > 2)
            {
                //стирание
                for (i = Top.y; i > Top.y - 4; i--)
                    Parameters.NullCell(Top.x, i);
                // закрашивание
                for (i = Top.y; i > Top.y - 4; i--)
                    Parameters.SetCell(Top.x+plus, i, color);
                Top.x+=plus;
            }
            if (Status < 3)
            {
                //стирание
                for (i = Top.x; i < Top.x + 4; i++)
                    Parameters.NullCell(i, Top.y);
                
                if (plus > 0)
                    Top.x++;
                else Top.x--;
                
                // закрашивание
                    for (i = Top.x; i < Top.x + 4; i++)
                    Parameters.SetCell(i, Top.y, color);               
            }
            return true;
        }
    }
    
    class TriangleFigure : Figures
    {
        public TriangleFigure()
            : base()
        {
            int i;
            //Проверка, может ли фигура упасть целиком:
            for (i = Top.x-1; i < Top.x + 2; i++)
                if (Parameters.CellInfo(1, i) != 0)
                    break;
            if (i < Top.x + 2)
            {
                Program.time.Stop();
                MessageBox.Show("Вы проиграли");
                return; 
            }
            //Закрашивание
            for (i = Top.x-1; i < Top.x + 2; i++)
                Parameters.SetCell(i, 1, color);
            Top.y = 0;
            
        }

        public override bool TickAction()
        {
            if (Status < 3)
            {
                int i;
                //Проверка, может ли фигура упасть целиком:
                if (Status == 1)
                {
                    for (i = Top.x - 1; i < Top.x + 2; i++)
                        if (Parameters.CellInfo(Top.y + 2, i) != 0)
                            return false;
                }
                else if (Status == 2)
                {
                    if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                    if (Parameters.CellInfo(Top.y, Top.x - 1) != 0) return false;
                    if (Parameters.CellInfo(Top.y, Top.x + 1) != 0) return false;
                }
                //стирание
                for (i = Top.x - 1; i < Top.x + 2; i++)
                    Parameters.NullCell(i, Top.y - (int)Math.Pow(-1, Status));
                Parameters.NullCell(Top.x, Top.y);
                // закрашивание
                Top++;
                for (i = Top.x - 1; i < Top.x + 2; i++)
                    Parameters.SetCell(i, Top.y - (int)Math.Pow(-1, Status), color); 
                Parameters.SetCell(Top.x, Top.y, color);
            }
            if (Status > 2)
            {
                int i;
                int temp = (int) Math.Pow(-1, Status);
                //Проверка, может ли фигура упасть целиком:
                if (Parameters.CellInfo(Top.y + 1, Top.x ) != 0) return false;
                if (Parameters.CellInfo(Top.y + 2, Top.x - temp) != 0) return false;
                //Стирание
                for (i = Top.y - 1; i < Top.y + 2; i++)
                    Parameters.NullCell(Top.x - temp, i);
                Parameters.NullCell(Top.x, Top.y);
                // закрашивание
                Top++;
                for (i = Top.y - 1; i < Top.y + 2; i++)
                    Parameters.SetCell(Top.x - temp, i, color);
                Parameters.SetCell(Top.x, Top.y, color);
            }
            return true;
        }
        
        public override bool PositionChange(int pos)
        {
            int temp = (int)Math.Pow(-1, Status);
            int i;
            if (Status > 2)
            { 
                //проверка на возможность
                if (Parameters.CellInfo(Top.y, Top.x - 2 * temp) != 0) return false;
                //Стирание
                Parameters.NullCell(Top.x - temp, Top.y - pos*temp);
                //закрашивание
                Parameters.SetCell(Top.x - 2 * temp, Top.y, color);
                Top.x -= temp;
                Top.y += pos * temp;
                Status = (int) (1.5 + 0.5 * pos * temp);
                return true;
            }
            if (Status < 3)
            {
                if (Status == 1 && Parameters.CellInfo(Top.y+2, Top.x) != 0) return false;
                if (Status == 2 && Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                //стирание
                for (i = Top.x - 1; i < Top.x + 2; i++)
                    Parameters.NullCell(i, Top.y - temp);
                Parameters.NullCell(Top.x, Top.y);
                //переназначение вершины
                if (Status == 1)
                {
                    Top.x += pos;
                    Top.y++;
                    Status = (int)(3.5 + 0.5 * pos);
                }
                else
                {
                    Top.x -= pos;
                    Top.y--;
                    Status = (int)(3.5 - 0.5 * pos);
                }
                // закрашивание
                for (i = Top.y - 1; i < Top.y + 2; i++)
                    Parameters.SetCell(Top.x - (int)Math.Pow(-1, Status), i, color);
                Parameters.SetCell(Top.x, Top.y, color);
                
            }
            return false;
        }
        
        public override bool Move(int plus)
        {
///         // 1 - горизонтальный вверх, 2 - горизонтальный вниз. 3 - вертикальный влево. 4 - верт - вправо.
            int temp= (int)Math.Pow(-1, Status);
            int i;
            if (Status < 3)  // горизонтальное положение
            {
                // проверка, может ли двинуться
                if (Parameters.CellInfo(Top.y, Top.x + plus) != 0) return false;
                if (Parameters.CellInfo(Top.y - temp, Top.x + 2 * plus) != 0) return false;

                //стирание
                for (i = Top.x - 1; i < Top.x + 2; i++)
                    Parameters.NullCell(i, Top.y - temp);
                Parameters.NullCell(Top.x, Top.y);
                Top.x += plus;
                // закрашивание
                for (i = Top.x - 1; i < Top.x + 2; i++)
                    Parameters.SetCell(i, Top.y - temp, color);
                Parameters.SetCell(Top.x, Top.y, color);
                return true;
            }
            if (Status == 3 )
            {
                //проверка
                if (plus > 0)
                {
                    for (i = Top.y - 1; i < Top.y + 2; i++)
                        if (Parameters.CellInfo(i, Top.x + 2) != 0) return false;
                }
                else
                {
                    if (Parameters.CellInfo(Top.y - 1, Top.x) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                    if (Parameters.CellInfo(Top.y, Top.x - 1) != 0) return false;
                }
                //стирание.
                for (i = Top.y - 1; i < Top.y + 2; i++)
                    Parameters.NullCell(Top.x + 1, i);
                Parameters.NullCell(Top.x, Top.y);
                //Закрашивание
                Top.x+=plus;
                for (i = Top.y - 1; i < Top.y + 2; i++)
                    Parameters.SetCell(Top.x + 1, i, color);
                Parameters.SetCell(Top.x, Top.y, color);
                return true;
            }
            if (Status == 4)
            {
                //проверка
                if (plus < 0)
                {
                    for (i = Top.y - 1; i < Top.y + 2; i++)
                        if (Parameters.CellInfo(i, Top.x - 2) != 0) return false;
                }
                else
                {
                    if (Parameters.CellInfo(Top.y - 1, Top.x) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                    if (Parameters.CellInfo(Top.y, Top.x + 1) != 0) return false;
                }
                //стирание.
                for (i = Top.y - 1; i < Top.y + 2; i++)
                    Parameters.NullCell(Top.x - 1, i);
                Parameters.NullCell(Top.x, Top.y);
                //Закрашивание
                Top.x += plus;
                for (i = Top.y - 1; i < Top.y + 2; i++)
                    Parameters.SetCell(Top.x - 1, i, color);
                Parameters.SetCell(Top.x, Top.y, color);
                return true;
            }
            return false;
        }
    }

    class SquareFigure : Figures
    {
        public SquareFigure():base()
        {    
            int i;
            //Проверка, может ли фигура упасть целиком:
            for (i = Top.x; i < Top.x + 2; i++)
                if (Parameters.CellInfo(1, i) != 0)
                {
                    Program.time.Stop();
                    MessageBox.Show("Вы проиграли");
                    return;
                }
            
            //Закрашивание
            for (i = Top.x; i < Top.x + 2; i++)
                Parameters.SetCell(i, 1, color);            
        
        }

        public override bool TickAction()
        {
                int i;
                //Проверка, может ли фигура упасть целиком:
                for (i = Top.x; i < Top.x + 2; i++)
                    if (Parameters.CellInfo(Top.y + 1, i) != 0)
                        return false;
                //стирание
                for (i = Top.x; i < Top.x + 2; i++)
                    Parameters.NullCell(i, Top.y-1);
                
                // закрашивание
                for (i = Top.x; i < Top.x + 2; i++)
                    Parameters.SetCell(i, Top.y + 1, color);
                Top++;
                return true;
        }
        public override bool Move(int plus)
        {
            int i;
            // проверка:
            if (plus > 0)
            {
                for (i = Top.y; i > Top.y - 2; i--)
                    if (Parameters.CellInfo(i, Top.x + 2) != 0) return false;
                //стирание
                for (i = Top.y; i > Top.y - 2; i--)
                    Parameters.NullCell(Top.x, i);
                // закрашивание
                for (i = Top.y; i > Top.y - 2; i--)
                    Parameters.SetCell(Top.x + 2, i, color);
                Top.x += plus;
            }
            else if (plus < 0)
            {
                for (i = Top.y; i > Top.y - 2; i--)
                    if (Parameters.CellInfo(i, Top.x - 1) != 0) return false;
                //стирание
                for (i = Top.y; i > Top.y - 2; i--)
                    Parameters.NullCell(Top.x+1, i);
                // закрашивание
                for (i = Top.y; i > Top.y - 2; i--)
                    Parameters.SetCell(Top.x + plus, i, color);
                Top.x += plus;
            }
            return true;
        }
        
        public override bool PositionChange(int pos)
        {
            return true;
        }
    }

    class FourLeftFigure : Figures
    {
        public FourLeftFigure()
            : base()
        {
            int i;
            Top.x += 2;
            //Проверка, может ли фигура упасть целиком:
            for (i = Top.x-2; i < Top.x; i++)
                if (Parameters.CellInfo(1, i) != 0)
                {
                    Program.time.Stop();
                    MessageBox.Show("Вы проиграли");
                    return;
                }

            //Закрашивание
            for (i = Top.x - 2; i < Top.x; i++)
                Parameters.SetCell(i, 1, color);
            Top.y = 0;
        }

        public override bool TickAction()
        {
            int i;
            if (Status == 1)
            {
                //Проверка, может ли фигура упасть целиком:
                for (i = Top.x - 2; i < Top.x; i++)
                    if (Parameters.CellInfo(Top.y + 2, i) != 0)
                        return false;
                if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                //стирание
                Parameters.NullCell(Top.x - 1, Top.y);
                Parameters.NullCell(Top.x, Top.y);
                Parameters.NullCell(Top.x - 2, Top.y + 1);
                // закрашивание
                Top++;
                Parameters.SetCell(Top.x, Top.y, color);
                Parameters.SetCell(Top.x - 1, Top.y + 1, color);
                Parameters.SetCell(Top.x - 2, Top.y + 1, color);
            }
            else if (Status == 2)
            {
                //Проверка
                if (Parameters.CellInfo(Top.y, Top.x - 1) != 0) return false;
                if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                //Cтирание
                Parameters.NullCell(Top.x - 1, Top.y - 2);
                Parameters.NullCell(Top.x, Top.y - 1);
                //Закрашивание
                Top++;
                Parameters.SetCell(Top.x - 1, Top.y - 1, color);
                Parameters.SetCell(Top.x, Top.y, color);
            }
            return true;
        }

        public override bool Move(int plus)
        {
            if (Status == 1)
            {
                if (plus > 0)
                {
                    // проверка 
                    if (Parameters.CellInfo(Top.y, Top.x + 1) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                    //стирание
                    Parameters.NullCell(Top.x - 2, Top.y + 1);
                    Parameters.NullCell(Top.x - 1, Top.y);
                    // закрашивание
                    Parameters.SetCell(Top.x + 1, Top.y, color);
                    Parameters.SetCell(Top.x, Top.y + 1, color);
                }
                else
                {
                    //проверка 
                    if (Parameters.CellInfo(Top.y, Top.x - 2) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 1, Top.x - 3) != 0) return false;
                    //Стирание
                    Parameters.NullCell(Top.x,Top.y);
                    Parameters.NullCell(Top.x-1,Top.y+1);
                    //закрашивание
                    Parameters.SetCell(Top.x-2,Top.y, color);
                    Parameters.SetCell(Top.x-3,Top.y+1,color);
                }
                Top.x+=plus;
                return true;
            }
            else if (Status == 2)
            {
                if (plus > 0)
                {
                    //Проверка
                    if (Parameters.CellInfo(Top.y - 2, Top.x) != 0) return false;
                    if (Parameters.CellInfo(Top.y - 1, Top.x + 1) != 0) return false;
                    if (Parameters.CellInfo(Top.y, Top.x + 1) != 0) return false;
                    //Стирание
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x - 1, Top.y - 1);
                    Parameters.NullCell(Top.x - 1, Top.y - 2);
                    //Закрашивание
                    Parameters.SetCell(Top.x, Top.y - 2, color);
                    Parameters.SetCell(Top.x + 1, Top.y - 1, color);
                    Parameters.SetCell(Top.x + 1, Top.y, color);
                }
                else if (plus < 0)
                {
                    //Проверка
                    if (Parameters.CellInfo(Top.y - 2, Top.x-2) != 0) return false;
                    if (Parameters.CellInfo(Top.y - 1, Top.x -2) != 0) return false;
                    if (Parameters.CellInfo(Top.y, Top.x - 1) != 0) return false;
                    //Стирание
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x, Top.y - 1);
                    Parameters.NullCell(Top.x - 1, Top.y - 2);
                    //Закрашивание
                    Parameters.SetCell(Top.x- 2, Top.y - 2, color);
                    Parameters.SetCell(Top.x - 2, Top.y - 1, color);
                    Parameters.SetCell(Top.x - 1, Top.y, color);
                }
                Top.x += plus;
                return true;
            }
            return false;
        }

        public override bool PositionChange(int pos)
        {
            if (Status == 1)
            {
                //Проверка 
                if (Parameters.CellInfo(Top.y, Top.x - 2) != 0) return false;
                if (Parameters.CellInfo(Top.y + 2, Top.x - 1) != 0) return false;
                //Стирание
                Parameters.NullCell(Top.x, Top.y);
                Parameters.NullCell(Top.x - 1, Top.y);
                //Закрашивание
                Parameters.SetCell(Top.x - 2, Top.y, color);
                Parameters.SetCell(Top.x - 1, Top.y + 2, color);
                Status = 2;
                Top.y += 2;
                Top.x--;
                return true;
            }
            else if (Status == 2)
            {
                // Проверка
                if (Parameters.CellInfo(Top.y - 2, Top.x) != 0) return false;
                if (Parameters.CellInfo(Top.y - 2, Top.x + 1) != 0) return false;
                //Стирание
                Parameters.NullCell(Top.x, Top.y);
                Parameters.NullCell(Top.x - 1, Top.y - 2);
                //Закрашивание 
                Parameters.SetCell(Top.x, Top.y-2, color);
                Parameters.SetCell(Top.x+1, Top.y-2, color);
                Status = 1;
                Top.y -= 2;
                Top.x++;
                return true;
            }
            return false;
        }
    }

    class FourRightFigure : Figures
    {
           public FourRightFigure()
            : base()
        {
            int i;
            //Top.x += 2;
            //Проверка, может ли фигура упасть целиком:
            for (i = Top.x+1; i < Top.x+3; i++)
                if (Parameters.CellInfo(1, i) != 0)
                {
                    Program.time.Stop();
                    MessageBox.Show("Вы проиграли");
                    return;
                }

            //Закрашивание
            for (i = Top.x +1; i < Top.x+3; i++)
                Parameters.SetCell(i, 1, color);
            Top.y = 0;
        }

        public override bool TickAction()
        {
            int i;
            if (Status == 1)
            {
                //Проверка, может ли фигура упасть целиком:
                for (i = Top.x +1; i < Top.x+3; i++)
                    if (Parameters.CellInfo(Top.y + 2, i) != 0)
                        return false;
                if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                //стирание
                Parameters.NullCell(Top.x + 1, Top.y);
                Parameters.NullCell(Top.x, Top.y);
                Parameters.NullCell(Top.x + 2, Top.y + 1);
                // закрашивание
                Top++;
                Parameters.SetCell(Top.x, Top.y, color);
                Parameters.SetCell(Top.x + 1, Top.y + 1, color);
                Parameters.SetCell(Top.x + 2, Top.y + 1, color);
            }
            else if (Status == 2)
            {
                //Проверка
                if (Parameters.CellInfo(Top.y, Top.x + 1) != 0) return false;
                if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                //Cтирание
                Parameters.NullCell(Top.x + 1, Top.y - 2);
                Parameters.NullCell(Top.x, Top.y - 1);
                //Закрашивание
                Top++;
                Parameters.SetCell(Top.x + 1, Top.y - 1, color);
                Parameters.SetCell(Top.x, Top.y, color);
            }
            return true;
        }

        public override bool Move(int plus)
        {
            if (Status == 1)
            {
                if (plus < 0)
                {
                    // проверка 
                    if (Parameters.CellInfo(Top.y, Top.x - 1) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                    //стирание
                    Parameters.NullCell(Top.x + 2, Top.y + 1);
                    Parameters.NullCell(Top.x + 1, Top.y);
                    // закрашивание
                    Parameters.SetCell(Top.x - 1, Top.y, color);
                    Parameters.SetCell(Top.x, Top.y + 1, color);
                }
                else
                {
                    //проверка 
                    if (Parameters.CellInfo(Top.y, Top.x + 2) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 1, Top.x + 3) != 0) return false;
                    //Стирание
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x + 1, Top.y + 1);
                    //закрашивание
                    Parameters.SetCell(Top.x + 2, Top.y, color);
                    Parameters.SetCell(Top.x + 3, Top.y + 1, color);
                }
                Top.x += plus;
                return true;
            }
            else if (Status == 2)
            {
                if (plus < 0)
                {
                    //Проверка
                    if (Parameters.CellInfo(Top.y - 2, Top.x) != 0) return false;
                    if (Parameters.CellInfo(Top.y - 1, Top.x - 1) != 0) return false;
                    if (Parameters.CellInfo(Top.y, Top.x - 1) != 0) return false;
                    //Стирание
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x + 1, Top.y - 1);
                    Parameters.NullCell(Top.x + 1, Top.y - 2);
                    //Закрашивание
                    Parameters.SetCell(Top.x, Top.y - 2, color);
                    Parameters.SetCell(Top.x - 1, Top.y - 1, color);
                    Parameters.SetCell(Top.x - 1, Top.y, color);
                }
                else if (plus > 0)
                {
                    //Проверка
                    if (Parameters.CellInfo(Top.y - 2, Top.x + 2) != 0) return false;
                    if (Parameters.CellInfo(Top.y - 1, Top.x + 2) != 0) return false;
                    if (Parameters.CellInfo(Top.y, Top.x + 1) != 0) return false;
                    //Стирание
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x, Top.y - 1);
                    Parameters.NullCell(Top.x + 1, Top.y - 2);
                    //Закрашивание
                    Parameters.SetCell(Top.x + 2, Top.y - 2, color);
                    Parameters.SetCell(Top.x + 2, Top.y - 1, color);
                    Parameters.SetCell(Top.x + 1, Top.y, color);
                }
                Top.x += plus;
                return true;
            }
            return false;
        }

        public override bool PositionChange(int pos)
        {
            if (Status == 1)
            {
                //Проверка 
                if (Parameters.CellInfo(Top.y, Top.x + 2) != 0) return false;
                if (Parameters.CellInfo(Top.y + 2, Top.x + 1) != 0) return false;
                //Стирание
                Parameters.NullCell(Top.x, Top.y);
                Parameters.NullCell(Top.x + 1, Top.y);
                //Закрашивание
                Parameters.SetCell(Top.x + 2, Top.y, color);
                Parameters.SetCell(Top.x + 1, Top.y + 2, color);
                Status = 2;
                Top.y += 2;
                Top.x++;
                return true;
            }
            else if (Status == 2)
            {
                // Проверка
                if (Parameters.CellInfo(Top.y - 2, Top.x) != 0) return false;
                if (Parameters.CellInfo(Top.y - 2, Top.x - 1) != 0) return false;
                //Стирание
                Parameters.NullCell(Top.x, Top.y);
                Parameters.NullCell(Top.x + 1, Top.y - 2);
                //Закрашивание 
                Parameters.SetCell(Top.x, Top.y - 2, color);
                Parameters.SetCell(Top.x - 1, Top.y - 2, color);
                Status = 1;
                Top.y -= 2;
                Top.x--;
                return true;
            }
            return false;
        }
    }

    class HorseLeftFigure : Figures
    {
        public HorseLeftFigure()
            : base()
        {
            int i;
            Random rnd = new Random();       
            //Проверка, может ли фигура упасть целиком:
            for (i = Top.x; i < Top.x + 3; i++)
                if (Parameters.CellInfo(1, i) != 0)
                {
                    Program.time.Stop();
                    MessageBox.Show("Вы проиграли");
                    return;
                }
            //Закрашивание
            for (i = Top.x; i < Top.x + 3; i++)
                Parameters.SetCell(i, 1, color);
            Top.y = 0;
        }
        public override bool TickAction()
        {
            int i;
            switch (Status)
            {
                case 1:    //Проверка, может ли фигура упасть целиком:
                    for (i = Top.x; i < Top.x + 3; i++)
                        if (Parameters.CellInfo(Top.y + 2, i) != 0)
                            return false;
                    //Cтирание
                    for (i = Top.x; i < Top.x + 3; i++)
                        Parameters.NullCell(i, Top.y + 1);
                    Parameters.NullCell(Top.x, Top.y);
                    //Закрашивание
                    Top++;
                    for (i = Top.x; i < Top.x + 3; i++)
                        Parameters.SetCell(i, Top.y + 1, color);
                    Parameters.SetCell(Top.x, Top.y, color);
                    break;
                case 2:
                    //Проверка, может ли фигура упасть целиком:
                    if (Parameters.CellInfo(Top.y, Top.x - 2) != 0) return false;
                    if (Parameters.CellInfo(Top.y, Top.x - 1) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                    //Cтирание
                    Parameters.NullCell(Top.x - 2, Top.y - 1);
                    Parameters.NullCell(Top.x - 1, Top.y - 1);
                    Parameters.NullCell(Top.x, Top.y - 1);
                    //Закрашивание
                    Parameters.SetCell(Top.x - 2, Top.y, color);
                    Parameters.SetCell(Top.x - 1, Top.y, color);
                    Parameters.SetCell(Top.x, Top.y + 1, color);
                    Top++;
                    break;
                case 4:
                    // Проверка
                    if (Parameters.CellInfo(Top.y + 3, Top.x - 1) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                    //Стирание
                    for (i = Top.y; i < Top.y + 3; i++)
                        Parameters.NullCell(Top.x - 1, i);
                    Parameters.NullCell(Top.x, Top.y);
                    Top++;
                    for (i = Top.y; i < Top.y + 3; i++)
                        Parameters.SetCell(Top.x - 1, i, color);
                    Parameters.SetCell(Top.x, Top.y, color);
                    break;
                case 3:
                    // Проверка
                    if (Parameters.CellInfo(Top.y + 1, Top.x + 1) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                    //Стирание
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x + 1, Top.y - 2);
                    Top++;
                    Parameters.SetCell(Top.x + 1, Top.y, color);
                    Parameters.SetCell(Top.x, Top.y, color);
                    break;
            }
            
            return true;
        }
        public override bool Move(int plus)
        {
            switch (Status)
            {
                case 1:
                    if (plus < 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x - 1) != 0) return false;
                        if (Parameters.CellInfo(Top.y + 1, Top.x - 1) != 0) return false;

                        Parameters.NullCell(Top.x, Top.y);
                        Parameters.NullCell(Top.x + 2, Top.y + 1);

                        Parameters.SetCell(Top.x - 1, Top.y, color);
                        Parameters.SetCell(Top.x - 1, Top.y + 1, color);
                    }
                    else if (plus > 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x + 1) != 0) return false;
                        if (Parameters.CellInfo(Top.y + 1, Top.x + 3) != 0) return false;
                        
                        Parameters.NullCell(Top.x, Top.y);
                        Parameters.NullCell(Top.x, Top.y + 1);

                        Parameters.SetCell(Top.x + 1, Top.y, color);
                        Parameters.SetCell(Top.x + 3, Top.y + 1, color);
                    }
                    break;
                case 2:
                    if (plus > 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x + 1) != 0) return false;
                        if (Parameters.CellInfo(Top.y - 1, Top.x + 1) != 0) return false;

                        Parameters.NullCell(Top.x, Top.y);
                        Parameters.NullCell(Top.x - 2, Top.y - 1);

                        Parameters.SetCell(Top.x + 1, Top.y, color);
                        Parameters.SetCell(Top.x + 1, Top.y - 1, color);
                    }
                    else if (plus < 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x - 1) != 0) return false;
                        if (Parameters.CellInfo(Top.y - 1, Top.x - 3) != 0) return false;
                        
                        Parameters.NullCell(Top.x, Top.y);
                        Parameters.NullCell(Top.x, Top.y - 1);

                        Parameters.SetCell(Top.x - 1, Top.y, color);
                        Parameters.SetCell(Top.x - 3, Top.y - 1, color);
                    }
                    break;
                case 3:
                    if (plus > 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x + 2) != 0) return false;
                        if (Parameters.CellInfo(Top.y - 1, Top.x + 2) != 0) return false;
                        if (Parameters.CellInfo(Top.y - 2, Top.x + 2) != 0) return false;

                        Parameters.NullCell(Top.x, Top.y);
                        Parameters.NullCell(Top.x + 1, Top.y - 1);
                        Parameters.NullCell(Top.x + 1, Top.y - 2);

                        Parameters.SetCell(Top.x + 2, Top.y, color);
                        Parameters.SetCell(Top.x + 2, Top.y - 1, color);
                        Parameters.SetCell(Top.x + 2, Top.y - 2, color);
                    }
                    if (plus < 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x - 1) != 0) return false;
                        if (Parameters.CellInfo(Top.y - 1, Top.x) != 0) return false;
                        if (Parameters.CellInfo(Top.y - 2, Top.x) != 0) return false;

                        Parameters.NullCell(Top.x + 1, Top.y);
                        Parameters.NullCell(Top.x + 1, Top.y - 1);
                        Parameters.NullCell(Top.x + 1, Top.y - 2);

                        Parameters.SetCell(Top.x - 1, Top.y, color);
                        Parameters.SetCell(Top.x, Top.y - 1, color);
                        Parameters.SetCell(Top.x, Top.y - 2, color);
                    }
                    break;
                case 4:
                    if (plus < 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x - 2) != 0) return false;
                        if (Parameters.CellInfo(Top.y + 1, Top.x - 2) != 0) return false;
                        if (Parameters.CellInfo(Top.y + 2, Top.x - 2) != 0) return false;

                        Parameters.NullCell(Top.x, Top.y);
                        Parameters.NullCell(Top.x - 1, Top.y + 1);
                        Parameters.NullCell(Top.x - 1, Top.y + 2);

                        Parameters.SetCell(Top.x - 2, Top.y, color);
                        Parameters.SetCell(Top.x - 2, Top.y + 1, color);
                        Parameters.SetCell(Top.x - 2, Top.y + 2, color);
                    }
                    if (plus > 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x + 1) != 0) return false;
                        if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                        if (Parameters.CellInfo(Top.y + 2, Top.x) != 0) return false;

                        Parameters.NullCell(Top.x - 1, Top.y);
                        Parameters.NullCell(Top.x - 1, Top.y + 1);
                        Parameters.NullCell(Top.x - 1, Top.y + 2);

                        Parameters.SetCell(Top.x + 1, Top.y, color);
                        Parameters.SetCell(Top.x, Top.y + 1, color);
                        Parameters.SetCell(Top.x, Top.y + 2, color);
                    }
                    break;
            }
            Top.x += plus;
            return true;
        }
        public override bool PositionChange(int pos)
        {
            switch (Status)
            {
                case  1:
                    if (Parameters.CellInfo(Top.y, Top.x+1) != 0) return false;
                    if (Parameters.CellInfo(Top.y - 1, Top.x + 1) != 0) return false;
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x + 2, Top.y + 1);
                    Parameters.SetCell(Top.x+1, Top.y, color);
                    Parameters.SetCell(Top.x + 1, Top.y - 1, color);
                    Status = 3;
                    Top.y++;
                    break;
                case 2:
                    if (Parameters.CellInfo(Top.y, Top.x-1) != 0) return false;
                    if (Parameters.CellInfo(Top.y - 2, Top.x - 1) != 0) return false;
                    if (Parameters.CellInfo(Top.y - 2, Top.x) != 0) return false;
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x - 2, Top.y - 1);
                    Parameters.NullCell(Top.x, Top.y - 1);
                    Parameters.SetCell(Top.x-1, Top.y, color);
                    Parameters.SetCell(Top.x - 1, Top.y - 2, color);
                    Parameters.SetCell(Top.x, Top.y - 2, color);
                    Status = 4;
                    Top.y-=2;
                    break;
                case 3:
                    if (Parameters.CellInfo(Top.y - 1, Top.x) != 0) return false;
                    if (Parameters.CellInfo(Top.y - 1, Top.x+2) != 0) return false;
                    if (Parameters.CellInfo(Top.y, Top.x + 2) != 0) return false;
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x + 1, Top.y);
                    Parameters.NullCell(Top.x + 1, Top.y - 2);
                    Parameters.SetCell(Top.x, Top.y - 1, color);
                    Parameters.SetCell(Top.x + 2, Top.y - 1, color);
                    Parameters.SetCell(Top.x + 2, Top.y, color);
                    Status = 2;
                    Top.x += 2;
                    break;
                case 4:
                    if (Parameters.CellInfo(Top.y + 2, Top.x) != 0) return false;
                    if (Parameters.CellInfo(Top.y+1, Top.x - 2) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 2, Top.x - 2) != 0) return false;
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x - 1, Top.y);
                    Parameters.NullCell(Top.x - 1, Top.y + 1);
                    Parameters.SetCell(Top.x - 2, Top.y+1, color);
                    Parameters.SetCell(Top.x - 2, Top.y + 2, color);
                    Parameters.SetCell(Top.x, Top.y + 2, color);
                    Top.x -= 2;
                    Top.y+=1;
                    Status = 1;
                    break;
                default : throw new NotImplementedException(); 
                    
            }
            return true;
        }
    }

    class HorseFigure : Figures
    {
        int RlyOrient; // 1- вправо
        //для horse переопределяем  положения.3 - вниз 2 штуки, 4 - вверх 2 штуки    
        public HorseFigure(int RlyOR)
            : base()
        {
            RlyOrient = RlyOR;
            Random rnd = new Random();       
            //Проверка, может ли фигура упасть целиком:
            if ((Parameters.CellInfo(1, Top.x) != 0) ||
             (Parameters.CellInfo(1, Top.x - RlyOrient) != 0) ||
             (Parameters.CellInfo(1, Top.x - 2*RlyOrient) != 0))
            {
                Program.time.Stop();
                MessageBox.Show("Вы проиграли");
                return;
            }
   
            //Закрашивание
            Parameters.SetCell(Top.x, 1,color);
            Parameters.SetCell(Top.x - RlyOrient, 1, color);
            Parameters.SetCell(Top.x - 2 * RlyOrient, 1, color);
            Top.y = 0;
        }
        public override bool TickAction()
        {
            switch (Status)
            {
                case 1:    //Проверка, может ли фигура упасть целиком:
                    if (Parameters.CellInfo(Top.y + 2, Top.x) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 2, Top.x - RlyOrient) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 2, Top.x - 2 * RlyOrient) != 0) return false;
                    //Cтирание
                    Parameters.NullCell(Top.x, Top.y );
                    Parameters.NullCell(Top.x - RlyOrient, Top.y + 1);
                    Parameters.NullCell(Top.x - 2 * RlyOrient, Top.y + 1);
                    //Закрашивание
                    Parameters.SetCell(Top.x, Top.y+2, color);
                    Parameters.SetCell(Top.x -RlyOrient, Top.y + 2, color);
                    Parameters.SetCell(Top.x - 2 * RlyOrient, Top.y + 2, color);
                    Top++;
                    break;
                case 2:
                    //Проверка, может ли фигура упасть целиком:
                    if (Parameters.CellInfo(Top.y, Top.x + RlyOrient) != 0) return false;
                    if (Parameters.CellInfo(Top.y, Top.x + 2*RlyOrient) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                    //Cтирание
                    Parameters.NullCell(Top.x + 2*RlyOrient, Top.y - 1);
                    Parameters.NullCell(Top.x +RlyOrient, Top.y - 1);
                    Parameters.NullCell(Top.x, Top.y - 1);
                    //Закрашивание
                    Parameters.SetCell(Top.x + 2*RlyOrient, Top.y, color);
                    Parameters.SetCell(Top.x + RlyOrient, Top.y, color);
                    Parameters.SetCell(Top.x, Top.y + 1, color);
                    Top++;
                    break;
                case 4:
                    // Проверка
                    if (Parameters.CellInfo(Top.y + 3, Top.x + RlyOrient) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                    //Стирание
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x + RlyOrient, Top.y);
                    Top++;
                    Parameters.SetCell(Top.x, Top.y, color);
                    Parameters.SetCell(Top.x+RlyOrient, Top.y + 2, color);
                    break;
                case 3:
                    // Проверка
                    if (Parameters.CellInfo(Top.y + 1, Top.x - RlyOrient) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                    //Стирание
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x - RlyOrient, Top.y - 2);
                    Top++;
                    Parameters.SetCell(Top.x - RlyOrient, Top.y, color);
                    Parameters.SetCell(Top.x, Top.y, color);
                    break;
            }
            
            return true;
        }
        public override bool Move(int plus)
        {
            switch (Status)
            {
                case 1:
                    if (plus*RlyOrient>0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x + RlyOrient) != 0) return false;
                        if (Parameters.CellInfo(Top.y + 1, Top.x + RlyOrient) != 0) return false;

                        Parameters.NullCell(Top.x, Top.y);
                        Parameters.NullCell(Top.x - 2*RlyOrient, Top.y + 1);

                        Parameters.SetCell(Top.x + RlyOrient, Top.y, color);
                        Parameters.SetCell(Top.x +RlyOrient, Top.y + 1, color);
                    }
                    else if (plus*RlyOrient < 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x - RlyOrient) != 0) return false;
                        if (Parameters.CellInfo(Top.y + 1, Top.x - 3*RlyOrient) != 0) return false;
                        
                        Parameters.NullCell(Top.x, Top.y);
                        Parameters.NullCell(Top.x, Top.y + 1);

                        Parameters.SetCell(Top.x - RlyOrient, Top.y, color);
                        Parameters.SetCell(Top.x - 3*RlyOrient, Top.y + 1, color);
                    }
                    break;
                case 2:
                    if (plus*RlyOrient < 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x - RlyOrient) != 0) return false;
                        if (Parameters.CellInfo(Top.y - 1, Top.x - RlyOrient) != 0) return false;

                        Parameters.NullCell(Top.x, Top.y);
                        Parameters.NullCell(Top.x + 2*RlyOrient, Top.y - 1);

                        Parameters.SetCell(Top.x - RlyOrient, Top.y, color);
                        Parameters.SetCell(Top.x - RlyOrient, Top.y - 1, color);
                    }
                    else if (plus*RlyOrient > 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x + RlyOrient) != 0) return false;
                        if (Parameters.CellInfo(Top.y - 1, Top.x + 3*RlyOrient) != 0) return false;
                        
                        Parameters.NullCell(Top.x, Top.y);
                        Parameters.NullCell(Top.x, Top.y - 1);

                        Parameters.SetCell(Top.x + RlyOrient, Top.y, color);
                        Parameters.SetCell(Top.x + 3*RlyOrient, Top.y - 1, color);
                    }
                    break;
                case 3:
                    if (plus*RlyOrient < 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x - 2*RlyOrient) != 0) return false;
                        if (Parameters.CellInfo(Top.y - 1, Top.x - 2*RlyOrient) != 0) return false;
                        if (Parameters.CellInfo(Top.y - 2, Top.x - 2*RlyOrient) != 0) return false;

                        Parameters.NullCell(Top.x, Top.y);
                        Parameters.NullCell(Top.x - RlyOrient, Top.y - 1);
                        Parameters.NullCell(Top.x - RlyOrient, Top.y - 2);
                        
                        Parameters.SetCell(Top.x - 2*RlyOrient, Top.y, color);
                        Parameters.SetCell(Top.x - 2*RlyOrient, Top.y - 1, color);
                        Parameters.SetCell(Top.x - 2*RlyOrient, Top.y - 2, color);
                    }
                    if (plus*RlyOrient > 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x + RlyOrient) != 0) return false;
                        if (Parameters.CellInfo(Top.y - 1, Top.x) != 0) return false;
                        if (Parameters.CellInfo(Top.y - 2, Top.x) != 0) return false;

                        Parameters.NullCell(Top.x - RlyOrient, Top.y);
                        Parameters.NullCell(Top.x - RlyOrient, Top.y - 1);
                        Parameters.NullCell(Top.x - RlyOrient, Top.y - 2);

                        Parameters.SetCell(Top.x + RlyOrient, Top.y, color);
                        Parameters.SetCell(Top.x, Top.y - 1, color);
                        Parameters.SetCell(Top.x, Top.y - 2, color);
                    }
                    break;
                case 4:
                    if (plus*RlyOrient > 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x + 2*RlyOrient) != 0) return false;
                        if (Parameters.CellInfo(Top.y + 1, Top.x + 2*RlyOrient) != 0) return false;
                        if (Parameters.CellInfo(Top.y + 2, Top.x + 2*RlyOrient) != 0) return false;

                        Parameters.NullCell(Top.x, Top.y);
                        Parameters.NullCell(Top.x + RlyOrient, Top.y + 1);
                        Parameters.NullCell(Top.x + RlyOrient, Top.y + 2);

                        Parameters.SetCell(Top.x + 2*RlyOrient, Top.y, color);
                        Parameters.SetCell(Top.x + 2*RlyOrient, Top.y + 1, color);
                        Parameters.SetCell(Top.x + 2*RlyOrient, Top.y + 2, color);
                    }
                    if (plus*RlyOrient < 0)
                    {
                        if (Parameters.CellInfo(Top.y, Top.x - RlyOrient) != 0) return false;
                        if (Parameters.CellInfo(Top.y + 1, Top.x) != 0) return false;
                        if (Parameters.CellInfo(Top.y + 2, Top.x) != 0) return false;

                        Parameters.NullCell(Top.x + RlyOrient, Top.y);
                        Parameters.NullCell(Top.x + RlyOrient, Top.y + 1);
                        Parameters.NullCell(Top.x + RlyOrient, Top.y + 2);

                        Parameters.SetCell(Top.x - RlyOrient, Top.y, color);
                        Parameters.SetCell(Top.x, Top.y + 1, color);
                        Parameters.SetCell(Top.x, Top.y + 2, color);
                    }
                    break;
            }
            Top.x += plus;
            return true;
        }
        public override bool PositionChange(int pos)
        {
            switch (Status)
            {
                case  1:
                    if (Parameters.CellInfo(Top.y, Top.x - RlyOrient) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 2, Top.x - RlyOrient) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 1 - RlyOrient, Top.x-1-RlyOrient) != 0) return false;
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x, Top.y + 1);
                    Parameters.NullCell(Top.x  - 2*RlyOrient,Top.y + 1);
                    Parameters.SetCell(Top.x - RlyOrient, Top.y,color);
                    Parameters.SetCell(Top.x - RlyOrient, Top.y + 2,color);
                    Parameters.SetCell(Top.x - 1 - RlyOrient, Top.y + 1 - RlyOrient, color);
                    Status = (int)(3.5 + (double)RlyOrient / 2);
                    if (RlyOrient == 1) Top.x -= 2;
                    else if (RlyOrient == -1) Top.y += 2;
                    break;
                case 2:
                    if (Parameters.CellInfo(Top.y - 2, Top.x + RlyOrient) != 0) return false;
                    if (Parameters.CellInfo(Top.y, Top.x + RlyOrient) != 0) return false;
                    if (Parameters.CellInfo(Top.y - 1 + RlyOrient, Top.x + RlyOrient + 1) != 0) return false;
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x, Top.y - 1);
                    Parameters.NullCell(Top.x+2*RlyOrient, Top.y-1);
                    Parameters.SetCell(Top.x + RlyOrient, Top.y-2, color);
                    Parameters.SetCell(Top.x+RlyOrient, Top.y, color);
                    Parameters.SetCell(Top.x+1+RlyOrient, Top.y - 1+RlyOrient, color);
                    Status = (int)(3.5 - (double)RlyOrient / 2);
                    if (RlyOrient == 1) Top.x += 2;
                    else if (RlyOrient == -1) Top.y -= 2;
                    break;
                case 3:
                    if (Parameters.CellInfo(Top.y - 1-RlyOrient, Top.x+1-RlyOrient) != 0) return false;
                    if (Parameters.CellInfo(Top.y - 1, Top.x+1-RlyOrient) != 0) return false;
                    if (Parameters.CellInfo(Top.y - 1, Top.x - 1 - RlyOrient) != 0) return false;

                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x - RlyOrient, Top.y);
                    Parameters.NullCell(Top.x - RlyOrient, Top.y - 2);

                    Parameters.SetCell(Top.x + 1 - RlyOrient, Top.y - 1 - RlyOrient, color);
                    Parameters.SetCell(Top.x + 1 - RlyOrient, Top.y - 1, color);
                    Parameters.SetCell(Top.x - 1 - RlyOrient, Top.y - 1, color);

                    Status = (int)(1.5 - (double)RlyOrient / 2);
                    if (RlyOrient == 1) Top.y -= 2;
                    else if (RlyOrient == -1) Top.x += 2;
                    break;
                case 4:
                    if (Parameters.CellInfo(Top.y + 1+RlyOrient, Top.x-1+RlyOrient) != 0) return false;
                    if (Parameters.CellInfo(Top.y+1, Top.x - 1+RlyOrient) != 0) return false;
                    if (Parameters.CellInfo(Top.y + 1, Top.x + 1+RlyOrient) != 0) return false;
                    Parameters.NullCell(Top.x, Top.y);
                    Parameters.NullCell(Top.x+RlyOrient, Top.y);
                    Parameters.NullCell(Top.x+RlyOrient ,Top.y +2);
                    Parameters.SetCell(Top.x-1+RlyOrient, Top.y+1+RlyOrient, color);
                    Parameters.SetCell(Top.x-1+RlyOrient, Top.y + 1, color);
                    Parameters.SetCell(Top.x+1+RlyOrient, Top.y + 1, color);
                    Status = (int)(1.5 + (double)RlyOrient / 2);
                    if (RlyOrient == 1) Top.y += 2;
                    else if (RlyOrient == -1) Top.x -= 2;
                    break;
            }
            return true;
        }
        
    }

    class Coord
    {
        public int x;
        public int y;
        public Coord()
        {
            x = Parameters.vpravo /2-2;
            y = 1;
        }
        public static Coord operator ++(Coord op)
        {
            op.y++;
            return op;
        }
    }

    static class MiniPics
    {
        public static Graphics gr;        
        static Brush er, p;

        static MiniPics()
        {
            gr = Program.Test.CreateGraphics();
            er = new SolidBrush(Color.White);
        }
        public static void ShowJpg(int num, int RlyPos)
        {
            p = new SolidBrush(Color.Black);
            gr.FillRectangle(er, 200, 442, 90, 20); 
            switch (num)
            {
                case 0: //Line
                    for (int i = 0; i < 4; i++)
                        gr.FillRectangle(p, 221+10*i, 452 , 9, 10);
                    break;
                case 1: ///triangle
                    for (int i = 0; i < 3; i++)
                        gr.FillRectangle(p, 221 + 10 * i, 452, 9, 10);
                    gr.FillRectangle(p, 231, 442, 9, 9);
                    break;
                case 2: //square
                    for (int i = 1; i < 3; i++)
                    {
                        gr.FillRectangle(p, 221 + 10 * i, 452, 9, 10);
                        gr.FillRectangle(p, 221 + 10 * i, 442, 9, 9);
                    }
                    break;
                case 3: //закорючка 
                    for (int i = 1; i < 3; i++)
                    {
                        gr.FillRectangle(p, 221 + 10 * (int)(i+0.5-(double)RlyPos/2), 452, 9, 10);
                        gr.FillRectangle(p, 221 + 10 * (int)(i + 0.5 + (double)RlyPos / 2), 442, 9, 9);
                    }
                    break;
                case 4: //Horse
                    gr.FillRectangle(p, 241+RlyPos*10, 442, 9, 9);
                    for(int i = 1; i<4;i++)
                        gr.FillRectangle(p, 221 + 10 * i, 452, 9, 10);
                    break;
            }
        }
    }
}
