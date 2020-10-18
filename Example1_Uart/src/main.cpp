#include <Arduino.h>

String receiveData = "";
uint16 count = 1;

void setup()
{
    // uart baud rate 設定
    Serial.begin(9600);
}

void loop()
{

    if (Serial.available() > 0)
    {
        char Serial_Buffer = Serial.read();
        if (Serial_Buffer != '\n') 
        {
            receiveData = receiveData + Serial_Buffer;
        }
        else
        {

            if (receiveData == "START\r")
            {
                Serial.println(">>> in START");
            }
            else if (receiveData == "STOP\r")
            {
                Serial.println(">>> in STOP");
            }
            else
            {
                Serial.print(receiveData);
            }

            receiveData = "";
        }
    }
    else
    {
        if (count % 50 == 0)
        {
            Serial.print(count);
            Serial.println(" | Hello World!");
        }
        count++;
    }
    delay(0.1 * 1000);
}
