#ifndef __BATTERY_CHECKER_HPP__
#define __BATTERY_CHECKER_HPP__

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

class BatteryChecker {
public:
    BatteryChecker();

    esp_err_t Initialize();
    esp_err_t Finalize();

    uint32_t GetBatteryLevelRaw();
private:
    TaskHandle_t task_handle_;
};


#endif //__BATTERY_CHECKER_HPP__
