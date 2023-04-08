
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