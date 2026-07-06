#include <driver/gpio.h>
#include <esp_lvgl_port.h>
#include <esp_lcd_panel_dev.h>
#include <esp_lcd_ili9341.h>
#include <esp_log.h>
#include "frontend/ui/tft.h"
#include "frontend/ui/menu/menu.h"

#define PIN_NUM_MOSI 13
#define PIN_NUM_CLK  12
#define PIN_NUM_CS   10
#define PIN_NUM_DC   19
#define PIN_NUM_RST  20
#define PIN_NUM_BCKL 8

#define kTftSpiSpeed (40 * 1000 * 1000) // 40MHz

typedef struct {
  lv_obj_t *btn;
  lv_style_t style;
} TftButton;

typedef struct {
  UiChartMenu chart1;
  UiOcpMenu ocp;
  TftButton btn1;
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
    .mosi_io_num = PIN_NUM_MOSI,
    .miso_io_num = -1,
    .sclk_io_num = PIN_NUM_CLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = kTftWidth * kTftHeight * sizeof(uint16_t),
  };
  ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO));

  esp_lcd_panel_io_handle_t io_handle = NULL;
  esp_lcd_panel_io_spi_config_t io_config = {
    .dc_gpio_num = PIN_NUM_DC,
    .cs_gpio_num = PIN_NUM_CS,
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
    .pin_bit_mask = 1ULL << PIN_NUM_BCKL,
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
  };
  gpio_config(&bk_gpio_config);
  gpio_set_level(PIN_NUM_BCKL, 1);

  printf("LVGL and ST7735 initialization done.\n");
}

void Tft_CreatePanel() {
  lvgl_port_lock(0);
  /*lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, "Hello ST7735!");
  lv_obj_set_style_text_color(label, lv_color_hex(0xFF0000), LV_STATE_DEFAULT);
  lv_obj_center(label);*/

  /*lv_obj_t *menu = lv_menu_create(lv_scr_act());
  lv_obj_set_size(menu, lv_pct(80), lv_pct(80));
  lv_obj_center(menu);

  lv_menu_set_mode_header(menu, LV_MENU_HEADER_TOP_FIXED);
  lv_menu_set_mode_root_back_button(menu, LV_MENU_ROOT_BACK_BUTTON_ENABLED);

  lv_obj_t *main_page = lv_menu_page_create(menu, "Main");
  lv_obj_t *main_list = lv_list_create(main_page);
  lv_obj_set_size(main_list, lv_pct(100), lv_pct(100));

  lv_obj_t *sub_page = lv_menu_page_create(menu, "Sub");
  lv_obj_t *sub_list = lv_list_create(sub_page);
  lv_obj_set_size(sub_list, lv_pct(100), lv_pct(100));

  lv_obj_t *btn = lv_list_add_button(main_list, LV_SYMBOL_FILE, "Enter");

  lv_menu_set_page(menu, main_page);*/

  /*lv_style_t *pStyle = &gTft.btn1.style;
  lv_style_init(pStyle);

  lv_style_set_bg_color(pStyle, lv_palette_main(LV_PALETTE_BLUE));
  lv_style_set_bg_opa(pStyle, LV_OPA_COVER);
  lv_style_set_bg_grad_color(pStyle, lv_palette_lighten(LV_PALETTE_BLUE, 2));
  lv_style_set_bg_grad_dir(pStyle, LV_GRAD_DIR_VER);

  lv_obj_t *pBtn = gTft.btn1.btn = lv_btn_create(lv_scr_act());
  lv_obj_add_style(pBtn, pStyle, LV_STATE_DEFAULT);
  lv_obj_set_size(pBtn, lv_pct(20), lv_pct(20));
  lv_obj_set_pos(pBtn, lv_pct(20), lv_pct(20));*/
  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x15252f), LV_STATE_DEFAULT);
  UiChartMenu_Initialize(&gTft.chart1);
  UiOcpMenu_Initialize(&gTft.ocp);
  UiOcpMenu_Show(&gTft.ocp);

  lvgl_port_unlock();
}

void Tft_Update() {
  /*static i32 count = 0;

  lv_obj_set_pos(gTft.btn1.btn, lv_pct(count), lv_pct(20));
  count++;
  count %= 50;*/

  lv_coord_t arr[10];
  for (int i = 0; i < 10; i++)
    arr[i] = rand() % 100;
  UiChartMenu_Update(&gTft.chart1, arr, 10);
}
