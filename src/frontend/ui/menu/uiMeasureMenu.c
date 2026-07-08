#include <dc2.h>
#include <esp_lvgl_port.h>
#include <esp_lcd_panel_dev.h>
#include <esp_lcd_ili9341.h>
#include <esp_log.h>
#include "frontend/control/manager.h"
#include "frontend/ui/tft.h"
#include "frontend/ui/menu/menu.h"

extern lv_font_t consolas_40px;

void UiMeasureMenu_Initialize(
  UiMeasureMenu *self
) {
  self->screen = lv_obj_create(NULL);
  lv_obj_set_user_data(self->screen, self);

  // Style: self->screen [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_bg_color(self->screen, lv_color_hex(0x506060), LV_PART_MAIN | LV_STATE_DEFAULT);

  // Create label: self->labelV
  self->labelV = lv_label_create(self->screen);
  lv_obj_set_width(self->labelV, LV_SIZE_CONTENT);
  lv_obj_set_height(self->labelV, LV_SIZE_CONTENT);
  lv_obj_set_align(self->labelV, LV_ALIGN_TOP_LEFT);
  lv_obj_set_x(self->labelV, 5);
  lv_obj_set_y(self->labelV, 0);
  lv_label_set_text(self->labelV, "000.0V");

  // Style: self->labelV [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_text_color(self->labelV, lv_color_hex(0xFFFF00), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(self->labelV, &consolas_40px, LV_PART_MAIN | LV_STATE_DEFAULT);
  
  // Create label: self->labelI
  self->labelI = lv_label_create(self->screen);
  lv_obj_set_width(self->labelI, LV_SIZE_CONTENT);
  lv_obj_set_height(self->labelI, LV_SIZE_CONTENT);
  lv_obj_set_align(self->labelI, LV_ALIGN_TOP_LEFT);
  lv_obj_set_x(self->labelI, 5);
  lv_obj_set_y(self->labelI, 40);
  lv_label_set_text(self->labelI, "00.00A");

  // Style: self->labelI [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_text_color(self->labelI, lv_color_hex(0xFF00FF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(self->labelI, &consolas_40px, LV_PART_MAIN | LV_STATE_DEFAULT);

  // Create label: self->labelFreq
  self->labelFreq = lv_label_create(self->screen);
  lv_obj_set_width(self->labelFreq, LV_SIZE_CONTENT);
  lv_obj_set_height(self->labelFreq, LV_SIZE_CONTENT);
  lv_obj_set_align(self->labelFreq, LV_ALIGN_TOP_LEFT);
  lv_obj_set_x(self->labelFreq, 5);
  lv_obj_set_y(self->labelFreq, 80);
  lv_label_set_text(self->labelFreq, "000.0Hz");

  // Style: self->labelFreq [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_text_font(self->labelFreq, &consolas_40px, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(self->labelFreq, lv_color_hex(0x2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);

  // Create label: self->labelPF
  self->labelPF = lv_label_create(self->screen);
  lv_obj_set_width(self->labelPF, LV_SIZE_CONTENT);
  lv_obj_set_height(self->labelPF, LV_SIZE_CONTENT);
  lv_obj_set_align(self->labelPF, LV_ALIGN_TOP_LEFT);
  lv_obj_set_x(self->labelPF, 5);
  lv_obj_set_y(self->labelPF, 174);
  lv_label_set_text(self->labelPF, "PF 1.00");

  // Style: self->labelPF [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_text_color(self->labelPF, lv_color_hex(0xEEAA44), LV_PART_MAIN | LV_STATE_DEFAULT);

  // Create label: self->labelP
  self->labelP = lv_label_create(self->screen);
  lv_obj_set_width(self->labelP, LV_SIZE_CONTENT);
  lv_obj_set_height(self->labelP, LV_SIZE_CONTENT);
  lv_obj_set_align(self->labelP, LV_ALIGN_TOP_LEFT);
  lv_obj_set_x(self->labelP, 5);
  lv_obj_set_y(self->labelP, 120);
  lv_label_set_text(self->labelP, "000.0W");

  // Style: self->labelP [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_text_color(self->labelP, lv_color_hex(0x00FF00), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(self->labelP, &consolas_40px, LV_PART_MAIN | LV_STATE_DEFAULT);

  // Create label: self->labelQ
  self->labelQ = lv_label_create(self->screen);
  lv_obj_set_width(self->labelQ, LV_SIZE_CONTENT);
  lv_obj_set_height(self->labelQ, LV_SIZE_CONTENT);
  lv_obj_set_align(self->labelQ, LV_ALIGN_TOP_LEFT);
  lv_obj_set_x(self->labelQ, 5);
  lv_obj_set_y(self->labelQ, 160);
  lv_label_set_text(self->labelQ, "0.0Var");

  // Style: self->labelQ [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_text_color(self->labelQ, lv_color_hex(0xEEAA44), LV_PART_MAIN | LV_STATE_DEFAULT);

  // Create label: self->labelTHD
  self->labelTHD = lv_label_create(self->screen);
  lv_obj_set_width(self->labelTHD, LV_SIZE_CONTENT);
  lv_obj_set_height(self->labelTHD, LV_SIZE_CONTENT);
  lv_obj_set_align(self->labelTHD, LV_ALIGN_TOP_LEFT);
  lv_obj_set_x(self->labelTHD, 5);
  lv_obj_set_y(self->labelTHD, 188);
  lv_label_set_text(self->labelTHD, "THD 00.0%");

  // Style: self->labelTHD [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_text_color(self->labelTHD, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
}

void UiMeasureMenu_Show(
  UiMeasureMenu *self
) {
  lvgl_port_lock(0);
  lv_screen_load(self->screen);
  lvgl_port_unlock();
}

void UiMeasureMenu_SetValue(
  UiMeasureMenu *self,
  Manager *manager
) {
  char buf[16];
  lvgl_port_lock(0);
  snprintf(buf, sizeof(buf), "%05.1fV", manager->urms); lv_label_set_text(self->labelV, buf);
  snprintf(buf, sizeof(buf), "%05.2fA", manager->irms); lv_label_set_text(self->labelI, buf);
  snprintf(buf, sizeof(buf), "%05.1fHz", manager->freq); lv_label_set_text(self->labelFreq, buf);
  snprintf(buf, sizeof(buf), "%05.1fW", manager->p); lv_label_set_text(self->labelP, buf);
  snprintf(buf, sizeof(buf), "%03.1fVar", manager->q); lv_label_set_text(self->labelQ, buf);
  snprintf(buf, sizeof(buf), "PF %04.2f", manager->pf); lv_label_set_text(self->labelPF, buf);
  snprintf(buf, sizeof(buf), "THD %04.1f%%", manager->thd); lv_label_set_text(self->labelTHD, buf);
  lvgl_port_unlock();
}

void UiMeasureMenu_Terminate(
  UiMeasureMenu *self
) {
  lv_obj_delete(self->labelV);
  lv_obj_delete(self->labelI);
  lv_obj_delete(self->labelP);
  lv_obj_delete(self->labelQ);
  lv_obj_delete(self->labelPF);
  lv_obj_delete(self->labelFreq);
  lv_obj_delete(self->labelTHD);
}
