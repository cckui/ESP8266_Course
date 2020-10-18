#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>

// +--------------------+
// |   DHT、LED
// +--------------------+
#define LEDPIN 4
#define DHTPIN 5
#define DHTTYPE DHT11 // DHT11 or DHT22
DHT dht(DHTPIN, DHTTYPE);

// +--------------------+
// |   OLED
// +--------------------+
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// +--------------------+
// |   Wifi
// +--------------------+
#define SSID "test"
#define PASS "12345678"

// +--------------------+
// |   IOT
// +--------------------+
#define IOTHOST "api.thingspeak.com"
#define IOTPORT 80
String GET = "GET /update?key=NAF43GJPLK3RA0IY";

void updateData();
void initOLED();
void oledShow();
float temperature, humidity;

void setup()
{
  delay(3000);

  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, HIGH);

  Serial.begin(9600);
  dht.begin();
  WiFi.begin(SSID, PASS);

  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println(SSID);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  digitalWrite(LEDPIN, LOW);

  Serial.println("Wifi Connected Success!");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  initOLED();
  delay(1000);
}
void loop()
{
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("Failed to read from DHT sensor!");
  }
  else
  {
    Serial.printf("Temp:%.2f C | Humi:%.2f %% \r\n", temperature, humidity); //要印 %，需使用%%
    oledShow();
    updateData();
  }
  delay(20 * 1000);
}

void updateData()
{
  // 設定 ESP8266 作為 Client 端
  WiFiClient client;
  if (!client.connect(IOTHOST, IOTPORT))
  {
    Serial.println("Thingspeak connection failed");
    return;
  }
  else
  {
    String getStr = GET + "&field1=" + String((float)temperature) +
                    "&field2=" + String((float)humidity) +
                    " HTTP/1.1\r\n";
    ;
    client.print(getStr);
    client.print("Host: ");
    client.println(IOTHOST);
    client.print("Connection: close\r\n\r\n");

    delay(1000);

    Serial.println("");
    Serial.println("===== IOT =====");
    String response = client.readString();
    Serial.println(response);

    client.stop();
  }
}

void initOLED()
{

  Wire.begin(2, 14); //SDA=13,SCL=12

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("SSD1306 allocation failed");
  }
  else
  {
    Serial.println("SSD1306 init OK!");
  }

  display.setTextColor(WHITE);
  display.display();
  display.clearDisplay();
}

void oledShow()
{

  display.clearDisplay();

  // display IP
  display.setTextSize(1);
  display.setCursor(0, 7);
  display.print("IP:");
  display.print(WiFi.localIP());

  // display temperature
  display.setTextSize(2);
  display.setCursor(0, 16);
  display.print(temperature);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");

  // display humidity
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humi: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(humidity);
  display.print(" %");

  display.display();
}