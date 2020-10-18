#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT11 // DHT11 or DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  dht.begin();
}

void loop()
{
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
  }
  else
  {
    Serial.printf("Temp:%.2f C | Humi:%.2f %% \r\n", t, h); //要印 %，需使用%%
  }
  delay(1000);
}