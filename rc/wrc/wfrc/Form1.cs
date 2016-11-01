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

            textBox1.Text = "---.---.-.-";
            dn = new dplnet();


            dn = new dplnet();

        }

        private void button1_Click(object sender, EventArgs e)
        {
            textBox2.Text = textBox1.Text + " 접속";
            byte [] ip = Encoding.Default.GetBytes("127.0.0.1");
            dn.DNAOpen(ip, 20000, 0x00);

        }


        private void maskedTextBox1_MaskInputRejected(object sender, MaskInputRejectedEventArgs e)
        {

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            
        }

        private void button2_Click(object sender, EventArgs e)
        {

            textBox2.Text = comboBox1.Text + " 에서 " + comboBox2.Text + " 으로 이동 중";
        }

    }
}
