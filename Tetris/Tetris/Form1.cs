using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Tetris
{
    public partial class Form1 : Form
    {   
        public Form1()
        {
            InitializeComponent();
        }
        public void ScoreSet(long arg)
        {
            label2.Text = arg.ToString();
        }
        private void выходToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if ( Parameters.gr != null) Parameters.gr.Dispose();
            MiniPics.gr.Dispose();
            Application.Exit();
        }
        private void задатьПараметрыToolStripMenuItem_Click(object sender, EventArgs e)
        {
            setup SetForm = new setup();
            SetForm.Show();
        }
        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            Graphics gr = e.Graphics;
            Pen p = new Pen(Color.Black, 1); // цвет линии и ширина
            int j; int i;
            int plus = 300 / Parameters.vpravo; //приращение для нашей сетки.
            int MaxVnizPx = 40 + Parameters.vniz * plus;
            
            //Прорисовка сетки
             if (Parameters.GridView) 
             {
                //Вертикальныые линии сетки:
                for (i = 20+plus; i <= 320-plus ; i += plus)
                    gr.DrawLine(p, i, 40, i, MaxVnizPx); // вертикальная
                //Горизонтальные линии сетки:
                for (j = 40+plus; j <= MaxVnizPx-plus ; j += plus)
                    gr.DrawLine(p, 20, j, 20+ plus*Parameters.vpravo, j); // горизонтальная                
             }
             //Прорисовка рамки
             gr.DrawRectangle(p, 20, 40, plus*Parameters.vpravo, MaxVnizPx-40);

            // Тут рисуем все  квадратики, которые закрашены.
             if (Program.time.Enabled)
                 for (i = 1; i <= Parameters.vniz; i++)
                     for (j = 0; j < Parameters.vpravo; j++)
                     {
                         if (Parameters.CellInfo(i, j) != 0 && Parameters.CellInfo(i, j) != 255)
                             FillSquare(ref e, ref gr, i - 1, j);
                     }
            if(Program.time.Enabled) MiniPics.ShowJpg(Program.NextF, Program.NextFRlyPos);
            gr.Dispose();// освобождаем все ресурсы, связанные с отрисовкой
            
        }
        //закрашивание квадратика
        private void FillSquare(ref PaintEventArgs e, ref Graphics gr, int r, int c)
        {   
            int temp = 300 / Parameters.vpravo;
            //r,c - координаты элемента в массиве. с - колонка. 
            Brush p = new SolidBrush(Parameters.colors[Parameters.CellInfo(r+1, c)]);  // цвет линии и ширина
            gr.FillRectangle(p, 21 + temp * c, 41 + temp * r, temp - 1, temp - 1);
        }
        //Обработка нажатий клавиш
        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            if (Program.Figure != null)
            {
                if (Program.time.Enabled)
                    switch (e.KeyCode)
                    {
                        case Keys.Left:
                            Program.Figure.Move(-1);
                            break;
                        case Keys.Right:
                            Program.Figure.Move(+1);
                            break;
                        case Keys.Up:
                            Program.Figure.PositionChange(-1);
                            break;
                        case Keys.Down:
                            Program.time.Interval = Program.WaitTime / (6 * (Levels.NumOfDifficulties + 1 - Levels.Difficult));
                            break;
                        case Keys.P:
                        case Keys.Pause:
                            Program.time.Stop();
                            Parameters.HideScreen();
                            label3.Visible = true;
                            break;
                        case Keys.G:
                            Parameters.GridView = !Parameters.GridView;
                            Program.Test.Refresh();
                            break;
                    }
                else if (e.KeyCode == Keys.Pause || e.KeyCode == Keys.P)
                {
                    Program.time.Start();
                    label3.Visible = false;
                    Program.Test.Refresh();
                }
            }
        }

        private void Form1_KeyUp(object sender, KeyEventArgs e)
        {
            Program.time.Interval = Program.WaitTime;
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            MessageBox.Show(" Управление на стрелках\n Pause break(p) - пауза\\возобновление\n G - Показать\\убрать сетку\n Начать игру - F2.",
                "Информация по управлению");
            
        }


    }
}
