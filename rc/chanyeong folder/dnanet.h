#ifndef __DNANET_H_F58A630C_11AC_4A56_AF38_99F8BAE0D613__
#define __DNANET_H_F58A630C_11AC_4A56_AF38_99F8BAE0D613__

int dnaOpen(char*, int, int);
int dnaClose(int);
int dnaAccept(int, char*, int);
int dnaWrite(int, char*, int, char*, int);
int dnaRead(int, char*, int, char*, int);

#endif