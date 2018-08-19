//Массив поля на 1 вниз больше! Сделано для того, чтоб вмещать некоторые фигуры
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Tetris
{
    static class Parameters
    {
        public static Color [] colors; // Здесь цвета для падающих квадратов.
        public static int vniz, vpravo;
        static int[,] setka;
        static bool grid; // рисуем или нет сетку. 
        //Для рисования
        public static Graphics gr;
        public static int SizeOfCell;
        public static Brush p, er;

    //Методы и свойства      
        static Parameters()
        {
            gr = Program.Test.CreateGraphics();
            er = new SolidBrush(Color.White);  // цвет линии и ширина
            grid = false;
            vniz = 16;
            vpravo = 12;
            colors = new Color[9] { 
                Color.Snow,
                Color.Orange, Color.Blue, Color.Yellow, Color.Green, 
                Color.Brown, Color.DarkBlue, Color.HotPink, Color.Pink
            };
            SizeOfCell = 300 / vpravo;
            SetMass();
        }
        //Следующая функция создает массив размерами vniz*vpravo
        public static void SetMass()
        {  //устанавливает сетку с новыми размерами
            try
            {
                setka = new int[vniz+1, vpravo];
                SizeOfCell = 300 / vpravo;
                for (int i = 0; i < vniz+1; i++)
                    for (int j = 0; j < vpravo; j++)
                        setka[i, j] = 0; // не заняты ничем

            }
            catch (OutOfMemoryException)
            {
                MessageBox.Show("Не хватило памяти, ребята");
            }
            catch (Exception)
            {
                MessageBox.Show("Произошла какая-то неизвестная ошибка. Уведомите об этом создателя");
            }
        }

        //Устанавливает значение элемента по координатам Xcoord, Ycoord
        //как num. num = 0 - клетка пустая, num !=0 - определенного цвета (см. массив colors)
        public static bool SetCell(int Xcoord, int Ycoord, int num)
        {
            try
            {
                if (setka[Ycoord, Xcoord] != 0) throw new BusyCellException();
                else
                {
                    setka[Ycoord, Xcoord] = num;
                    if (Ycoord>0)
                    gr.FillRectangle(p, 21 + SizeOfCell * Xcoord, 41 + SizeOfCell * (Ycoord - 1),
                        SizeOfCell - 1, SizeOfCell - 1);
                }
            }
            catch (IndexOutOfRangeException)
            {
                MessageBox.Show("Неправильная работа с массивом в методе SetCell. Сообщите об этом программисту");
                return false;
            }
            catch (BusyCellException)
            {
                MessageBox.Show("Все-таки нельзя проходить сквозь стены");
                return false;
            }
            return true;
        }
        //Обнуление элемента.
        public static void NullCell(int Xcoord, int Ycoord)
        {
            setka[Ycoord, Xcoord] = 0;
            gr.FillRectangle(er, 21 + SizeOfCell * Xcoord, 41 + SizeOfCell * (Ycoord - 1),
                        SizeOfCell - 1, SizeOfCell - 1);
        }
        //Узнаем, нужно ли рисовать сетку. 
        public static bool GridView
        {
            get
            {
                return grid;
            }
            set
            {
                grid = value;
            }
        }
        //Получение информации о ячейке.
        public static int CellInfo(int i, int j)
        {
            try
            {
                return setka[i, j];
            }
            catch (IndexOutOfRangeException)
            {
                return 255;// Если неправильно использован метод, то чтоб не выходило за границы массива возвращаем ошибку 255
            }
        }
        //Очистка строки.
        public static int LineBreak()
        {
            int i, j, k;
            int scores = 0;
            Brush nb;
            // проверка, какие строчки надо смещать куда
            for (i = vniz; i > 0; i--)
            {
                for (j = 0; j < vpravo; j++)
                    if (setka[i, j] == 0) break; 

                if (j == vpravo)
                {
                    for (k = i-1; k >= 0; k--)
                        for (j = 0; j < vpravo; j++)
                        {
                            nb = new SolidBrush(colors[setka[k, j]]);
                            setka[k + 1, j] = setka[k, j];    
                            gr.FillRectangle(nb, 21 + SizeOfCell * j, 41 + SizeOfCell * k,
                                SizeOfCell - 1, SizeOfCell - 1);
                        }
                    i++;
                    scores += 100;
                }
            }
            return scores;
           
        }
        //Скрывает при паузе элементы
        public static void HideScreen()
        {
            gr.FillRectangle(er, 21, 41, SizeOfCell * vpravo-1, vniz * SizeOfCell-1);
        }
    }
    //Ошибка, обозначающее, что ячейка уже занята
    class BusyCellException : Exception
    {
    }
}
