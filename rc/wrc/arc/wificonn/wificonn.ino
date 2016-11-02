#include <SoftwareSerial.h>
#include "ESP8266.h"  //위에꺼랑 이거랑 같이쓰면오류안남
#define BAUD_RATE     115200
#define UART_WRITE         0
#define UART_READ          1
#define UART_BUF_CNT       2
#define UART_BUF_SZ        64

const char UART_COMMAND[][UART_BUF_SZ] =
{
  "AT\r\n",              //0
  "AT+CWMODE=1\r\n",     //1
  "AT+CWJAP_CUR=\"DALS\",\"DAL@D510\"", ///2
};
int UART_COMMAND_DELAY[] =
{
  100,
  1000,
  20000,
};


SoftwareSerial srl(6, 7); // RX, TX

char buf[UART_BUF_CNT][UART_BUF_SZ];
int uart_index = 0;
int uart_command_index = 0;

void setup()
{
  // set the data rate for the SoftwareSerial port
  srl.begin(BAUD_RATE);

  while (!srl.available()) ;
  memset(buf, 0, sizeof(char)*UART_BUF_CNT*UART_BUF_SZ);
  Serial.begin(9600);
}

void bufClear(int idx)
{
  uart_index = 0;
  memset(buf[idx], 0, sizeof(char)*UART_BUF_SZ);
}

int checkAck(int* cmd)
{
  int e = -1;
  int idx = 0;
  for ( idx = 0; idx<UART_BUF_SZ-1 ; idx++, uart_index++ )
  {
    buf[UART_READ][uart_index] = srl.read();

    if (  (uart_index>0) && (buf[UART_READ][uart_index-1]=='O') && (buf[UART_READ][uart_index]=='K') )
    {
      Serial.write("checkAck SUCCESS \r\n");
      bufClear(UART_READ);
      e = idx;
      *cmd = 1;
      break;
    }
    if ( (uart_index>0) && (buf[UART_READ][uart_index-1]=='\r') && (buf[UART_READ][uart_index]=='\n')  )
    {
      Serial.write("checkAck SUCCESS 2\r\n");
      bufClear(UART_READ);
      *cmd = 1;
      break;
    }
    
  }

  if ( e < 0 )
  {
    bufClear(UART_READ);
    //Serial.write("checkAck FAIL \r\n");
  }
  return e;
}

int readUart(int count, int* cmd)
{
  int e = -1;
  int idx = 0;
  for ( idx = 0 ; idx<count ; idx++ )
  {
    e = checkAck(cmd);
    if ( e > 0 )
    {
      break;
    }
  }
  return e;  
}

int writeAT(int idx, int count, int* cmd)
{
  int e = -1;
  if ( *cmd > 0 ) srl.write(UART_COMMAND[idx], strlen(UART_COMMAND[idx]));
  if ( readUart(count, cmd) > 0 )
  {
    e = idx;
  }
  return e;
}

void loop()
{
  int e = -1;
  int c = 1;  //// 1:Command Enable,   0:Command Disable
  uart_command_index = 0;
  
  while ( 1 )
  {
    e = writeAT(uart_command_index, UART_COMMAND_DELAY[uart_command_index], &c);
    if ( e>0 )
    {
      if ( uart_command_index == 0 )
      {
        uart_command_index = 1;
      }
      else if ( uart_command_index == 1 )
      {
        uart_command_index = 2;
      }
      else if ( uart_command_index == 2 )
      {
        uart_command_index = 0;
        break;
      }
    }
    
  }

  while (1 )
  {
    //writeAT(0, 1000, &c);
    delay(2000);
  }

  
}

