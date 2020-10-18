#include <ESP8266WiFi.h>
#include <time.h>

const char *ssid = "test";
const char *password = "12345678";

int ledPin = 4;

int timezone = 8 * 3600; //設定時區因為台灣是+8所以->時區*3600
int dst = 0;

void setup()
{

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  delay(3000);

  WiFi.begin(ssid, password);
  Serial.begin(9600);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println(ssid);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  digitalWrite(ledPin, LOW);

  Serial.println("Wifi Connected Success!");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  configTime(timezone, dst, "time.stdtime.gov.tw", "time.google.com");
}

void loop()
{

  time_t now = time(nullptr);
  struct tm *p_tm = localtime(&now);

  Serial.print(p_tm->tm_year + 1900);
  Serial.print("-");
  Serial.print(p_tm->tm_mon + 1);
  Serial.print("-");
  Serial.print(p_tm->tm_mday);

  Serial.print(" ");

  Serial.print(p_tm->tm_hour);
  Serial.print(":");
  Serial.print(p_tm->tm_min);
  Serial.print(":");
  Serial.println(p_tm->tm_sec);

  delay(5000);
}