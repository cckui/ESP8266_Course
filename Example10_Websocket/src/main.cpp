// Ref https://github.com/morrissinger/ESP8266-Websocket

#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

#define pushButton 0
#define LED 4
#define HOST "esp8266.17micro.com"
#define PORT 8080

const char *ssid = "test";
const char *password = "12345678";

char path[] = "/ws";
char host[] = "esp8266.17micro.com";

WebSocketClient webSocketClient;
WiFiClient client;

void setup()
{
	pinMode(pushButton, INPUT);
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);

	Serial.begin(9600);
	delay(1000);

	// +------------------------------------
	// |  Init wifi
	// +------------------------------------
	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	delay(5000);
	// +------------------------------------
	// |  Init websocket
	// +------------------------------------
	if (client.connect(HOST, PORT))
	{
		Serial.println("Connected");
	}
	else
	{
		Serial.println("Connection failed.");
		while (1)
		{
			// Hang on failure
		}
	}

	// Handshake with the server
	webSocketClient.path = path;
	webSocketClient.host = host;
	if (webSocketClient.handshake(client))
	{
		Serial.println("Handshake successful");
	}
	else
	{
		Serial.println("Handshake failed.");
		while (1)
		{
			// Hang on failure
		}
	}
}

void loop()
{
	String recvData;

	int buttonState = digitalRead(pushButton);

	if (buttonState == 0)
	{
		webSocketClient.sendData("Click Push Button!");
	}

	if (client.connected())
	{

		webSocketClient.getData(recvData);
		if (recvData.length() > 0)
		{

			if (recvData == "/open")
			{
				digitalWrite(LED, LOW);
			}
			else if (recvData == "/close")
			{
				digitalWrite(LED, HIGH);
			}
			else
			{
				Serial.print("Received data: ");
				Serial.println(recvData);
			}
		}
	}
	else
	{
		Serial.println("Client disconnected.");
		while (1)
		{
			// Hang on disconnect.
		}
	}

	// wait to fully let the client disconnect
	delay(500);
}