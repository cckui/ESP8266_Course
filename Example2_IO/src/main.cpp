#include <Arduino.h>

#define LED 4
#define pushButton 0

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(pushButton, INPUT);
}

void loop()
{

  int buttonState = digitalRead(pushButton);

  if (buttonState != 0)
  {
    digitalWrite(LED, HIGH);
    // 單位:ms  1s=1000ms
    delay(1 * 1000);
    digitalWrite(LED, LOW);
    delay(1 * 1000);
  }
  else
  {

    for (int dutyCycle = 0; dutyCycle < 1023; dutyCycle++)
    {
      analogWrite(LED, dutyCycle);
      delay(1);
    }

    for (int dutyCycle = 1023; dutyCycle > 0; dutyCycle--)
    {
      analogWrite(LED, dutyCycle);
      delay(1);
    }
  }
}