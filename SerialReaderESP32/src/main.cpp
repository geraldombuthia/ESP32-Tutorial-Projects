#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

const char *ssid = "AFTERMATH";
const char *password = "YLLEQGQG72F";

static const int LED_PIN = LED_BUILTIN;
static const uint8_t buf_len = 255;
static const uint8_t msg_queue_len = 90;
static QueueHandle_t queue;

// typedef struct Message
// {
//   int id;
//   char *text;
// } Message;

void initWeb() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network: " + String(ssid) + " with IP: " + WiFi.localIP().toString());
}
void sendData(int data)
{
  HTTPClient http;
  WiFiClient client;
  http.begin(client, "http://192.168.8.133:5500/data");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String httpRequestData = "data=" + String(data);
  int httpResponseCode = http.POST(httpRequestData);
  if (httpResponseCode > 0)
  {
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  }
  else
  {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
}
void task1(void *parameter)
{
  while (true)
  {
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.print("Task 1 with cpu: ");
    Serial.println(xPortGetCoreID());
  }
}
void task2(void *parameter)
{
  while (true)
  {
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    Serial.print("Task 2 with cpu: ");
    Serial.println(xPortGetCoreID());
  }
}

void task3(void *parameter)
{
  static int num = 0;
  while (true)
  {
    char msg[buf_len];

    if (Serial.available() > 0)
    {
      String data = Serial.readStringUntil('\n');

      int num = atoi(data.c_str());
      if (xQueueSend(queue, &num, 10) == pdTRUE)
      {
        Serial.println("Send to queue success");
      }
      else
      {
        Serial.println("Send to queue fail");
      }
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void task4(void *parameter)
{
  while (true)
  {
    char *rcvmsg;
    int id;
    if (xQueueReceive(queue, &id, 0) == pdTRUE)
    {
      // Serial.println(id);
      sendData(id);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
// void task4(void *parameter)
// {
//   while (1)
//   {
//     char rcvmsg[10];
//     if (xQueueReceive(queue, (void *)&rcvmsg, 0) == pdTRUE)
//     {
//       Serial.println(rcvmsg);
//     }
//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//   }
// }
// SoftwareSerial mySerial(2, 3); // RX, TX
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  initWeb();
  queue = xQueueCreate(msg_queue_len, sizeof(int));

  // xTaskCreate(
  //     task1,    /* Task function. */
  //     "task 1", /* String with name of task. */
  //     1500,     /* Stack size in bytes. */
  //     NULL,     /* Parameter passed as input of the task */
  //     1,        /* Priority of the task. */
  //     NULL      /* Task handle. */
  // );

  // xTaskCreate(
  //     task2,
  //     "task 2",
  //     1500,
  //     NULL,
  //     1,
  //     NULL);
  xTaskCreate(
      task3,
      "task 3",
      1500,
      NULL,
      2,
      NULL);
  xTaskCreate(
      task4,
      "task 4",
      2500,
      NULL,
      2,
      NULL);
}

void loop()
{
  // put your main code here, to run repeatedly:
  // digitalWrite(LED_BUILTIN, HIGH);
  // Serial.println("Hello World");
  // delay(1000);
  // digitalWrite(LED_BUILTIN, LOW);
  // Serial.println("Goodbye World");
  // delay(1000);
}