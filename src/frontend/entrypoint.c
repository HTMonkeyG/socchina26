#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_lvgl_port.h>
#include <esp_lcd_panel_dev.h>
#include <esp_lcd_ili9341.h>
#include <esp_log.h>
#include <sdkconfig.h>

#include "frontend/ui/tft.h"

static const char *TAG = "example";

void app_main() {
  Tft_Initialize();
  Tft_CreatePanel();

  while (1) {
    Tft_Update();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
