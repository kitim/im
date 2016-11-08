#ifndef __WFM_H_5002903e_7609_44f2_a211_ee0fa7ef5669__
#define __WFM_H_5002903e_7609_44f2_a211_ee0fa7ef5669__

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BAUD_RATE          115200
#define UART_BUF_SZ        128
#define CBUF_COUNT         2
#define UART_OK            0
#define UART_FAIL          -1
#define UART_PENDDING      -2

typedef struct
{
  ////SoftwareSerial srl(6,7);
  
  char buf[UART_BUF_SZ];
  char cbuf[CBUF_COUNT][UART_BUF_SZ];
  int  idx;
  int  cidx;

  int (*uwrite)(char*, int);
  int (*uread)();
  
}tagWFM;

int wfm_check_ack(tagWFM* p, const char* ack, int sz);
int wfm_uread(tagWFM* p, int count, const char* ack, int sz);
int wfm_uwrite(tagWFM* p, const char* cmd, const char* arg);
int wfm_write(tagWFM* p, const char* buf, int sz, int msec);

#ifdef __cplusplus
}
#endif

#endif
