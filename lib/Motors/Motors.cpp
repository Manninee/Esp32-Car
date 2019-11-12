#include "Motors.hh"

namespace Car
{
    Motors::Motors(uint8_t IN1, uint8_t IN2, uint8_t IN3, uint8_t IN4, uint8_t ENA, uint8_t ENB,
    uint8_t sIN1, uint8_t sIN2, uint8_t sIN3, uint8_t sIN4, uint8_t sPot):
    IN1_(IN1), IN2_(IN2), IN3_(IN3), IN4_(IN4), ENA_(ENA), ENB_(ENB), sPot_(sPot),
    desiredSpeed_(0), direction_(0), directionMultiplier_(1.0), frontStepper(32, sIN1, sIN2, sIN3, sIN4),
    backMotorTaskHandle_(nullptr), frontMotorTaskHandle_(nullptr), motorStopTaskHandle_(nullptr)
    {
        uint16_t const PWMFREQ = 20000;
        uint8_t const PWMRESOLUTION = 10;
        ledcSetup(0, PWMFREQ, PWMRESOLUTION);
        ledcSetup(1, PWMFREQ, PWMRESOLUTION);

        ledcAttachPin(ENA_, 0);
        ledcAttachPin(ENB_, 1);

        pinMode(IN1_, OUTPUT);
        pinMode(IN2_, OUTPUT);
        pinMode(IN3_, OUTPUT);
        pinMode(IN4_, OUTPUT);
        pinMode(sPot_, INPUT);

        frontStepper.setSpeed(800);
    }

    void Motors::startTasks(){
        xTaskCreate(Motors::backMotorTask, "m1", 2048, this, tskIDLE_PRIORITY + 1, &backMotorTaskHandle_);
        xTaskCreate(Motors::frontMotorTask, "m2", 2048, this, tskIDLE_PRIORITY + 1, &frontMotorTaskHandle_);
        xTaskCreate(Motors::motorStopTask, "m3", 2048, this, tskIDLE_PRIORITY + 2, &motorStopTaskHandle_);
    }

    void Motors::setSpeed(int16_t speed){
        desiredSpeed_ = 2 * speed;
        xTaskNotify(motorStopTaskHandle_, 1, eSetValueWithOverwrite);
        xTaskNotifyGive(backMotorTaskHandle_);
    }

    void Motors::setDirection(int8_t direction){
        direction_ = direction;

        xTaskNotifyGive(frontMotorTaskHandle_);
            
        // Update back wheel speed multiplier
        directionMultiplier_ = 1 - abs(direction_ / 400);
        xTaskNotifyGive(backMotorTaskHandle_);     
    }

    void Motors::backMotorTask(void* parameter){
        Motors* motors = (Motors *) parameter;
        TickType_t xLastWakeTime = xTaskGetTickCount();
        TickType_t const frequency = pdMS_TO_TICKS(10);
        int16_t currentPWM = 0;
        uint8_t const STEP = 10;
        bool pwmChanging = false;

        for(;;){
            if(!pwmChanging)
                ulTaskNotifyTake(pdTRUE, portMAX_DELAY);           
            else
                vTaskDelayUntil(&xLastWakeTime, frequency);
                
            // Check if speed is changing
            int16_t pwmDifference = motors->desiredSpeed_ - currentPWM;
            if(pwmDifference != 0){
                int8_t sign = (pwmDifference > 0) ? 1:-1;

                pwmChanging = true;
                
                if(abs(pwmDifference) < STEP)
                    currentPWM = motors->desiredSpeed_;
                else
                    currentPWM += (STEP * sign);
            }
            else
                pwmChanging = false;

            uint16_t rightMotorPwm = 0, leftMotorPwm = 0;
            
            // Calculate values for both motors
            if(currentPWM != 0){
                leftMotorPwm = abs(currentPWM) + 513;
                rightMotorPwm = leftMotorPwm;
                
                // Slow down other motor when turning
                if(motors->direction_ != 0){
                    if(motors->direction_ < 0)
                        leftMotorPwm *= motors->directionMultiplier_;
                    else
                        rightMotorPwm *= motors->directionMultiplier_;
                }
            }               
            
            if(pwmChanging){
                if(currentPWM > 0){
                    digitalWrite(motors->IN1_, LOW);
                    digitalWrite(motors->IN2_, HIGH);
                    digitalWrite(motors->IN3_, LOW);
                    digitalWrite(motors->IN4_, HIGH);
                }
                else if(currentPWM < 0){
                    digitalWrite(motors->IN1_, HIGH);
                    digitalWrite(motors->IN2_, LOW);
                    digitalWrite(motors->IN3_, HIGH);
                    digitalWrite(motors->IN4_, LOW);
                }
                else{
                    digitalWrite(motors->IN1_, LOW);
                    digitalWrite(motors->IN2_, LOW);
                    digitalWrite(motors->IN3_, LOW);
                    digitalWrite(motors->IN4_, LOW);
                }
            }

            ledcWrite(0, leftMotorPwm);
            ledcWrite(1, rightMotorPwm);
        }
    }

    void Motors::frontMotorTask(void* parameter){
        Motors* motors = (Motors *) parameter;
        uint8_t const adcPin = motors->sPot_;
        uint8_t const DEADZONE = 25;
        uint16_t const CENTER = 1500;
        uint8_t const STEPS = 5;
        uint16_t desiredValue = CENTER;
        uint16_t potValue = 0;
        for(;;){
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            for(;;){
                potValue = analogRead(adcPin);
                desiredValue = CENTER + 3 * motors->direction_;
                int16_t difference = potValue - desiredValue;

                if(abs(difference) < DEADZONE)
                    break;
                else if(difference < 0)
                    motors->frontStepper.step(-STEPS);
                else
                    motors->frontStepper.step(STEPS);

                taskYIELD();
            }
        }
    }

    void Motors::motorStopTask(void* parameter){
        Motors* motors = (Motors *) parameter;
        TickType_t const WAITTIME = pdMS_TO_TICKS(300);
        uint32_t notifiedValue = 0;

        for(;;){
            xTaskNotifyWait(0x00, ULONG_MAX, &notifiedValue, WAITTIME);
            if(notifiedValue == 0){
                motors->desiredSpeed_ = 0;
                motors->setDirection(0);
            }
        }

    }
};