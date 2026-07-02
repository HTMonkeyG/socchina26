#include <driver/gpio.h>
#include <esp_lvgl_port.h>
#include <esp_lcd_panel_dev.h>
#include <esp_lcd_ili9341.h>
#include <esp_log.h>
#include "frontend/ui/tft.h"

#define PIN_NUM_MOSI 13
#define PIN_NUM_CLK  12
#define PIN_NUM_CS   10
#define PIN_NUM_DC   19
#define PIN_NUM_RST  20
#define PIN_NUM_BCKL 8

#define DISP_WIDTH  320
#define DISP_HEIGHT 240

#define SPI_CLOCK_SPEED (40 * 1000 * 1000) // 40MHz

void Tft_Initialize() {
  const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
  esp_err_t err = lvgl_port_init(&lvgl_cfg);
  if (err != ESP_OK) {
    printf("LVGL port initialization failed\n");
    return;
  }

  spi_bus_config_t bus_cfg = {
    .mosi_io_num = PIN_NUM_MOSI,
    .miso_io_num = -1,
    .sclk_io_num = PIN_NUM_CLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = DISP_WIDTH * DISP_HEIGHT * sizeof(uint16_t),
  };
  ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO));

  esp_lcd_panel_io_handle_t io_handle = NULL;
  esp_lcd_panel_io_spi_config_t io_config = {
    .dc_gpio_num = PIN_NUM_DC,
    .cs_gpio_num = PIN_NUM_CS,
    .pclk_hz = SPI_CLOCK_SPEED,
    .lcd_cmd_bits = 8,
    .lcd_param_bits = 8,
    .spi_mode = 0,
    .trans_queue_depth = 10,
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(SPI2_HOST, &io_config, &io_handle));

  esp_lcd_panel_handle_t panel_handle = NULL;
  esp_lcd_panel_dev_config_t panel_config = {
    .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
    .reset_gpio_num = PIN_NUM_RST,
    .bits_per_pixel = 16,
    .vendor_config = NULL,
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

  const lvgl_port_display_cfg_t disp_cfg = {
    .io_handle = io_handle,
    .panel_handle = panel_handle,
    .buffer_size = DISP_WIDTH * DISP_HEIGHT,
    .double_buffer = false,
    .hres = DISP_WIDTH,
    .vres = DISP_HEIGHT,
    .monochrome = false,
    .color_format = LV_COLOR_FORMAT_RGB565,
    .rotation = {
      .swap_xy = true,
      .mirror_x = false,
      .mirror_y = false,
    },
    .flags = {
      .buff_dma = true,
    }
  };
  lv_disp_t * disp = lvgl_port_add_disp(&disp_cfg);
  if (disp == NULL) {
    printf("Failed to add display to LVGL\n");
    return;
  }

  gpio_config_t bk_gpio_config = {
    .pin_bit_mask = 1ULL << PIN_NUM_BCKL,
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
  };
  gpio_config(&bk_gpio_config);
  gpio_set_level(PIN_NUM_BCKL, 1);
}
