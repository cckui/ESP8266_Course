#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN 5
#define DHTTYPE DHT11 // DHT11 or DHT22
DHT dht(DHTPIN, DHTTYPE);

ESP8266WebServer server(80);    // HTTP伺服器Port
IPAddress apIP(192, 168, 6, 6); // 設定AP模式的IP位址為192.168.6.6

const char *ssid = "ESP8266";      // AP SSID
const char *password = "12345678"; // AP PW

void rootRouter()
{
  server.send(200, "text/html", "<p>Hello World! <b>ESP8266 Web Demo</b>!</p>");
}

void setup()
{
  WiFi.mode(WIFI_AP);                                         // 啟用AP模式
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); 
  WiFi.softAP(ssid, password);                                

  Serial.begin(9600);
  Serial.println();

  dht.begin();

  server.on("/demo", []() {
    float temp = dht.readTemperature();
    float humi = dht.readHumidity();
    if (isnan(temp) || isnan(humi))
    {
      Serial.println("Failed to read from DHT sensor!");
      server.send(200, "text/html", "Failed to read from DHT sensor!");
    }
    else
    {
      Serial.printf("Temp:%.2f C | Humi:%.2f %% \r\n", temp, humi); //要印 %，需使用%%
      char htmlStr[600];
      snprintf(htmlStr, 600,
               "<html>\
                  <head>\
                    <meta charset='utf-8'/>\
                    <meta http-equiv='refresh' content='5'/>\
                    <title>esp8266 Web Demo</title>\
                    </head>\
                    <body>\
                      <h1>ESP8266溫溼度</h1>\
                      <p>溫度：%.2f °C </p>\
                      <p>濕度：%.2f %% </p>\
                    </body>\
                </html>",
               temp, humi);

      server.send(200, "text/html", htmlStr);
    }
  });

  server.on("/index.html", rootRouter);
  server.on("/", rootRouter);
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not Found");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
}