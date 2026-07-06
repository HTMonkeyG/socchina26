#include <dc2.h>
#include <esp_lvgl_port.h>
#include <esp_lcd_panel_dev.h>
#include <esp_lcd_ili9341.h>
#include <esp_log.h>
#include "frontend/ui/tft.h"
#include "frontend/ui/menu/menu.h"

void UiOcpMenu_Initialize(
  UiOcpMenu *self
) {
  // Create screen: self->screen.
  self->screen = lv_obj_create(NULL);
  lv_obj_set_user_data(self->screen, self);

  // Style: self->screen [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_bg_color(self->screen, lv_color_hex(0xFF4444), LV_PART_MAIN | LV_STATE_DEFAULT);

  // Create label: self->label1
  self->label1 = lv_label_create(self->screen);
  lv_obj_set_width(self->label1, LV_SIZE_CONTENT);
  lv_obj_set_height(self->label1, LV_SIZE_CONTENT);
  lv_obj_set_align(self->label1, LV_ALIGN_CENTER);
  lv_obj_set_x(self->label1, 0);
  lv_obj_set_y(self->label1, 0);
  lv_label_set_text(self->label1, "OCP Triggered");

  // Style: self->label1 [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_text_color(self->label1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

  // Create label: self->label2
  self->label2 = lv_label_create(self->screen);
  lv_obj_set_width(self->label2, LV_SIZE_CONTENT);
  lv_obj_set_height(self->label2, LV_SIZE_CONTENT);
  lv_obj_set_align(self->label2, LV_ALIGN_CENTER);
  lv_obj_set_x(self->label2, 0);
  lv_obj_set_y(self->label2, 18);
  lv_label_set_text(self->label2, "The system has been shut down.");

  // Style: self->label2 [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_text_color(self->label2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

  // Create line: self->line1
  self->line1 = lv_line_create(self->screen);
  lv_obj_set_width(self->line1, 78);
  lv_obj_set_height(self->line1, 78);
  lv_obj_set_align(self->line1, LV_ALIGN_CENTER);
  lv_obj_set_x(self->line1, 0);
  lv_obj_set_y(self->line1, -51);
  static const lv_point_precise_t p[] = {{15, 15}, {63, 63}};
  lv_line_set_points(self->line1, p, 2);

  // Style: self->line1 [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_line_color(self->line1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_line_width(self->line1, 7, LV_PART_MAIN | LV_STATE_DEFAULT);

  // Create arc: self->arc1
  self->arc1 = lv_arc_create(self->screen);
  lv_obj_set_width(self->arc1, 78);
  lv_obj_set_height(self->arc1, 78);
  lv_obj_set_align(self->arc1, LV_ALIGN_CENTER);
  lv_obj_set_x(self->arc1, 0);
  lv_obj_set_y(self->arc1, -51);
  lv_arc_set_value(self->arc1, 0);
  lv_arc_set_bg_angles(self->arc1, 0, 360);
  lv_obj_add_flag(self->arc1, LV_OBJ_FLAG_CLICKABLE);

  // Style: self->arc1 [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_arc_color(self->arc1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_width(self->arc1, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_rounded(self->arc1, false, LV_PART_MAIN | LV_STATE_DEFAULT);

  // Style: self->arc1 [LV_PART_KNOB|LV_STATE_DEFAULT]
  lv_obj_set_style_bg_opa(self->arc1, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
}

void UiOcpMenu_Show(
  UiOcpMenu *self
) {
  lv_screen_load(self->screen);
}

void UiOcpMenu_Terminate(
  UiOcpMenu *self
) {
  lv_obj_delete(self->arc1);
  lv_obj_delete(self->label2);
  lv_obj_delete(self->label1);
  lv_obj_delete(self->screen);
}
