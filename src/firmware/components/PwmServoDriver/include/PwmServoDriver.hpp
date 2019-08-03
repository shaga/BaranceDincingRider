#ifndef __PWM_SERVO_DRIVER_HPP__
#define __PWM_SERVO_DRIVER_HPP__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

class PwmServoDriver {
public:
    PwmServoDriver();
    esp_err_t initialize();
    
private:



};

#endif //__PWM_SERVO_DRIVER_HPP__
