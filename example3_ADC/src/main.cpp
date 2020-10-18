#include <Arduino.h>

const int analogInPin = A0;

int sensorValue = 0;
float MappingValue = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  sensorValue = analogRead(analogInPin);

  //map(value, fromLow, fromHigh, toLow, toHigh)
  //ESP8266 ADC為 10bit ADC，Voltage 0~1v
  MappingValue = map(sensorValue, 0, 1023, 0, 1000);

  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.print(MappingValue);
  Serial.println(" (mV)");

  delay(1000);
}