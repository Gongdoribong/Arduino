#include <Arduino.h>
#include <LiquidCrystal_I2C.h>


int AA = 6; //모터 디지털핀
int AB = 7; //모터 디지털핀
int red = 9;    //RGB red
int green = 10;    //RGB green
int blue = 11;    //RGB blue
int R, G, B, air, alcohol;

LiquidCrystal_I2C lcd(0x27, 16, 2);



///////////////////////////////////////////

void setup() {
    Serial.begin(9600);
    lcd.begin(); //lcd 초기화
    lcd.backlight();    //lcd 백라이트 켜기
    for(int i = 4; i < 12; i++)
    {
        pinMode(i, OUTPUT);    //RGBled, 피에조 세팅
    }
    pinMode(A1, INPUT); //공기질
    pinMode(A3, INPUT); //알코올
}

void loop() {
    alcohol = analogRead(A3);   //알코올 입력
    Serial.println(alcohol);

    if(alcohol > 250)
    {
        lcd.setCursor(2,0);
        lcd.print("Shut off!"); 
        alcoholWarning();
    }
    else
    {
        digitalWrite(4, HIGH);
        air = avgAnalogRead(A1);
        airWarning(air);
        
    }
    
}


//공기질, 알코올, RGB led : 아날로그
//피에조, 수중펌프, dc 모터 : 디지털

//수중펌프 가동 시간
//공기질, 알코올 입력 받기!

void airWarning(int n)       //공기질
{
    if(n < 181)    //안전
    {
        R = 0;
        G = 255;
        B = 0;
    }

    else if (n>181 && n<300)  //경고
    {
        R = 255;
        G = 255;
        B = 0;
    }

    else    //위험
    {
        R = 255;
        G = 0;
        B = 0;
        lcd.setCursor(2,0);
        lcd.print("Open the window.");
    }

    analogWrite(red, R);
    analogWrite(green, G);
    analogWrite(blue, B);
}

void alcoholWarning()        //알코올 감지
{
    digitalWrite(4, LOW);

    //빨간색 발광
    analogWrite(red, 255);
    analogWrite(green, 0);
    analogWrite(blue, 0);

    delay(100);

    analogWrite(red, 0);

    delay(100);

    tone(8,3000);   //경고음

    digitalWrite(AA, HIGH);
    digitalWrite(AB, LOW);
    delay(1000);    //물 쏟는 시간

    digitalWrite(AA, LOW);
    digitalWrite(AB, LOW);
    return 0;
}

int avgAnalogRead(int pin)  //공기질 입력받기
{
    float value = 0;
    for(int i = 0; i < 10; i++)
    {
        value += analogRead(pin);
        delay(100);
    }

    value = value / 10;
    Serial.println(value);
    return (value);
}