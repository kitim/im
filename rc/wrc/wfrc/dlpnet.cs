using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace wfrc
{
  unsafe public partial class dlpnet
  {
    [DllImport("dplnet.dll")]
    public static extern int dnaOpen(char* ip, int port, int type);

    [DllImport("dplnet.dll")]
    public static extern int dnaClose(int sd);

    [DllImport("dplnet.dll")]
    public static extern int dnaAccept(int sd, char* ip, int sz);

    [DllImport("dplnet.dll")]
    public static extern int dnaRead(int sd, char* buf, int sz, char* ip, int port);

    [DllImport("dplnet.dll")]
    public static extern int dnaWrite(int sd, char* buf, int sz, char* ip, int port);


    public int DNAOpen(byte[] ip, int port, int type)
    {
      int e = 0;
      fixed (byte* p = ip)
      {
        e = dnaOpen((char*)p, port, type);
      }
      return e;
    }


  }
}
