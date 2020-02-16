#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <NeoPixelBus.h>

#define relayPin 27


const char *ssid = ".....";
const char *password = "......";
const char *host = "esp32_stripled";


WebServer server(80);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> LED(1, 12);


void show_ledcolor(uint8_t r, uint8_t g, uint8_t b)
{
        LED.SetPixelColor(0, RgbColor(r, g, b));
        LED.Show();
}

void status(String status)
{
  char data[50];
  snprintf(data, sizeof(data), "{\"status\": %s}",            
           status.c_str());
  server.sendHeader("Connection", "close");
  server.send(200, "application/json", data);
}

void turnoff()
{
  status("off");
  digitalWrite(relayPin, LOW);
}

void turnon() {
  status("on");
  digitalWrite(relayPin, HIGH);
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

if (!MDNS.begin(host))
  {
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  server.on("/off", HTTP_GET, turnoff);
  server.on("/on", HTTP_GET, turnon);
}

void loop()
{
  server.handleClient();
}

