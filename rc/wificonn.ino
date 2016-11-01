#include <SoftwareSerial.h>
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

SoftwareSerial srl(6, 7); // RX, TX

char buf[UART_BUF_CNT][UART_BUF_SZ];
int uart_index = 0;
int uart_command_index = 0;

void setup()
{
  // set the data rate for the SoftwareSerial port
  srl.begin(BAUD_RATE);

  //while (!srl.available()) ;
  memset(buf, 0, sizeof(char)*UART_BUF_CNT*UART_BUF_SZ);
  Serial.begin(9600);
}

void bufClear(int idx)
{
  uart_index = 0;
  memset(buf[idx], 0, sizeof(char)*UART_BUF_SZ);
}

int checkAck()
{
  int e = -1;
  int idx = 0;
  for ( idx = 0; idx<UART_BUF_SZ-1 ; idx++, uart_index++ )
  {
    buf[UART_READ][uart_index] = srl.read();

    if ( (uart_index>0) && (buf[UART_READ][uart_index-1]=='O') && (buf[UART_READ][uart_index]=='K') )
    {
      Serial.write("checkAck SUCCESS \r\n");
      bufClear(UART_READ);
      e = idx;
      break;
    }
  }

  if ( e < 0 )
  {
    bufClear(UART_READ);
    Serial.write("checkAck FAIL \r\n");
  }
  return e;
}

int readUart(int count)
{
  int e = -1;
  int idx = 0;
  for ( idx = 0 ; idx<count ; idx++ )
  {
    if ( checkAck() >= 0 )
    {
      Serial.write("readUart SUCCESS \r\n");
      e = idx;
      break;
    }
  }

  if ( e < 0 )
  {
    Serial.write("readUart FAIL \r\n");
  }
  return e;  
}

int writeAT(int idx, int count)
{
  int e = -1;
  srl.write(UART_COMMAND[idx]);
  if ( readUart(count) >= 0 )
  {
    e = idx;
  }
  return e;
}

void loop()
{
  int e = 0;
  uart_command_index = 0;
  while ( 1 )
  {
    e = writeAT(uart_command_index, 100);
    if ( e>=0 )
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
    writeAT(0, 100);
    delay(2000);
  }

  
}

