#ifndef __PWM_SERVO_DRIVER_HPP__
#define __PWM_SERVO_DRIVER_HPP__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

struct ServoIo {
    ledc_timer_t    pwm_timer;
    ledc_channel_t  pwm_channel;
    gpio_num_t      pwm_io;
};

class PwmServoDriver {
public:
    enum ServoName {
        kSvr0,
        kSvr1,
        kSvr2,
        kSvr3,
        kServoCount,
    };

    PwmServoDriver();
    esp_err_t Initialize();
    esp_err_t Drive(int ch, double pos);
    void SetFlip(int ch, bool is_flip);
    void SetTrim(int ch, int16_t trim);
private:
    enum UseTimeChannel {
        kTimerSvr01,
        kTimerSvr23,
        kTimerCount,
    };

    static const uint32_t kPwmFrequency;
    static const uint16_t kDefaultServoCenter;
    static const uint16_t kServoRange;
    static const uint16_t kServoPosMin;
    static const uint16_t kServoPosMax;
    static const ledc_timer_t kUseTimer[kTimerCount];
    static const ServoIo kServoInfo[kServoCount]; 

    esp_err_t InitTimers();
    esp_err_t InitPwm();

    bool is_flip_[kServoCount];
    int16_t trim_[kServoCount];
};

#endif //__PWM_SERVO_DRIVER_HPP__
