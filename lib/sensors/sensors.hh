#ifndef SENSORS_hh
#define SENSORS_hh

#include <Arduino.h>

namespace sensors
{
    struct distanceSensor{
        uint8_t trigPin;
        uint8_t echoPin;
        uint8_t currentDistance;
    };

    extern distanceSensor forwardSensor;
    extern distanceSensor backwardSensor;

    void setupDistanceSensors(uint8_t trigPinF, uint8_t echoPinF, uint8_t trigPinB, uint8_t echoPinB);
    void startSensors(QueueHandle_t* queuePointer);
    void IRAM_ATTR echoHandler();
    void distanceSensorTask(void* parameters);
}
#endif