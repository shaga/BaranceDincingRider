#include "BatteryChecker.hpp"
#include "freertos/semphr.h"

static const uint32_t kTaskStackSize = 1024;
static const UBaseType_t kTaskPriority = 2;

static const int kHistorySize = 10;

static const TickType_t kTaskTickCount = (TickType_t)(100 / portTICK_RATE_MS);

static int history_pos = 0;

static uint32_t level_sum = 0;

static uint32_t *level_history;

static SemaphoreHandle_t semaphore = NULL;

static void TaskBatteryRead(void* params) {
    uint32_t buffer = 0;
    TickType_t last_tick = xTaskGetTickCount();
    while (true) {
        buffer = adc1_get_raw((adc1_channel_t) ADC_CHANNEL_6);
        xSemaphoreTake(semaphore, portMAX_DELAY);
        level_sum += buffer;
        if (history_pos >= kHistorySize) {
            level_sum -= level_history[history_pos%kHistorySize];
        }
        level_history[history_pos%kHistorySize] = buffer;
        history_pos++;
        if (history_pos >= kHistorySize * 2) {
            history_pos = kHistorySize;
        }
        xSemaphoreGive(semaphore);
        vTaskDelayUntil(&last_tick, kTaskTickCount);
    }
}

BatteryChecker::BatteryChecker() : task_handle_(NULL) {
}

esp_err_t BatteryChecker::Initialize() {
    esp_err_t ret = ESP_OK;

    ret = Finalize();

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten((adc1_channel_t)ADC_CHANNEL_6, ADC_ATTEN_DB_11);

    level_sum = 0;
    history_pos = 0;
    level_history = (uint32_t*) malloc(kHistorySize*sizeof(uint32_t));

    semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(semaphore);

    xTaskCreate(TaskBatteryRead, "BatteryCheck", kTaskStackSize, NULL, kTaskPriority, &task_handle_);

    return ret;
}

esp_err_t BatteryChecker::Finalize() {
    esp_err_t ret = ESP_OK;

    if (task_handle_ != NULL) {
        vTaskDelete(task_handle_);
        task_handle_ = NULL;
    }

    if (level_history != NULL) {
        free(level_history);
        level_history = NULL;
    }

    return ret;
}

uint32_t BatteryChecker::GetBatteryLevelRaw() {
    xSemaphoreTake(semaphore, portMAX_DELAY);
    int size = history_pos < kHistorySize ? history_pos : kHistorySize;
    uint32_t buffer = level_sum;
    xSemaphoreGive(semaphore);
    return size == 0 ? 0 : (buffer / size);
}