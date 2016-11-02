#include <SoftwareSerial.h>
#include "ESP8266.h"  //위에꺼랑 이거랑 같이쓰면오류안남
#define BAUD_RATE     115200
#define UART_BUF_SZ        128
#define UART_OK            0
#define UART_FAIL          -1
#define UART_PENDDING      -2

const char UART_COMMAND[][UART_BUF_SZ] =
{
  "AT+RST\r\n", //0
  "AT\r\n",              //1
  "AT+CWMODE=1\r\n",     //2
  "AT+CWQAP\r\n", //3
  "AT+CWJAP_CUR=\"DALS\",\"DAL@D510\"\r\n", //4

  "AT+CIPSTART=\"TCP\",\"192.168.0.15\",2654\r\n", //5   현재 내 컴퓨터 아이피
  "ATE0\r\n",   ////   6 Echo Off
  "ATE1\r\n",   ////   7 Echo On
};

SoftwareSerial srl(6, 7); // RX, TX

char buf[UART_BUF_SZ];
int uart_index = 0;
int uart_command_index = 0;
int run_command = 1;
char dbg[80];

void setup()
{
  // set the data rate for the SoftwareSerial port
  srl.begin(BAUD_RATE);

  while (!srl.available()) ;
  memset(buf, 0, sizeof(char)*UART_BUF_SZ);
}

void bufClear()
{
  uart_index = 0;
  memset(buf, 0, sizeof(char)*UART_BUF_SZ);
}

int checkAck()
{
  int e = UART_PENDDING;
  int idx = 0;
  char c = 0;
  for ( idx = 0; idx<UART_BUF_SZ-1 ; idx++ )
  {
    c = srl.read();
    if ( c >= 0 )
    {
      buf[uart_index] = c;
      if (  (uart_index>0) && (buf[uart_index-1]=='O') && (buf[uart_index]=='K') )
      {
        bufClear();
        e = UART_OK;
        break;
      }
      uart_index++;
    }
  }
  return e;
}

int readUart(int count)
{
  int e = -1;
  int idx = 0;
  for ( idx = 0 ; idx<count ; idx++ )
  {
    e = checkAck();
    if ( e == UART_OK  )
    {
      break;
    }
  }
  return e;  
}

int writeAT(char* cmd)
{
  return srl.write(cmd, strlen(cmd));
}

void loop()
{
  char c = 0;
  int e = UART_OK;
  uart_command_index = 0;

  delay(4000);

  writeAT(UART_COMMAND[7]);
  e = readUart(100);

  writeAT(UART_COMMAND[1]);
  e = readUart(100);

  writeAT(UART_COMMAND[3]);
  e = readUart(2000);

  writeAT(UART_COMMAND[2]);
  e = readUart(100);

  while ( 1 )
  {
    if ( e == UART_OK )
    {
      writeAT(UART_COMMAND[4]);
      e = UART_FAIL;
    }
    e = readUart(1000);
    if ( e == UART_OK )
    {
      break;
    }
  }

  while ( 1 )
  {
    if ( e == UART_OK )
    {
      writeAT(UART_COMMAND[5]);
      e = UART_FAIL;
    }
    e = readUart(1000);
    if ( e == UART_OK )
    {
      break;
    }
  }


  writeAT(UART_COMMAND[6]);
  e = readUart(100);


  bufClear();
  while(1)
  {
    c = srl.read();
    if ( c >=0 )
    {
      //srl.println(c);
      srl.write("[", 1);
      srl.write(&c, 1);
      srl.write("]", 1);
    }
  }
}

