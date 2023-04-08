#include <LiquidCrystal.h>;
const int rs = 12, en = 11, d5 = 6, d6 = 5, d7 = 4, d8 = 3;
LiquidCrystal lcd(rs, en, d5, d6, d7, d8);

bool isAlarm = false, isStopwatch = false, isTimer = false;
const int buzzer = 2, led = 13;
const int pushbtn1 = 10;
const int pushbtn2 = 9;
const int pushbtn3 = 8;
const int pushbtn4 = 7;

int hour = 10, minute = 15, second = 5, pis = 0, mls = 0, lastTime = 0, now;
int hourAlarm = 0, minuteAlarm = 0, secondAlarm = 0;
int mode = 0;
int flag = 0, flagAlarm = 0; // indicates 0 = AM, 1 = PM

int btnstate1; // indicates pushbtn's state
int btnstate2;
int btnstate3;
int btnstate4;

void setup()
{
    lcd.begin(16, 2);
    pinMode(pushbtn2, INPUT);
    pinMode(pushbtn3, INPUT);
    pinMode(pushbtn4, INPUT);
    pinMode(pushbtn1, INPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(led, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    if (mode == 0)
        clock();
    else if (mode == 1)
        setTime();
    else if (mode == 2)
        setAlarm();
}

void checkState()
{
    btnstate1 = digitalRead(pushbtn1);
    btnstate2 = digitalRead(pushbtn2);
    btnstate3 = digitalRead(pushbtn3);
    btnstate4 = digitalRead(pushbtn4);

    if (btnstate1 | btnstate2)
    {
        if (btnstate1 == 1)
        {
            mode = 1;
        }
        if (btnstate2 == 1)
        {
            mode = 2;
        }
    }
    else
    {
        mode = 0;
    }
}
