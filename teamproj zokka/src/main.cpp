#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(2,3,4,5,6,7);   // lcd 선언

int btn1 = 11;
int btn2 = 10;
int btn3 = 9;     // 알람 끄는 버튼

boolean btn1State = false;    // 한 번만 눌리기 위해 설정
boolean btn2State = false;
boolean btn3State = false;

int setAlm = 12;    // 알람 시간 설정 슬라이드 버튼
int setTime = 13;   // 현재 시간 설정 슬라이드 버튼
int cntSld = 15;    // 타이머 슬라이드 버튼
int buzzer = 8;     // 피에조 + led

boolean ringingBuzz = false;    // 알람 울릴지 정하는 변수

int nTotalSec = 0;    // 현재 시간
int nhours = 0;
int nmins = 0;
int nsecs = 0;
int plusTime = 0;

int aTotalSec = 0;   // 알람 시간
int ahours = 0;
int amins = 0;
int asecs = 0;
boolean almSetted = false;    // 알람 설정 되었는지 여부 확인 변수

int tTotalSec = 0;   // 타이머
int thours = 0;
int tmins = 0;
int tsecs = 0;
boolean timerStart = false;   // 타이머가 작동중인지 확인하는 변수
unsigned long tStartTime = 0;   // 타이머가 시작된 지점





String getStringTime(int time)    // 한자리 수 일 때 앞에 0 붙이기
{
  String result = "";

  if(time<10)
    result = "0" + String(time);
  else
    result = String(time);

  return result;
}


void displayLCD(int mode, int hour, int min, int sec)   // lcd에 시간 표시
{
  String viewHour;
  String viewMin;
  String viewSec;

  lcd.setCursor(4,0);

  viewHour = getStringTime(hour);
  viewMin = getStringTime(min);
  viewSec = getStringTime(sec); 

  lcd.print(viewHour+":"+viewMin+":"+viewSec);


  if(mode == 0)   // 어떤 모드인지 알려줌
  {
    lcd.setCursor(6,1);
    lcd.print("Timer");
  }
  else if(mode == 1)
  {
    lcd.setCursor(6,1);
    lcd.print("Alarm");
  }
  else if(mode == 2)
  {
    lcd.setCursor(4,1);
    lcd.print("Set Time");
  }
  else
  {
    lcd.setCursor(4,1);
    lcd.print("        ");
  }
}


void setup() 
{
  lcd.begin(16,2);
  Serial.begin(9600);
  
  for(int i=9;i<=13;i++)	// 버튼 설정
    pinMode(i,INPUT);
  pinMode(8,OUTPUT);	// 피에조 led 설정
  pinMode(15,INPUT);  // 타이머 슬라이드 버튼

  lcd.setCursor(4,0);
  lcd.print("00:00:00");
}

void loop() 
{

  ////////// 타이머 ///////////

  if (digitalRead(cntSld) == HIGH)        // 좌측의 슬라이드 버튼을 올렸을 때
  {
    displayLCD(0, thours, tmins, tsecs);
    if(digitalRead(btn1) == HIGH && !btn1State)   // 하단의 첫 번째 버튼을 누르면 1분씩 증가
    {
      tTotalSec = tTotalSec + 60;
      btn1State = true;
      thours = tTotalSec/3600;
      tmins = (tTotalSec%3600) / 60;
      tsecs = tTotalSec%60;
    }

    if(digitalRead(btn1) == LOW)
      btn1State = false;
      

    
    if(digitalRead(btn2) == HIGH && !btn2State)   // 하단의 두 번째 버튼을 누르면 현재 회로에 흐른 시간이 기록됨
    {
      btn2State = true;
      timerStart = true;
      tStartTime = millis();
    }

    if(digitalRead(btn2) == LOW)
      btn2State = false;
    
    if(timerStart)
    {
      if(tTotalSec>0)   // 타이머가 가동중일 때
      {
        if(millis() - tStartTime  > 1000)   // tTotalSec가 1초에 1씩 줄어듦
        {
          tTotalSec--;
          tStartTime = millis();
        }
        thours = tTotalSec/3600;            // 시간 환산
        tmins = (tTotalSec%3600) / 60;
        tsecs = tTotalSec%60;
      }
      else  // 타이머가 00:00:00 이 되었을 때
      {
        ringingBuzz = true;     // 알람 울리기
        timerStart = false;     // 타이머 작동 중지
      }
    }
  }




  ///////// 알람 시간 설정 //////////

  if (digitalRead(setAlm) == LOW)     // 우측 상단의 슬라이드 버튼을 올렸을 때
  {
    displayLCD(1, ahours, amins, asecs);

    if(digitalRead(btn1) == HIGH && !btn1State)     // 하단의 첫 번째 버튼을 누르면 1분 증가
    {
      aTotalSec = aTotalSec + 60;
      ahours = aTotalSec/3600;
      amins = (aTotalSec%3600) / 60;
      asecs = aTotalSec%60;
      btn1State = true;
      almSetted = true;
    }

    if(digitalRead(btn1) == LOW)
      btn1State = false;

    if(digitalRead(btn2) == HIGH && !btn2State)     // 하단의 두 번째 버튼을 누르면 1시간 증가
    {
      aTotalSec = aTotalSec + 3600;
      ahours = aTotalSec/3600;
      amins = (aTotalSec%3600) / 60;
      asecs = aTotalSec%60;
      btn2State = true;
      almSetted = true;
    }

    if(digitalRead(btn2) == LOW)
      btn2State = false;
  }




  ///////// 현재 시간 설정 //////////

  if (digitalRead(setTime) == LOW)      // 우측 하단의 슬라이드 버튼을 올렸을 때
  {
    displayLCD(2, nhours, nmins, nsecs);

    if(digitalRead(btn1) == HIGH && !btn1State)     // 하단의 첫 번째 버튼을 누르면 1분 증가
    {
      plusTime = plusTime + 60;
      btn1State = true;
      nhours = nTotalSec/3600;
      nmins = (nTotalSec%3600) / 60;
      nsecs = nTotalSec%60;
    }

    if(digitalRead(btn1) == LOW)
      btn1State = false;

    if(digitalRead(btn2) == HIGH && !btn2State)     // 하단의 두 번째 버튼을 누르면 1시간 증가
    {
      plusTime = plusTime + 3600;
      btn2State = true;
      nhours = nTotalSec/3600;
      nmins = (nTotalSec%3600) / 60;
      nsecs = nTotalSec%60;
    }

    if(digitalRead(btn2) == LOW)
      btn2State = false;
  }



  ///////// 현재 시간 //////////

  if(digitalRead(setTime) == HIGH && digitalRead(setAlm) == HIGH && digitalRead(cntSld) == LOW)   // 슬라이드 버튼이 모두 내려가 있을 때
  {
    displayLCD(3, nhours, nmins, nsecs);    // 현재 시간 표시
  }

  nTotalSec = (millis()/1000 + plusTime);
  nhours = nTotalSec/3600;
  nmins = (nTotalSec%3600) / 60;
  nsecs = nTotalSec%60;

  if(ahours == nhours && amins == nmins && asecs == nsecs && almSetted)   // 알람이 설정 된 상태에서 알람 시간과 현재 시간이 일치할 때
  {
    ringingBuzz = true;   // 알람 울리기
    almSetted = false;    // 알람 작동 중지
  }
    


  ///////// 알람 울림 //////////

  if(ringingBuzz)
  {
    tone(8,1000);
    delay(100);
    noTone(8);
    delay(100);
    if(digitalRead(btn3) == HIGH)   // 상단의 버튼을 누르면
      ringingBuzz = false;    // 알람 중지
  }
    
}
