#include <stdio.h>
#include "dnanet.h"

#define BUFSIZE 512

void main()
{
	int sd = 0;
	char buf[BUFSIZE + 1];

	printf("Program Start \r\n");


	sd = dnaOpen("127.0.0.1", 9600, 0x08);


	dnaAccept(sd, "127.0.0.1", 0);
		//dnaRead(sd, buf, BUFSIZE, 0, 9600);}