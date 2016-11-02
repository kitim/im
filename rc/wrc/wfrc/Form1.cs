using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace wfrc
{
    public partial class Form1 : Form
    {
        dplnet dn;

        public Form1()
        {
            InitializeComponent();
            maskedTextBox1.Text = "___.___._._.";            
            maskedTextBox1.Text = "00000000";
            dn = new dplnet();
            button2.Click += " 안녕!!";

        }

        private void button1_Click(object sender, EventArgs e)
        {
            textBox2.Text = maskedTextBox1.Text + " 접속";
            byte [] ip = Encoding.Default.GetBytes("127.0.0.1");
            dn.DNAOpen(ip, 20000, 0x00);

        }



        private void button2_Click(object sender, EventArgs e)
        {
            
            //if (comboBox1.SelectedIndex == comboBox2.SelectedIndex)
             //textBox2.Text = "정지" ;
           //else
           //    textBox2.Text = (comboBox1.SelectedIndex + " 에서 " + comboBox2.SelectedIndex + "으로 이동 중");
          
        }




    }
}
