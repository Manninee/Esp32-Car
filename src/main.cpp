#include <Arduino.h>
#include <WiFi.h>

#include "webServer.hh"
#include "Motors.hh"
#include "sensors.hh"

void setupWifi();
void serialPortTask(void* parameter);
void httpDataTask(void* parameter);

Car::webServer HttpServer;
Car::Motors motors(27, 26, 25, 33, 14, 32, 5, 19, 18, 21, 35);

QueueHandle_t serialPortQueue;

void setup() {
  Serial.begin(250000);
  delay(1000);
  
  setupWifi();

  HttpServer.startServer();
  xTaskCreate(serialPortTask, "Task0", 8196, nullptr, tskIDLE_PRIORITY + 1, nullptr);
  xTaskCreate(httpDataTask, "Task1", 2048, nullptr, tskIDLE_PRIORITY + 1, nullptr);

  sensors::setupDistanceSensors(22,2, 23, 4);
  sensors::startSensors(HttpServer.getInputQueue());

  motors.startTasks();
}

void loop(){}

void setupWifi(){
  WiFi.mode(WIFI_AP);
  const char * SSID = "SomeSSID";
  const char * PWD = "Something"; 
  IPAddress IP(193, 1, 1, 1);
  IPAddress SUBNET(255, 255, 255, 0);
  WiFi.softAP(SSID, PWD);
  WiFi.softAPConfig(IP, IP, SUBNET);

  delay(500);
}

void serialPortTask(void* parameter){
  serialPortQueue = xQueueCreate(30, sizeof(uint8_t[2]));
  QueueHandle_t* webInput = HttpServer.getInputQueue();
  TickType_t const FREQUENCY = pdMS_TO_TICKS(100);
  TickType_t lastWakeTime = xTaskGetTickCount();
  uint8_t inputBuffer[5];
  uint8_t outputBuffer[2];
  for(;;){
    vTaskDelayUntil(&lastWakeTime, FREQUENCY);
    for(;;){
      if(!Serial.available())
        break;

      Serial.readBytesUntil('\n', inputBuffer, 5);

      if(inputBuffer[1] == '='){
        uint8_t data[2] = {inputBuffer[0], inputBuffer[2]};
        xQueueSend(*webInput, &data, pdMS_TO_TICKS(5));
      }        
    } 

    for(;;){
      if(xQueueReceive(serialPortQueue, &outputBuffer, 0) == pdFALSE)
        break;

      uint8_t dataToSend[5] = {outputBuffer[0], '=', outputBuffer[1], '\n'};
      Serial.write(dataToSend, 5);
    }
  }
}



void httpDataTask(void* parameter){
  QueueHandle_t* webOutput = HttpServer.getOutputQueue();
  uint16_t data[2];

  for(;;){
    xQueueReceive(*webOutput, &data, portMAX_DELAY);

    switch (data[0])
    {    
    case 'S':
      motors.setSpeed(data[1] - 255);
      break;

    case 'D':
      motors.setDirection(data[1] - 100);
      break;
    
    default:
      uint8_t serialData[2] = {(uint8_t)data[0], (uint8_t)data[1]};
      xQueueSend(serialPortQueue, &serialData, pdMS_TO_TICKS(5));
      break;
    }
  }
}