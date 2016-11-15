#include <SoftwareSerial.h>
#include "wfm.h"

#define AP_INFO       "\"DALS\",\"DAL@D510\""
#define SVR_INFO      "\"TCP\",\"192.168.0.16\",2654"

#define GBUF_SZ        128
//
// 자동자 진행 방향 정의
//
#define CAR_DIR_FW  0   // 전진.
#define CAR_DIR_BK  1   // 후진.
#define CAR_DIR_LF  2   // 좌회전.
#define CAR_DIR_RF  3   // 우회전
#define CAR_DIR_ST  4   // 정지.

// 
// 차량 운행 방향 상태 전역 변수. // 정지 상태.
int g_carDirection = CAR_DIR_ST; // 

int g_carSpeed = 190; // 220 ~ 255 사이의 값을 넣어주도록 합니다.
//
// 주의:  ENA, ENB 는 PWM 지원 포트에 연결한다.
// 다음 업데이트시 변경합니다.
#define ENA   6
#define EN1   7
#define EN2   3

#define EN3   4
#define EN4   2
#define ENB   5


SoftwareSerial srl(8,9);
tagWFM  wfm;
char gbuf[GBUF_SZ];

unsigned int intr[2] = {0,0};

char uart_read()
{
  return srl.read();
}

int uart_write(char* p, int sz)
{
  return srl.write(p, sz);
}

int uart_status(char* p, int sz)
{
  Serial.print(p);
  return 0;
}

void init_car_controller_board()
{
  pinMode(ENA, OUTPUT);  // ENA
  pinMode(EN1, OUTPUT);  // EN1
  pinMode(EN2, OUTPUT);  // EN2

  pinMode(ENB, OUTPUT);  // ENB
  pinMode(EN3, OUTPUT);  // EN3
  pinMode(EN4, OUTPUT);  // EN4
}

//
// 전후좌우 4개의 함수는 테스트시
// DC 모터 연결에 맞게 고쳐서 정정해야 합니다.
// DC 모터 연결 (+)(-) 연결 변경하거나 코드를 변경합니다.
//
void car_backward()//후진
{
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, HIGH);
  analogWrite(ENA, g_carSpeed);

  digitalWrite(EN3, LOW);
  digitalWrite(EN4, HIGH);
  analogWrite(ENB, g_carSpeed);
}

void car_forward()//전진
{

  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, LOW);
  analogWrite(ENA, g_carSpeed);

  digitalWrite(EN3, HIGH);
  digitalWrite(EN4, LOW);
  analogWrite(ENB, g_carSpeed);
}
//
void car_right()//우회전
{
  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, LOW);
  analogWrite(ENA, g_carSpeed);

  digitalWrite(EN3, LOW);
  digitalWrite(EN4, HIGH);
  analogWrite(ENB, g_carSpeed);
}

//
void car_left()//좌회전
{
  digitalWrite(EN1, LOW);
  digitalWrite(EN2, HIGH);
  analogWrite(ENA, g_carSpeed);

  digitalWrite(EN3, HIGH);
  digitalWrite(EN4, LOW);
  analogWrite(ENB, g_carSpeed);
}

//
//
void car_stop()
{
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

//
// 방향 전환값에 의해 차량 운행.
//
void car_update(char mode)
{
  if (mode == '0')
  {
    g_carDirection == CAR_DIR_ST;
  }
  else 
  {
    if (g_carDirection == CAR_DIR_FW)  // 전진
    {
      car_forward();
    }
    else
    if (g_carDirection == CAR_DIR_BK) // 후진.
    {
      car_backward();
    }
    else
    if (g_carDirection == CAR_DIR_LF) // 좌회전
    {
      car_left();
    }
    else
    if (g_carDirection == CAR_DIR_RF) // 우회전
    {
      car_right();
    }
    else
    if (g_carDirection == CAR_DIR_ST) // 정지.
    {
      car_stop();
    }
    g_carDirection == CAR_DIR_FW;
  }

  // %%
  // 현재는 매번 loop 에서 호출되지만 차후에 커맨드가 있을 경우만 호출될 수 있도록
  // 변경해야 합니다.
  
}


////
// 라인트레이서 모듈 핀맵
#define LT_MODULE_L A0  //  14
#define LT_MODULE_F A1  //  15
#define LT_MODULE_R A2  //  16
#define LT_MODULE_S A3  //  17

void init_line_tracer_modules()
{
  pinMode(LT_MODULE_L, INPUT);
  pinMode(LT_MODULE_F, INPUT);
  pinMode(LT_MODULE_R, INPUT);
  pinMode(LT_MODULE_S, INPUT);
}

// is detected left
bool lt_isLeft()
{
  int ret = digitalRead(LT_MODULE_L);  // 왼쪽 센서
  return ret == 1 ? true : false;

}

bool lt_isForward()
{
  int ret = digitalRead(LT_MODULE_F); // 중간 센서
  return ret == 1 ? true : false;

}
bool lt_isRight()
{
  int ret = digitalRead(LT_MODULE_R); // 오른쪽 센서
  return ret == 1 ? true : false;
}
bool lt_isStop()
{
  int ret = digitalRead(LT_MODULE_S); // 점 센서
  return ret == 1 ? true : false;
}

int ll = 0;
int ff = 0;
int rr = 0;
int ss = 0;
int fwd_check[3] = {0, 0, 0};
int count = 0;
// 
void lt_mode_update(char mode)
{
  if ( mode == '0' )
  {
    car_stop();
    return;
  }
  ll = lt_isLeft();
  ff = lt_isForward();
  rr = lt_isRight();

  if (  fwd_check[0] == 1 && fwd_check[1] == 0 && fwd_check[2]== 1 )
  {
    ss = 0;
    count ++;
  }
  else
  {
    count = 0;
    ss = lt_isStop();
  }

  if ( count >=1024 )
  {
    fwd_check[0] == 1 && fwd_check[1] == 0 && fwd_check[2]== 0;
    count = 0;
  }
  
  if (ll&&ff&&rr)
  {
    g_carDirection = CAR_DIR_ST; // stop
  }
  else
  if (!ll&&!ff&&!rr)
  {
    g_carDirection = CAR_DIR_ST; // stop
  }
  else
  if (ll)
  {
    g_carDirection = CAR_DIR_LF;
  }
  else
  if (rr)
  {
    g_carDirection = CAR_DIR_RF;
  }
  else
  if (ff)
  {
    g_carDirection = CAR_DIR_FW;
    if (ss)
    {
      fwd_check[0] = 0;
      fwd_check[1] = 1;
      fwd_check[2] = 1;
      g_carDirection = CAR_DIR_ST;
    }
  }
 
}

// 부팅 후 1회 실행되는 함수. 초기화 함수. Setup()
void setup()
{
  //
  Serial.begin(9600);
  
  srl.begin(BAUD_RATE);
  Serial.print("program start .... \r\n");
  //
  init_car_controller_board();
  
  memset(&wfm, 0, sizeof(tagWFM));
  wfm.uwrite = uart_write;
  wfm.uread = uart_read;
  wfm.ustatus = uart_status;

  srl.listen();

}

// 계속 실행되는 함수. Loop()
void loop()
{
  int c = 0;
  int e = 0;
  int i = 0;
  int sz = 0;
  int r = 0;
  char buf[UART_BUF_SZ];
  char cbuf[2][32];
  char vbuf[2][32];
  int ce = 0;

  memset(buf, 0, UART_BUF_SZ);

  delay(2000);
  wfm_uwrite(&wfm, "AT+UART_DEF", "9600,8,1,0,0");
  e = wfm_uread_check(&wfm, buf, UART_BUF_SZ, "OK\r\n", 4, 1000);
   Serial.print("AT+UART_DEF ");
  if ( e > 0 ) Serial.print("OK\r\n");
  else Serial.print("FAIL\r\n");

  delay(2000);
  srl.begin(9600);
  delay(2000);
    
  memset(buf, 0, UART_BUF_SZ);
  wfm_uwrite(&wfm, "ATE1", 0);
  e = wfm_uread_check(&wfm, buf, UART_BUF_SZ, "OK\r\n", 4, 1000);
  Serial.print("ATE1 ");
  if ( e > 0 ) Serial.print("OK\r\n");
  else Serial.print("FAIL\r\n");

  memset(buf, 0, UART_BUF_SZ);
  wfm_uwrite(&wfm, "AT+CWAUTOCONN", "0");
  e = wfm_uread_check(&wfm, buf, UART_BUF_SZ, "OK\r\n", 4, 1000);
  Serial.print("AT+CWAUTOCONN ");
  if ( e > 0 ) Serial.print("OK\r\n");
  else Serial.print("FAIL\r\n");

  memset(buf, 0, UART_BUF_SZ);
  wfm_uwrite(&wfm, "AT+CWQAP", 0);
  e = wfm_uread_check(&wfm, buf, UART_BUF_SZ, "OK\r\n", 4, 1000);
  Serial.print("AT+CWQAP ");
  if ( e > 0 ) Serial.print("OK\r\n");
  else Serial.print("FAIL\r\n");

  memset(buf, 0, UART_BUF_SZ);
  wfm_uwrite(&wfm, "AT+CWMODE", "1");
  e = wfm_uread_check(&wfm, buf, UART_BUF_SZ, "OK\r\n", 4, 1000);
  Serial.print("AT+CWMODE ");
  if ( e > 0 ) Serial.print("OK\r\n");
  else Serial.print("FAIL\r\n");

  e = -1;
  while ( 1 )
  {
    if ( e < 0 )
    {
      memset(buf, 0, UART_BUF_SZ);
      wfm_uwrite(&wfm, "AT+CWJAP_CUR", AP_INFO);
    }
    e = wfm_uread_check(&wfm, buf, UART_BUF_SZ, "OK\r\n", 4, 1000);
    if ( e > 0 )
    {
      break;
    }
  }
  Serial.print("AT+CWJAP_CUR ");
  if ( e > 0 ) Serial.print("OK\r\n");
  else Serial.print("FAIL\r\n");

  e = -1;
  while ( 1 )
  {
    if ( e < 0 )
    {
      memset(buf, 0, UART_BUF_SZ);
      wfm_uwrite(&wfm, "AT+CIPMUX", "0");
    }
    e = wfm_uread_check(&wfm, buf, UART_BUF_SZ, "OK\r\n", 4, 1000);
    if ( e > 0 )
    {
      break;
    }
  }
  Serial.print("AT+CIPMUX ");
  if ( e > 0 ) Serial.print("OK\r\n");
  else Serial.print("FAIL\r\n");

  e = -1;
  while ( 1 )
  {
    if ( e < 0 )
    {
      memset(buf, 0, UART_BUF_SZ);
      wfm_uwrite(&wfm, "AT+CIPSTART", SVR_INFO);
    }
    e = wfm_uread_check(&wfm, buf, UART_BUF_SZ, "OK\r\n", 4, 90000);
    if ( e > 0 )
    {
      break;
    }
  }
  Serial.print("AT+CIPSTART ");
  if ( e > 0 ) Serial.print("OK\r\n");
  else Serial.print("FAIL\r\n");

  i = 0;
  memset(vbuf, 0, sizeof(char)*2*32);
  
  while ( 1 )
  {
    #if 1
    e = wfm_uread(&wfm, &buf[i], UART_BUF_SZ);
    if ( e > 0 )
    {
      i+=e;

      Serial.println("..........");
      Serial.println(buf);
      Serial.println("++++++++++");
      sz = wfm_check(&wfm, buf, i, "+IPD,", 5);
      if ( sz > 0 && i>=12 )
      {
        //sz = wfm_check(&wfm, buf, i, "\r\n", 2);
        //if ( sz > 0 )
        {
          i = 0;
          ce = wfm_str_dlmtr(&wfm, buf, ":", 0, cbuf[0]);
          cbuf[0][ce] = 0;
          ce = wfm_str_dlmtr(&wfm, buf, ":", 1, cbuf[1]);
          cbuf[1][ce] = 0;

          #if 0
          Serial.println("----------");
          ce = wfm_str_dlmtr(&wfm, cbuf[0], ",", 1, vbuf[0]);
          vbuf[0][ce] = 0;
          {
            char dbg[32];
            sprintf(dbg, "read : %d --> ", atoi(vbuf));
            Serial.print(dbg);
          }
          #endif
          ce = wfm_str_dlmtr(&wfm, cbuf[1], ",", 0, vbuf[0]);
          vbuf[0][ce] = 0;
          ce = wfm_str_dlmtr(&wfm, cbuf[1], ",", 1, vbuf[1]);
          vbuf[1][ce] = 0;
          {
            char dbg[32];
            sprintf(dbg, "%c -> %c \r\n", vbuf[0][0], vbuf[1][0]);
            Serial.print(dbg);
          }
          Serial.println("==========");
        }
      }
    }
    #endif
    {
      char dbg[32];
      sprintf(dbg, "%c -> %c \r\n", vbuf[0][0], vbuf[1][0]);
      Serial.print(dbg);
    }

    if (vbuf[0][0] == '0')
    {
      fwd_check[0] = fwd_check[1] = 0;
      car_update(vbuf[0][0]);
      lt_mode_update(vbuf[0][0]);
    }
    if (vbuf[0][0] == '1')
    {
      if ( fwd_check[0] == 0 && fwd_check[1] == 0 && fwd_check[2] == 0 )
      {
        fwd_check[0] = 1;
        fwd_check[1] = 0;
        fwd_check[2] = 0;
      }
      else
      if (  fwd_check[0] == 0 && fwd_check[1] == 1 && fwd_check[2]== 1 )
      {
        fwd_check[0] = 0;
        fwd_check[1] = 0;
        fwd_check[2] = 1;
      }
      else
      if (  fwd_check[0] == 0 && fwd_check[1] == 0 && fwd_check[2]== 1 )
      {
        fwd_check[0] = 1;
        fwd_check[1] = 0;
        fwd_check[2] = 1;
      }
      lt_mode_update(vbuf[0][0]);
      ///////////////////////////////////

      
      ///////////////////////////////////
      car_update(vbuf[0][0]);  
    }


    #if 0
    if (vbuf[0][0] == '2')
    {
      stst = '2';
      int cnt = 0;
      car_update(stst);
      lt_mode_update(stst);
      if ( ss == 1)
      {
        cnt += 1;
        if (cnt == 2)
        {
          vbuf[0][0] == '0';
        }
      }
    }
    #endif 
  }
}
