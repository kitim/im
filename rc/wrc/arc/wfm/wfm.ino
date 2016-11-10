#include <SoftwareSerial.h>
#include "wfm.h"

#define RETRY_COUNT    1000

SoftwareSerial srl(6,7);
tagWFM  wfm;

int uart_read()
{
  return srl.read();
}

int uart_write(char* p, int sz)
{
  return srl.write(p, sz);
}

void setup() {
  Serial.begin(9600);
  srl.begin(BAUD_RATE);
  memset(&wfm, 0, sizeof(tagWFM));
  wfm.uwrite = uart_write;
  wfm.uread = uart_read;
}


void loop() {
  int c = 0;
  int e = 0;
  int i = 0;
  char buf[32];
  Serial.print("program Start \r\n");
  // put your main code here, to run repeatedly:
  delay(2000);

#if 1

  wfm_uwrite(&wfm, "ATE1", 0);
  e = wfm_uread(&wfm, 100, "OK", 2);
  Serial.print("ATE1 -> ");
  if ( e == UART_OK ) Serial.print("SUCCESS\r\n");
  else Serial.print("FAIL\r\n");

  for ( i=0 ; i<RETRY_COUNT ; i++ )
  {
    wfm_uwrite(&wfm, "AT", 0);
    e = wfm_uread(&wfm, 1000, "OK", 2);
    Serial.print("AT -> ");
    if ( e == UART_OK )
    {
      Serial.print("SUCCESS\r\n");
      break;
    }
    else Serial.print("FAIL\r\n");
  }

  
  for ( i=0 ; i<RETRY_COUNT ; i++ )
  {
    wfm_uwrite(&wfm, "AT+CWQAP", 0);
    e = wfm_uread(&wfm, 500, "OK", 2);
    Serial.print("AT+CWQAP -> ");
    if ( e == UART_OK )
    {
      Serial.print("SUCCESS\r\n");
      break;
    }
    else Serial.print("FAIL\r\n");
  }

  for ( i=0 ; i<RETRY_COUNT ; i++ )
  {
    wfm_uwrite(&wfm, "AT+CWMODE", "1");
    e = wfm_uread(&wfm, 500, "OK", 2);
    Serial.print("AT+CWMODE -> ");
    if ( e == UART_OK )
    {
      Serial.print("SUCCESS\r\n");
      break;
    }
    else Serial.print("FAIL\r\n");
  }


  while (1  )
  {
    if ( e == UART_OK )
    {
      wfm_uwrite(&wfm, "AT+CWJAP_CUR", "\"DALS\",\"DAL@D510\"");
      e = UART_FAIL;
    }
    e = wfm_uread(&wfm, 1000, "OK", 2);
    if ( e == UART_OK )
    {
      break;
    }
  }
  Serial.print("AT+CWJAP_CUR -> ");
  if ( e == UART_OK ) Serial.print("SUCCESS\r\n");
  else Serial.print("FAIL\r\n");
  
  for ( i=0 ; i<RETRY_COUNT ; i++ )
  {
    wfm_uwrite(&wfm, "AT+CIPMUX", "0");
    e = wfm_uread(&wfm, 100, "OK", 2);
    Serial.print("AT+CIPMUX -> ");
    if ( e == UART_OK )
    {
      Serial.print("SUCCESS\r\n");
      break;
    }
    else Serial.print("FAIL\r\n");
  }
  
  while ( 1 )
  {
    if ( e == UART_OK )
    {
      wfm_uwrite(&wfm, "AT+CIPSTART", "\"TCP\",\"192.168.0.39\",2654");
      e = UART_FAIL;
    }
    e = wfm_uread(&wfm, 1000, "OK", 2);
    if ( e == UART_OK )
    {
      break;
    }
  }
  Serial.print("AT+CIPSTART -> ");
  if ( e == UART_OK )
  {
    Serial.print("SUCCESS\r\n");
  }
  else Serial.print("FAIL\r\n");

  while ( 1 )
  {
    e = wfm_read(&wfm, buf, 32);
    if ( e > 0 )
    {
      Serial.println(buf);
    }
  }
#endif

}
