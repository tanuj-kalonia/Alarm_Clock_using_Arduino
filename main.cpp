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

//////////****************CLOCK***************//////////
void clock()
{
    lcd.setCursor(0, 0);
    lcd.print("What time is it?");
    lcd.setCursor(0, 1);
    lcd.print("It's ");
    if (hour < 10)
        lcd.print("0");
    lcd.print(hour);
    lcd.print(":");
    if (minute < 10)
        lcd.print("0");
    lcd.print(minute);
    lcd.print(":");
    if (second < 10)
        lcd.print("0");
    lcd.print(second);

    if (flag == 0)
        lcd.print(" AM");
    if (flag == 1)
        lcd.print(" PM");

    checkState();
    delay(200);
    runningClock(&hour, &minute, &second, &now, &lastTime, &mls, &flag);
}

void runningClock(int *_hour, int *_minute, int *_second, int *_now, int *_lastTime, int *_mls, int *_flag)
{
    int increHour = 0;
    *_now = (millis());
    *_mls += (*_now - *_lastTime);
    *_lastTime = *_now;

    if (*_mls >= 1000)
    {
        *_second += (*_mls / 1000);
        *_mls %= 1000;
    }
    if (*_second >= 60)
    {
        *_minute += *_second / 60;
        *_second %= 60;
    }
    if (*_minute >= 60)
    {
        increHour = *_minute / 60;
        *_minute %= 60;
    }
    if (increHour % 24 == 0)
        ; // hour as before
    else
    {
        increHour %= 24;
        if (increHour < 12)
        {
            if (*_hour == 12)
            {
                *_hour = increHour;
            }
            else if (*_hour + increHour >= 12)
            {
                *_hour -= 12;
                *_flag = *_flag == 0 ? 1 : 0;
            }
        }
        else
        {
            *_hour = ((*_hour + increHour) % 12);
            *_flag = *_flag == 0 ? 1 : 0;
        }
        if (*_flag == 0 & *_hour == 12)
            *_hour = 0;
        else if (*_flag == 1 & *_hour == 0)
            *_hour = 12;
    }
    if (isAlarm)
        checkAlarm();
}

//////////****************TIMESETTING***************//////////
void setTime()
{
    int *main_var;
    int X, Y;
    lcd.clear();

    int currentChangeIn = 1; // 1 is hour 2 is minute and so on
    while (true)
    {
        lcd.setCursor(0, 0);
        lcd.print("Set Time");
        lcd.setCursor(0, 1);
        if (hour < 10)
            lcd.print("0");
        lcd.print(hour);
        lcd.print(":");
        if (minute < 10)
            lcd.print("0");
        lcd.print(minute);
        lcd.print(":");
        if (second < 10)
            lcd.print("0");
        lcd.print(second);

        if (flag == 0)
            lcd.print(" AM");
        if (flag == 1)
            lcd.print(" PM");

        btnstate1 = digitalRead(pushbtn1);
        btnstate2 = digitalRead(pushbtn2);
        btnstate3 = digitalRead(pushbtn3);
        btnstate4 = digitalRead(pushbtn4);
        if (btnstate1 == 1)
        {
            mode = 0;
            delay(200);
            break;
        }
        if (btnstate3 == 1)
        {
            if (currentChangeIn == 3)
                currentChangeIn = 0;
            currentChangeIn += 1;
            delay(200);
        }
        if (currentChangeIn == 1)
        {
            main_var = &hour;
            X = 0;
            Y = 1;

            if (digitalRead(pushbtn2) == 1)
            {
                hour += 1;
                if (hour == 13)
                    hour = 1;
                else if (hour == 12 & flag == 0)
                    hour = 0;
                delay(200);
            }
        }
        if (currentChangeIn == 2)
        {
            main_var = &minute;
            X = 3;
            Y = 1;

            if (digitalRead(pushbtn2) == 1)
            {
                minute += 1;
                if (minute == 60)
                    minute = 0;
                delay(200);
            }
        }
        if (currentChangeIn == 3)
        {
            main_var = &second;
            X = 6;
            Y = 1;

            if (digitalRead(pushbtn2) == 1)
            {
                second += 1;
                if (second == 60)
                    second = 0;
                delay(200);
            }
        }
        if (btnstate4 == 1)
        {
            flag = flag == 0 ? 1 : 0;
            if (flag == 0 & hour == 12)
                hour = 0;
            else if (flag == 1 & hour == 0)
                hour = 12;
            delay(200);
        }
        blink(*main_var, X, Y);
        runningClock(&hour, &minute, &second, &now, &lastTime, &mls, &flag);
    }
}

//////////****************ALARM***************//////////
void setAlarm()
{
    int *main_var;
    int X, Y;
    lcd.clear();

    int currentChangeIn = 1; // 1 is hour 2 is minute and so on
    while (true)
    {
        lcd.setCursor(0, 0);
        lcd.print("Set Alarm");
        lcd.setCursor(0, 1);
        if (hourAlarm < 10)
            lcd.print("0");
        lcd.print(hourAlarm);
        lcd.print(":");
        if (minuteAlarm < 10)
            lcd.print("0");
        lcd.print(minuteAlarm);
        lcd.print(":");
        if (secondAlarm < 10)
            lcd.print("0");
        lcd.print(secondAlarm);

        if (flagAlarm == 0)
            lcd.print(" AM");
        if (flagAlarm == 1)
            lcd.print(" PM");

        btnstate1 = digitalRead(pushbtn1);
        btnstate2 = digitalRead(pushbtn2);
        btnstate3 = digitalRead(pushbtn3);
        btnstate4 = digitalRead(pushbtn4);
        if (btnstate1 == 1)
        {
            mode = 0;
            delay(200);
            break;
        }
        if (btnstate3 == 1)
        {
            if (currentChangeIn == 3)
                currentChangeIn = 0;
            currentChangeIn += 1;
            delay(200);
        }
        if (currentChangeIn == 1)
        {
            main_var = &hourAlarm;
            X = 0;
            Y = 1;

            if (digitalRead(pushbtn2) == 1)
            {
                hourAlarm += 1;
                if (hourAlarm == 13)
                    hourAlarm = 1;
                else if (hourAlarm == 12 & flagAlarm == 0)
                    hourAlarm = 0;
                delay(200);
            }
        }
        if (currentChangeIn == 2)
        {
            main_var = &minuteAlarm;
            X = 3;
            Y = 1;

            if (digitalRead(pushbtn2) == 1)
            {
                minuteAlarm += 1;
                if (minuteAlarm == 60)
                    minuteAlarm = 0;
                delay(200);
            }
        }
        if (currentChangeIn == 3)
        {
            main_var = &secondAlarm;
            X = 6;
            Y = 1;

            if (digitalRead(pushbtn2) == 1)
            {
                secondAlarm += 1;
                if (secondAlarm == 60)
                    secondAlarm = 0;
                delay(200);
            }
        }
        if (btnstate4 == 1)
        {
            flagAlarm = flagAlarm == 0 ? 1 : 0;
            if (flagAlarm == 0 & hourAlarm == 12)
                hourAlarm = 0;
            else if (flagAlarm == 1 & hourAlarm == 0)
                hourAlarm = 12;
            delay(200);
        }
        blink(*main_var, X, Y);
    }
    isAlarm = true;
    runningClock(&hour, &minute, &second, &now, &lastTime, &mls, &flag);
}

void checkAlarm()
{
    if (hour == hourAlarm & minute == minuteAlarm & second == secondAlarm & flag == flagAlarm)
    {
        tone(buzzer, 1000);
        delay(3000);
        noTone(buzzer);
        isAlarm = false;
    }
}
