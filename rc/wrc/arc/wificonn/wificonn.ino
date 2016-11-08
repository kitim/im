#include <SoftwareSerial.h>
//#include "ESP8266.h"  //위에꺼랑 이거랑 같이쓰면오류안남
#define BAUD_RATE     115200
#define UART_BUF_SZ        128
#define CBUF_COUNT         2
#define UART_OK            0
#define UART_FAIL          -1
#define UART_PENDDING      -2

SoftwareSerial srl(6, 7); // RX, TX

char buf[UART_BUF_SZ];
int uart_index = 0;
int uart_command_index = 0;
int run_command = 1;
char dbg[80];

char cbuf[CBUF_COUNT][UART_BUF_SZ];
int  cindex = 0;

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


int checkAck(char* ack, int sz)
{
  int e = UART_PENDDING;
  int idx = 0;
  char c = 0;
  int i = 0;
  int _sz = sz-1;
  for ( idx = 0; idx<UART_BUF_SZ-1 ; idx++ )
  {
    c = srl.read();
    if ( c >= 0 )
    {
      buf[uart_index] = c;
      if (  uart_index>0  ) 
      {
        for ( i=0 ; i<sz ; i++ )
        {
          if ( (buf[uart_index-(i)] == *(ack+_sz) ) )
          {
            _sz--;
            if ( _sz <= 0 )
            {
              bufClear();
              e = UART_OK;
              break;
            }
          }
        }
      }
      uart_index++;
    }
  }
  return e;
}

int readUart(int count, char* ack, int sz)
{
  int e = -1;
  int idx = 0;
  for ( idx = 0 ; idx<count ; idx++ )
  {
    e = checkAck(ack, sz);
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

int writeATex(char* cmd, char* arg)
{
  char _cmd[UART_BUF_SZ];
  if ( arg )
  {
    sprintf(_cmd, "%s=%s\r\n", cmd, arg);
  }
  else
  {
    sprintf(_cmd, "%s\r\n", cmd);
  }
  return srl.write(_cmd, strlen(_cmd));
}

int writeEx(char* buf, int sz, int msec)
{
  int e = 0;
  char _sb[UART_BUF_SZ];
  sprintf(_sb, "%d", sz+2);
  
  writeATex("AT+CIPSENDEX", _sb);  //8...
  e = readUart(1000, "> ", 2);
  if ( e == UART_OK )
  {
    sprintf(_sb, "%s\\0", buf);
    writeATex(_sb, sz+2);
    delay(msec);
  }
  return e;
}


void loop()
{
  char c = 0;
  int e = UART_OK;
  uart_command_index = 0;

  delay(4000);

  writeATex("ATE1", 0);    //// 0  == '\0'  : NULL
  e = readUart(100, "OK", 2);

  writeATex("AT", 0);
  e = readUart(100, "OK", 2);

  writeATex("AT+CWQAP", 0);
  e = readUart(2000, "OK", 2);

  writeATex("AT+CWMODE", "1");
  e = readUart(100, "OK", 2);

  while (1  )
  {
    if ( e == UART_OK )
    {
      writeATex("AT+CWJAP_CUR", "\"DALS\",\"DAL@D510\"");
      e = UART_FAIL;
    }
    e = readUart(1000, "OK", 2);
    if ( e == UART_OK )
    {
      break;
    }
  }

  writeATex("AT+CIPMUX", "0");
  e = readUart(100, "OK", 2);


  while ( 1 )
  {
    if ( e == UART_OK )
<<<<<<< HEAD
    {
      writeATex("AT+CIPSTART", "\"TCP\",\"192.168.0.16\",2654");
      e = UART_FAIL;
    }
    e = readUart(1000, "OK", 2);
    if ( e == UART_OK )
    {
      break;
    }
  }



  bufClear();
  e = UART_FAIL;

  while(1)
  {
    writeATex("ATE1", 0);
    e = readUart(1000, "OK", 2);

    c = srl.read();
    if ( c >= 0 )
    {
=======
    {
      writeATex("AT+CIPSTART", "\"TCP\",\"192.168.0.16\",2654");
      e = UART_FAIL;
    }
    e = readUart(1000, "OK", 2);
    if ( e == UART_OK )
    {
      break;
    }
  }



  bufClear();
  e = UART_FAIL;

  while(1)
  {
    writeATex("ATE1", 0);
    e = readUart(1000, "OK", 2);

    c = srl.read();
    if ( c >= 0 )
    {
>>>>>>> 1a87a53a889910ca58fa18ff794d05fc56e7ed60
      if ( c == '+' )
      {
        cindex = 0;
        uart_index = 0;
      }

      buf[uart_index] = c;
      uart_index++;
      if ( c == ':' )
      {
        cindex ++;
        uart_index = 0;
      }

      if ( cindex == CBUF_COUNT )
      {
        /////// 문자열 분리
<<<<<<< HEAD
        ///////
        ///////
=======
        /////// +IPD,4:1,3:    //+IPD,8:101,303;
        ///////
        
>>>>>>> 1a87a53a889910ca58fa18ff794d05fc56e7ed60
        writeATex("AT", 0);
        e = readUart(1000, "OK", 2);

        e = UART_OK;
      }
<<<<<<< HEAD
      
=======
       
>>>>>>> 1a87a53a889910ca58fa18ff794d05fc56e7ed60
    }

    if ( e == UART_OK )
    {
      writeATex("ATE0", 0);
      e = readUart(1000, "OK", 2);

      writeEx("OK\r\n", 4, 500);
      e = UART_FAIL;
    }
  }

  
}
<<<<<<< HEAD

=======
>>>>>>> 1a87a53a889910ca58fa18ff794d05fc56e7ed60
