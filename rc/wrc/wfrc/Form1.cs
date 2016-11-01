using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
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

      dn = new dplnet();
    }

    private void button1_Click(object sender, EventArgs e)
    {
      byte [] ip = Encoding.Default.GetBytes("127.0.0.1");
      dn.DNAOpen(ip, 20000, 0x00);
    }
  }
}
