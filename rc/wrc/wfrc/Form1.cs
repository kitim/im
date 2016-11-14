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
        int sd = 0;
        int csd = 0;
        byte[] buf = new byte[512];
        byte[] ip = new byte[16];
        int btn_open_sts = 0;
        public Form1()
        {
           InitializeComponent();
           dn = new dplnet();

        }
        private void button1_Click(object sender, EventArgs e)
        {
            if (btn_open_sts == 0)
            {
                btn_open_sts = 1;
                button1.Text = "DISCONNECTED";
                byte[] ip = Encoding.Default.GetBytes("192.168.0.7");
                int port = 2654;
                int type = 0;
                int typ = 0x00;
                sd = dn.DNAOpen( ip, port, typ);
                textBox2.Text += "192.168.0.7 Open \r\n";
                textBox2.Select(textBox2.Text.Length, 0);
                
                if (type == 0 && sd > 0) 
                {
                    csd = dn.DNAAccept(sd, ip, 0);                    
                    if (csd > 0)
                    {   
                        textBox2.Text += "CONNECT RC CAR \r\n";
                        timer1.Enabled = true;
                    }
                }
                textBox2.Select(textBox2.Text.Length, 0);
                textBox2.ScrollToCaret();
            }
            else
            {
                btn_open_sts = 0;
                button1.Text = "CONNECTED";
                textBox2.Text += "192.168.0.7 CLOSE\r\n";
                textBox2.Select(textBox2.Text.Length, 0);
                textBox2.ScrollToCaret();
                dn.DNAClose(sd);
            }            
            }
        private void button2_Click_1(object sender, EventArgs e)
        {

            if (tb_depart.Text == tb_arrive.Text)
                   textBox2.Text += " 이동 불가 ";
            else
                textBox2.Text += (tb_depart.Text + " 에서 " + tb_arrive.Text + "으로 이동 시작...");
                textBox2.Text += "\r\n";
                textBox2.Select(textBox2.Text.Length, 0);
                textBox2.ScrollToCaret();

                byte[] wbuf1 = Encoding.Default.GetBytes(tb_depart.Text);
                dn.DNAWrite(sd, wbuf1, tb_depart.Text.Length, null, 0);
                byte[] wbuf = Encoding.Default.GetBytes(",");
                dn.DNAWrite(sd, wbuf, ",".Length, null, 0);
                byte[] wbuf2 = Encoding.Default.GetBytes(tb_arrive.Text);
                dn.DNAWrite(sd, wbuf2, tb_arrive.Text.Length, null, 0);
                

                

        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            int err = 0;
            err = dn.DNARead(csd, buf, 512, ip, 0);
            if(err>0)
            {
                buf[err] = 0;
                textBox2.Text += Encoding.Default.GetString(buf);
                textBox2.Text += "\r\n";

                textBox2.Select(textBox2.Text.Length, 0);
                textBox2.ScrollToCaret();
            }
        }
    }
}