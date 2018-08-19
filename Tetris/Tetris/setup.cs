using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Tetris
{
    public partial class setup : Form
    {
        public setup()
        {
            InitializeComponent();
        }

        private void setup_Load(object sender, EventArgs e)
        {
            textBox1.Text = Parameters.vpravo.ToString();
            checkBox1.Checked = Parameters.GridView;
            switch (Levels.Difficult)
            {
                case 1:
                    ChangeLevelBox.Text = "Легко";
                    break;
                case 2:
                    ChangeLevelBox.Text = "Средне";
                    break;
                case 3:
                    ChangeLevelBox.Text = "Сложно";
                    break;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void label3_Click(object sender, EventArgs e)
        {
        }
        private void button1_Click(object sender, EventArgs e)
        {

            try
            {
                // Считывание количества клеток по горизонтали
                if (int.Parse(textBox1.Text) > 5) Parameters.vpravo = int.Parse(textBox1.Text);
                else throw new Exception();
                // Расчёт количества клеток по вертикали из соображений, что если можно вместить
                // не целое кол-во клеток на экране, то не целую часть опускаем. Нижняя граница идет не до 440 пикселя.
                int temp = 300 / Parameters.vpravo;
                Parameters.vniz = 400 / temp;
                //Показывать сетку или нет
                if (checkBox1.Checked) Parameters.GridView = true;
                else Parameters.GridView = false;
                switch (ChangeLevelBox.Text.ToString())
                {
                    case "Легко": Levels.SetUp(1); break;
                    case "Средне": Levels.SetUp(2); break;
                    case "Сложно": Levels.SetUp(3); break;
                    default: throw new ArgumentNullException();
                }
                Program.GameStart();
            }
            catch (FormatException)
            {
                MessageBox.Show("Неправильно введены значения, попытайтесь снова");
            }
            catch (ArgumentNullException)
            {
                MessageBox.Show("Неправильно введены значения, попытайтесь снова");
            }
            catch (Exception)
            {
                MessageBox.Show("Введите значение, большее чем 5");
            }
            finally
            {
                this.Close();
                
            }
        }
    }
}
