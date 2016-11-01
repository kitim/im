

#include <windows.h>
#include <stdio.h>

void main()
{
  void* hdl;
  HANDLE h;
  int (*dnaOpen)(char*, int, int);
  int (*XSocketCreate)(void**);

  int e = 0;

  h = LoadLibrary("dplnet.dll");
  hdl = LoadLibrary("xmtif.dll");

 *(FARPROC*)&dnaOpen = GetProcAddress(h,"dnaOpen");
 *(FARPROC*)&XSocketCreate = GetProcAddress(hdl,"XSocketCreate");


  e = dnaOpen("127.0.0.1", 30000, 0);


  XSocketCreate(&hdl);


  printf("e : %d\r\n", e);

}