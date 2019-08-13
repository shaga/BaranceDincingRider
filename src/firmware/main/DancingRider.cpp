/* LEDC (LED Controller) fade example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "PwmServoDriver.hpp"
#include "BatteryChecker.hpp"

static const PwmServoDriver::ServoName kServoMotor = PwmServoDriver::kSvr0;
static const PwmServoDriver::ServoName kServoSteer = PwmServoDriver::kSvr3;

PwmServoDriver servo;
BatteryChecker battery_checker;

extern "C" {
    void app_main();
}

void app_main()
{
    int ch;

    esp_err_t ret = servo.Initialize();

    printf("servo driver init:%d\n", ret);

    ret = battery_checker.Initialize();

    ret = servo.Drive(3, 0);

    printf("servo drive: %d\n", ret);

    vTaskDelay(5000 / portTICK_PERIOD_MS);

    uint32_t level = 0;
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        level = battery_checker.GetBatteryLevelRaw();
        printf("hoge(%d)\n", level);
    }
}
