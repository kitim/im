#include "Arduino.h"

#include "wfm.h"
#include <stdio.h>


int wfm_check_ack(tagWFM* p, const char* ack, int sz)
{
  int e = UART_PENDDING;
  int idx = 0;
  char c = 0;
  int i = 0;
  int _sz = sz-1;
  
  for ( idx = 0; idx<UART_BUF_SZ-1 ; idx++ )
  {
    c = p->uread();
    //c = p->srl.read();
    if ( c >= 0 )
    {
      p->buf[p->idx] = c;
      if (  p->idx > 0  ) 
      {
        for ( i=0 ; i<sz ; i++ )
        {
          if ( (p->buf[p->idx-(i)] == *(ack+_sz) ) )
          {
            _sz--;
            if ( _sz <= 0 )
            {
              memset(p->buf, 0, UART_BUF_SZ);
              p->idx = 0;
              e = UART_OK;
              break;
            }
          }
        }
      }
      p->idx++;
    }
  }
  return e;
}

int wfm_uread(tagWFM* p, int count, const char* ack, int sz)
{
  int e = -1;
  int idx = 0;
  for ( idx = 0 ; idx<count ; idx++ )
  {
    e = wfm_check_ack(p, ack, sz);
    if ( e == UART_OK  )
    {
      break;
    }
  }
  return e;  
}

int wfm_uwrite(tagWFM* p, const char* cmd, const char* arg)
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
  return p->uwrite(_cmd, strlen(_cmd));
  //return p->srl.write(_cmd, strlen(_cmd));
}

int wfm_write(tagWFM* p, const char* buf, int sz, int msec)
{
  int e = 0;
  char _sb[UART_BUF_SZ];
  sprintf(_sb, "%d", sz+2);
  
  wfm_uwrite(p, "AT+CIPSENDEX", _sb);  //8...
  e = wfm_uread(p, 1000, "> ", 2);
  if ( e == UART_OK )
  {
    sprintf(_sb, "%s\\0", buf);
    p->uwrite(_sb, sz+2);
    //p->srl.write(_sb, sz+2);
    delay(msec);
  }
  return e;
}

int wfm_str_dlmtr(const char* str, const char* dlmtr, int idx, char* out)
{
  int e = 0;
  int i = 0;
  int _i = 0;
  int _idx = 0;
  for ( i=0 ; *(str+i) ; i++, _i++)
  {
    *(out+_i) = *(str+i);
    if ( *(str+i) == *(dlmtr+0)  )
    {
      _i = -1;
      _idx++;
    }
    if ( *(str+i+1) == 0 )
    {
      _i++;
      _idx++;
    }
    if ( _idx == (idx-1) )
    {
      e = _i;
      *(out+_i) = 0;
      break;
    }
  }
  return e;
}

int wfm_read(tagWFM* p, char* buf, int sz)
{
  int e = 0;
  int c = 0;
  int i = 0;
  
  wfm_uwrite(p, "ATE1", 0);
  e = wfm_uread(p, 100, "OK", 2);
  if ( e<UART_OK ) return e;
  
  for ( i=0 ; i<sz ; )
  {
    c = p->uread();
    if ( c>=0 )
    {
      if ( c=='+' )
      {
        p->cidx = 0;
        p->idx = 0;
      }
      p->cbuf[p->cidx][p->idx] = c;
      p->idx++;

      if ( c==':' )
      {
        p->cbuf[p->cidx][p->idx-1] = 0;
        p->cidx++;
        p->idx=0;
      }

      if ( p->cidx == CBUF_COUNT )
      {
        if ( wfm_str_dlmtr(p->cbuf[0], ",", 1, buf) > 0 )
        {
          e = atoi(buf);
          wfm_str_dlmtr(p->cbuf[1], ":", 1, buf);
        }
      }
    }
  }
  wfm_uwrite(p, "ATE0", 0);
  e = wfm_uread(p, 100, "OK", 2);
  if ( e<UART_OK ) return e;

  return e;
}

