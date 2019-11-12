#include "sensors.hh"

namespace sensors
{

    distanceSensor forwardSensor;
    distanceSensor backwardSensor;

    uint64_t echoStart = 0;
    uint64_t echoEnd = 0;

    QueueHandle_t* queue = nullptr;

    void setupDistanceSensors(uint8_t trigPinF, uint8_t echoPinF, uint8_t trigPinB, uint8_t echoPinB){
        pinMode(trigPinF, OUTPUT);
        pinMode(trigPinB, OUTPUT);
        pinMode(echoPinF, INPUT);
        pinMode(echoPinB, INPUT);
        forwardSensor = {trigPinF, echoPinF, 0};
        backwardSensor = {trigPinB, echoPinB, 0};
    }

    void startSensors(QueueHandle_t* queuePointer){
        queue = queuePointer;
        xTaskCreate(distanceSensorTask, "s1", 2048, nullptr, tskIDLE_PRIORITY + 1, nullptr);
    }

    void echoHandler(){
        echoEnd = micros();
        if(echoStart == 0){
            echoStart = echoEnd;
            echoEnd = 0;
        }
    }

    void distanceSensorTask(void* parameters){
        TickType_t const WAITTIME = pdMS_TO_TICKS(25);
        distanceSensor* sensorPointer = nullptr;
        float const multiplier = 0.017;
        bool sensorSelector = true;
        
        for(;;){
            if(sensorSelector)
                sensorPointer = &forwardSensor;
            else
                sensorPointer = &backwardSensor;
            sensorSelector = !sensorSelector;
            
            digitalWrite(sensorPointer->trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(sensorPointer->trigPin, LOW);
            attachInterrupt(sensorPointer->echoPin, echoHandler, CHANGE);
            vTaskDelay(WAITTIME);
            detachInterrupt(sensorPointer->echoPin);

            // Calculate Distance
            if(echoStart != 0 && echoEnd != 0){
                float fDistance = (echoEnd - echoStart) * multiplier;
                uint16_t iDistance = round(fDistance);
                if(iDistance > 200)
                    iDistance = 201;
                sensorPointer->currentDistance = (uint8_t)iDistance;
            }
            else
                sensorPointer->currentDistance = 201;
            
            echoEnd = 0; echoStart = 0;

            // Send values
            if(queue != nullptr){
                char sensor = sensorSelector ? 'F' : 'B';
                uint8_t data[2] = {sensor, sensorPointer->currentDistance};
                xQueueSend(*queue, (void*) &data, 0);
            }
                
            vTaskDelay(WAITTIME);        
        }
    }
}