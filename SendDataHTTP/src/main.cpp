#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "AFTERMATH";
const char *password = "YLLEQGQG72F";

const char *serverName = "http://192.168.8.133:5500/data";

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;
unsigned int no = 0;
void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  HTTPClient http;
  WiFiClient client;
  if ((millis() - lastTime) > timerDelay)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("Connected to WiFi");

      http.begin(client, serverName);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      
      String httpRequestData = "Counter="+ String(no++);
      int httpResponseCode = http.POST(httpRequestData);

      if (httpResponseCode > 0)
      {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String response = http.getString();
        Serial.println(response);
      }
      else
      {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
    }
    lastTime = millis();
  }
}