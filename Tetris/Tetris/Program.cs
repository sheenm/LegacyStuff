// О возможном баге см. в блокнот.
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Tetris
{
    static class Levels
    {
        public static int StartWaitTime; // сколько миллисекунд за тик   
        public static int MinimumWaitTIme = 120; //минимальное кол-во миллисекунд на действие
        public static int Difficult;    //уровень сложности
        public static int DecreaseIntensivity; // через сколлько падений фигурок будет убыстряться скорость
        public static int NumOfDifficulties
        {
            get { return 3; }
            private set { }
        }
        
        public static void SetUp(int Diff)
        {
            Difficult = Diff;
            StartWaitTime = 600-Levels.Difficult*80;
            DecreaseIntensivity = 3 * (NumOfDifficulties - Difficult + 1)+2;
        }
    }

    static class Program
    {
        public static Form1 Test;
        public static Timer time;
        public static int WaitTime;
        public static long Scores;
        public static int NextF, NextFRlyPos;
        static int NumberOfTicks;
        static Random rnd;
        
        public static Figures Figure;
        //Функции
        static Program()
        {
            NumberOfTicks = 6;
            time = new Timer();
            rnd = new Random();
            time.Tick += new EventHandler(TickHandler);
            time.Tick += new EventHandler(IntervalLower);
        }

        public static void GameStart()
        {
            Parameters.SetMass();
            Figure = null;
            WaitTime = Levels.StartWaitTime;
            Scores = 0;
            NextF = rnd.Next() % 5;
            time.Interval = WaitTime; //установка интервала  на каждый тик
            Test.Refresh();
            time.Start();  // Запуск
        }
        static void TickHandler(object sender, EventArgs e)
        {
            if (Figure == null)
            {
                Scores += Parameters.LineBreak();
            
                switch (NextF)
                {
                    case 0: Figure = new LineFigure(); break;
                    case 1: Figure = new TriangleFigure(); break;
                    case 2: Figure = new SquareFigure(); break;
                    case 3:
                        if (NextFRlyPos == 1)
                            Figure = new FourLeftFigure();
                        else Figure = new FourRightFigure();
                        break;
                    case 4: Figure = new HorseFigure( NextFRlyPos ); break;
                }
                if ((NextFRlyPos = rnd.Next() % 2) == 0) NextFRlyPos = -1;
                else NextFRlyPos = 1;
                NextF = rnd.Next() % 5 ;
                MiniPics.ShowJpg(NextF,NextFRlyPos);
            }
            else
            {
                if (!Figure.TickAction()) Figure = null;
            }
            Scores +=WaitTime/100;
            Test.ScoreSet(Scores);
        }
        static void IntervalLower(object sender, EventArgs e) {

            if (Figure == null)
            {
                NumberOfTicks++;
            }

            if (NumberOfTicks % Levels.DecreaseIntensivity == 0)
            {
                WaitTime -= 12;
                if (WaitTime <= Levels.MinimumWaitTIme) WaitTime = Levels.MinimumWaitTIme;
                time.Interval = WaitTime;
                NumberOfTicks++;
            }
            
        }
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(Test = new Form1());
            
        }
    }
}
