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
using System.Net;
using System.Net.Sockets;



namespace wfrc
{
    public partial class Form1 : Form
    {
        dplnet dn;

        public Form1()
        {
            InitializeComponent();
            maskedTextBox1.Text = "___.___._._.";
            //maskedTextBox1.Text = "00000000";
            dn = new dplnet();



        }

        private void button1_Click(object sender, EventArgs e)
        {

            textBox2.Text += maskedTextBox1.Text + " 접속\n";
            //MessageBox.Show(this.textBox2.Text);\
            textBox2.Text += "\r\n";

            byte[] ip = Encoding.Default.GetBytes("127.0.0.1");
            dn.DNAOpen(ip, 20000, 0x00);
            dn.DNAWrite(,,,ip,20000); 
            dn.DNAAccept( );

        }



        private void button2_Click_1(object sender, EventArgs e)
        {

            if (comboBox1.SelectedIndex == comboBox2.SelectedIndex)
                textBox2.Text += "정지";
            else
                textBox2.Text += (comboBox1.SelectedItem + " 에서 " + comboBox2.SelectedItem + "으로 이동 중...");

            textBox2.Text += "\r\n";
            //MessageBox.Show(this.textBox2.Text);


        }


    }
}