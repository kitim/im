#include <windows.h>
#include <stdio.h>

typedef struct
{
  HANDLE h;
  
  int (*dnaOpen)(char*, int, int);
	int (*dnaClose)(int);
	int (*dnaAccept)(int, char*, int);
	int (*dnaWrite)(int, char*, int, char*, int);
	int (*dnaRead)(int, char*, int, char*, int);
} tagDNANET;


void main()
{
  int e = 0;
  int csd = 0;
  char buf[1024];
  int sz = 0;
  int idx = 0;

  tagDNANET dn;

  memset(&dn, 0, sizeof(tagDNANET));

  dn.h = LoadLibrary("dplnet.dll");

 *(FARPROC*)&dn.dnaOpen = GetProcAddress(dn.h,"dnaOpen");
 *(FARPROC*)&dn.dnaClose = GetProcAddress(dn.h, "dnaClose");
 *(FARPROC*)&dn.dnaAccept = GetProcAddress(dn.h, "dnaAccept");
 *(FARPROC*)&dn.dnaWrite = GetProcAddress(dn.h, "dnaWrite");
 *(FARPROC*)&dn.dnaRead = GetProcAddress(dn.h, "dnaRead");

  e = dn.dnaOpen("127.0.0.1", 2654, 0x00);


  if (e > 0)
  {
    printf("Open SUCCESS\r\n");
  }
  while (1)
  {
    csd = dn.dnaAccept(e, buf, 0);
    if (csd > 0 ) break;
  }

  while (1)
  {
	e = dn.dnaWrite(csd, "1,3:", 4, 0, 0);
	e  = dn.dnaRead(csd, buf, 1024, 0, 0);
	if (e > 0)
	{
		buf[e] = 0;
		printf("%08d : %s\r\n", idx, buf);
		idx++;
	}
	Sleep(1000);
  }
}