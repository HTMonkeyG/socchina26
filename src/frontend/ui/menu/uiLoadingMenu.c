#include <dc2.h>
#include <esp_lvgl_port.h>
#include <esp_lcd_panel_dev.h>
#include <esp_lcd_ili9341.h>
#include <esp_log.h>
#include "frontend/ui/tft.h"
#include "frontend/ui/menu/menu.h"

void UiLoadingMenu_Initialize(
  UiLoadingMenu *self
) {
  // Create screen: self->screen.
  self->screen = lv_obj_create(NULL);
  lv_obj_set_user_data(self->screen, self);

  // Style: self->screen [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_bg_color(self->screen, lv_color_hex(0x506060), LV_STATE_DEFAULT);

  // Create spinner: self->spinner
  self->spinner = lv_spinner_create(self->screen);
  lv_obj_set_width(self->spinner, 100);
  lv_obj_set_height(self->spinner, 100);
  lv_obj_set_align(self->spinner, LV_ALIGN_TOP_LEFT);
  lv_obj_set_x(self->spinner, 110);
  lv_obj_set_y(self->spinner, 70);
  lv_spinner_set_anim_params(self->spinner, 1000, 180);

  // Style: self->spinner [LV_PART_INDICATOR|LV_STATE_DEFAULT]
  lv_obj_set_style_arc_color(self->spinner, lv_color_hex(0x999999), LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // Create label: self->label
  self->label = lv_label_create(self->screen);
  lv_obj_set_width(self->label, LV_SIZE_CONTENT);
  lv_obj_set_height(self->label, LV_SIZE_CONTENT);
  lv_obj_set_align(self->label, LV_ALIGN_BOTTOM_MID);
  lv_obj_set_x(self->label, 0);
  lv_obj_set_y(self->label, -20);
  lv_label_set_text(self->label, "Setting up...");

  // Style: self->label [LV_PART_MAIN|LV_STATE_DEFAULT]
  //lv_obj_set_style_text_font(self->label, &lv_font_montserratMedium_20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(self->label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
}

void UiLoadingMenu_Show(
  UiLoadingMenu *self
) {
  lvgl_port_lock(0);
  lv_screen_load(self->screen);
  lvgl_port_unlock();
}

void UiLoadingMenu_Terminate(
  UiLoadingMenu *self
) {
  lv_obj_delete(self->label);
  lv_obj_delete(self->spinner);
  lv_obj_delete(self->screen);
}
