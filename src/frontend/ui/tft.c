#include <math.h>
#include <driver/gpio.h>
#include <esp_lvgl_port.h>
#include <esp_lcd_panel_dev.h>
#include <esp_lcd_ili9341.h>
#include <esp_log.h>
#include "shared/packets.h"
#include "frontend/control/manager.h"
#include "frontend/ui/tft.h"
#include "frontend/ui/menu/menu.h"
#include "frontend/ui/input.h"

#define kTftPinMosi 13
#define kTftPinClk  12
#define kTftPinCs   10
#define kTftPinDc   19
#define kTftPinRst  20
#define kTftPinBg 8

// 40MHz
#define kTftSpiSpeed (40 * 1000 * 1000)

// 20Hz
#define kTftUpdateInteval (configTICK_RATE_HZ / 20)

typedef struct {
  u32 lastUpdateTick;
  UiLoadingMenu loading;
  UiFftChartMenu chart1;
  UiMeasureMenu measure;
  UiOcpMenu ocp;
} Tft;

static Tft gTft = {0};

void Tft_Initialize() {
  const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
  esp_err_t err = lvgl_port_init(&lvgl_cfg);
  if (err != ESP_OK) {
    printf("LVGL port initialization failed\n");
    return;
  }

  spi_bus_config_t bus_cfg = {
    .mosi_io_num = kTftPinMosi,
    .miso_io_num = -1,
    .sclk_io_num = kTftPinClk,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = kTftWidth * kTftHeight * sizeof(uint16_t),
  };
  ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO));

  esp_lcd_panel_io_handle_t io_handle = NULL;
  esp_lcd_panel_io_spi_config_t io_config = {
    .dc_gpio_num = kTftPinDc,
    .cs_gpio_num = kTftPinCs,
    .pclk_hz = kTftSpiSpeed,
    .lcd_cmd_bits = 8,
    .lcd_param_bits = 8,
    .spi_mode = 0,
    .trans_queue_depth = 10,
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(SPI2_HOST, &io_config, &io_handle));

  esp_lcd_panel_handle_t panel_handle = NULL;
  esp_lcd_panel_dev_config_t panel_config = {
    .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
    .reset_gpio_num = kTftPinRst,
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
    .buffer_size = kTftWidth * kTftHeight,
    .double_buffer = false,
    .hres = kTftWidth,
    .vres = kTftHeight,
    .monochrome = false,
    .color_format = LV_COLOR_FORMAT_RGB565,
    .rotation = {
      .swap_xy = true,
      .mirror_x = false,
      .mirror_y = false,
    },
    .flags = {
      .buff_dma = true,
      .swap_bytes = true
    }
  };
  lv_disp_t * disp = lvgl_port_add_disp(&disp_cfg);
  if (disp == NULL) {
    printf("Failed to add display to LVGL\n");
    return;
  }

  gpio_config_t bk_gpio_config = {
    .pin_bit_mask = 1ULL << kTftPinBg,
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
  };
  gpio_config(&bk_gpio_config);
  gpio_set_level(kTftPinBg, 1);

  printf("LVGL and ST7735 initialization done.\n");
}

void Tft_CreatePanel() {
  lvgl_port_lock(0);

  UiLoadingMenu_Initialize(&gTft.loading);
  UiFftChartMenu_Initialize(&gTft.chart1);
  UiMeasureMenu_Initialize(&gTft.measure);
  UiOcpMenu_Initialize(&gTft.ocp);

  UiLoadingMenu_Show(&gTft.loading);

  lvgl_port_unlock();
}

void Tft_Update() {
  if (xTaskGetTickCount() - gTft.lastUpdateTick < kTftUpdateInteval)
    return;
  gTft.lastUpdateTick = xTaskGetTickCount();

  // Change the screen to loading if the backend is not ready or died.
  if (Manager_IsLoading() && lv_screen_active() != gTft.loading.screen)
    UiLoadingMenu_Show(&gTft.loading);
  else if (!Manager_IsLoading() && lv_screen_active() == gTft.loading.screen)
    UiMeasureMenu_Show(&gTft.measure);

  if (lv_screen_active() != gTft.loading.screen) {
    if (Input_IsPressed(kInputKey_Left))
      UiMeasureMenu_Show(&gTft.measure);
    else if (Input_IsPressed(kInputKey_Right))
      UiFftChartMenu_Show(&gTft.chart1);
  }

  // Update screen states.
  if (lv_screen_active() == gTft.measure.screen)
    UiMeasureMenu_SetValue(&gTft.measure, GetManager());

  if (lv_screen_active() == gTft.chart1.screen)
    UiFftChartMenu_SetValue(&gTft.chart1, GetManager());
}
