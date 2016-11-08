#include <SoftwareSerial.h>
#include "wfm.h"

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
  // put your setup code here, to run once:

  memset(&wfm, 0, sizeof(tagWFM));
  wfm.uwrite = uart_write;
  wfm.uread = uart_read;
}


void loop() {
  int c = 0;
  int e = 0;
  // put your main code here, to run repeatedly:
  delay(4000);

#if 1

  wfm_uwrite(&wfm, "ATE1", 0);
  e = wfm_uread(&wfm, 100, "OK", 2);


  wfm_uwrite(&wfm, "AT", 0);
  e = wfm_uread(&wfm, 100, "OK", 2);

  wfm_uwrite(&wfm, "AT+CWQAP", 0);
  e = wfm_uread(&wfm, 100, "OK", 2);

  wfm_uwrite(&wfm, "AT+CWMODE", "1");
  e = wfm_uread(&wfm, 100, "OK", 2);


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
  wfm_uwrite(&wfm, "AT+CIPMUX", "0");
  e = wfm_uread(&wfm, 100, "OK", 2);

  
  while ( 1 )
  {
    if ( e == UART_OK )
    {
      wfm_uwrite(&wfm, "AT+CIPSTART", "\"TCP\",\"192.168.0.16\",2654");
      e = UART_FAIL;
    }
    e = wfm_uread(&wfm, 1000, "OK", 2);
    if ( e == UART_OK )
    {
      break;
    }
  }
#endif

}
