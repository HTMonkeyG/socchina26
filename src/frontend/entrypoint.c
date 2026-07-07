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
#include "frontend/connection/connection.h"

static const char *TAG = "example";

void app_main() {
  // Initialize connection between boards.
  Connection_Initialize();

  // Initialize TFT screen.
  Tft_Initialize();
  Tft_CreatePanel();

  // Main loop.
  while (1) {
    Connection_Update();
    Tft_Update();
    vTaskDelay(1);
  }
}
