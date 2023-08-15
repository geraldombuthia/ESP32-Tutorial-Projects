#include <Arduino.h>
#include <WiFi.h>
#include <httpClient.h>
#include <Arduino_JSON.h>

const char *ssid = "AFTERMATH";
const char *password = "YLLEQGQG72F";

int n = 0;

String readSerial()
{
  String dataRead = "";
  while (true)
  {
    if (Serial.available())
    {
      char c = Serial.read();
      if (c == '\n')
      {
        break;
      }
      dataRead += c;
    }
  }
  return dataRead;
}

void setup()
{
  // put your setup code here, to run once:
  WiFi.begin(ssid, password);
  Serial.begin(115200);
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
  if (WiFi.status() == WL_CONNECTED)
  {
    http.begin(client, "http://192.168.8.133:5500/data");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    if (Serial.available())
    {
      String httpRequestData = "Count=" + Serial.readStringUntil('\n');
      int httpResponseCode = http.POST(httpRequestData);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }

    // Serial.println(readSerial());
    http.end();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  else
  {
    Serial.println("WiFi Disconnected");
    WiFi.disconnect();
    WiFi.reconnect();
  }
}
// void loop()
// {
//   // put your main code here, to run repeatedly:
//   if (WiFi.status() == WL_CONNECTED)
//   {
//     HTTPClient http;
//     WiFiClient client;

//     http.begin(client, "http://192.168.8.133:5500/data");
//     http.addHeader("Content-Type", "application/x-www-form-urlencoded");

//     int httpResponseCode = http.POST("Count=" + readSerial());

//     Serial.print("HTTP Response code: ");
//     Serial.println(httpResponseCode);
//     // Serial.println(readSerial());
//     http.end();
//   }
//   else
//   {
//     Serial.println("WiFi Disconnected");
//     WiFi.disconnect();
//     WiFi.reconnect();
//   }
//   vTaskDelay(1000 / portTICK_PERIOD_MS);
// }