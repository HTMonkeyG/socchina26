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

  lvgl_port_lock(0);
  lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello ST7735!");
  lv_obj_center(label);
  lvgl_port_unlock();

  printf("LVGL and ST7735 initialization done.\n");

  while (1) {
    //ESP_LOGI(TAG, "aaa");
    vTaskDelay(1);
  }
}
