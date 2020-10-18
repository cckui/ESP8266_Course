#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <TridentTD_LineNotify.h>

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
// |   Line
// +--------------------+
#define LINETOKEN "w8q9V6NABxHYbB24kAZ5ETAx8bXisOX20aYYZIIahDf"

void initOLED();
void oledShow();
void sendLineNotify();
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
    sendLineNotify();
  }
  delay(20 * 1000);
}

void sendLineNotify()
{
  String message = "\nIP:" + WiFi.localIP().toString();
  message += "\n溫度:" + String((float)temperature);
  message += "\n濕度:" + String((float)humidity);
  LINE.setToken(LINETOKEN);
  LINE.notify(message);

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