#ifndef MOTORS_hh
#define MOTORS_hh

#include <Arduino.h>
#include <Stepper.h>

namespace Car{
    class Motors
    {
        public:
        Motors(uint8_t IN1, uint8_t IN2, uint8_t IN3, uint8_t IN4, uint8_t ENA, uint8_t ENB,
                uint8_t sIN1, uint8_t sIN2, uint8_t sIN3, uint8_t sIN4, uint8_t sPot);

        void startTasks();

        void setSpeed(int16_t speed);
        void setDirection(int8_t direction);

        private:
        static void backMotorTask(void* parameter);
        static void frontMotorTask(void* parameter);
        static void motorStopTask(void* parameter);

        uint8_t IN1_, IN2_, IN3_, IN4_, ENA_, ENB_, sPot_;
        int16_t desiredSpeed_;
        int8_t direction_;
        float directionMultiplier_;

        Stepper frontStepper;
        TaskHandle_t backMotorTaskHandle_;
        TaskHandle_t frontMotorTaskHandle_;
        TaskHandle_t motorStopTaskHandle_;
    };
};


#endif