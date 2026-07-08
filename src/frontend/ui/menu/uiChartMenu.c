#include <dc2.h>
#include <esp_lvgl_port.h>
#include <esp_lcd_panel_dev.h>
#include <esp_lcd_ili9341.h>
#include <esp_log.h>
#include "shared/packets.h"
#include "frontend/ui/menu/menu.h"

void UiFftChartMenu_Initialize(
  UiFftChartMenu *self
) {
  // Create screen: self->screen.
  self->screen = lv_obj_create(NULL);
  lv_obj_set_user_data(self->screen, self);

  // Style: self->screen [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_bg_color(self->screen, lv_color_hex(0x506060), LV_STATE_DEFAULT);

  // Create chart: self->chart.
  self->chart = lv_chart_create(self->screen);
  lv_chart_set_point_count(self->chart, kFftResultPoints);
  lv_chart_set_type(self->chart, LV_CHART_TYPE_BAR);
  lv_obj_set_size(self->chart, 310, 200);
  lv_obj_align(self->chart, LV_ALIGN_TOP_MID, 0, 0);

  // Style: self->chart [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_bg_color(self->chart, lv_color_hex(0x506060), LV_STATE_DEFAULT);

  // Chart series.
  self->series = lv_chart_add_series(
    self->chart,
    lv_color_hex(0xE4E610),
    LV_CHART_AXIS_PRIMARY_Y);

  lv_chart_set_ext_y_array(self->chart, self->series, self->data);
  lv_chart_set_update_mode(self->chart, LV_CHART_UPDATE_MODE_CIRCULAR);

  // Create container: self->labelContainer.
  self->labelContainer = lv_obj_create(self->screen);
  lv_obj_set_width(self->labelContainer, 310);
  lv_obj_set_height(self->labelContainer, 25);
  lv_obj_set_align(self->labelContainer, LV_ALIGN_BOTTOM_MID);
  lv_obj_set_x(self->labelContainer, 0);
  lv_obj_set_y(self->labelContainer, -5);

  lv_obj_remove_flag(
    self->labelContainer,
    LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_CLICK_FOCUSABLE);

  // Style: self->labelContainer [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_pad_top(self->labelContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(self->labelContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(self->labelContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(self->labelContainer, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(self->labelContainer, lv_color_hex(0xDDBB00), LV_PART_MAIN | LV_STATE_DEFAULT);

  // Create label: self->labelFreq
  self->labelFreq = lv_label_create(self->labelContainer);
  lv_obj_set_width(self->labelFreq, LV_SIZE_CONTENT);
  lv_obj_set_height(self->labelFreq, LV_SIZE_CONTENT);
  lv_obj_set_align(self->labelFreq, LV_ALIGN_LEFT_MID);
  lv_obj_set_x(self->labelFreq, 5);
  lv_obj_set_y(self->labelFreq, 0);
  lv_label_set_text(self->labelFreq, "Freq: 50.0Hz");

  // Style: self->labelFreq [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_text_color(
    self->labelFreq,
    lv_color_hex(0x202020),
    LV_PART_MAIN | LV_STATE_DEFAULT);

  // Create label: self->labelThd
  self->labelThd = lv_label_create(self->labelContainer);
  lv_obj_set_width(self->labelThd, LV_SIZE_CONTENT);
  lv_obj_set_height(self->labelThd, LV_SIZE_CONTENT);
  lv_obj_set_align(self->labelThd, LV_ALIGN_LEFT_MID);
  lv_obj_set_x(self->labelThd, lv_pct(50));
  lv_obj_set_y(self->labelThd, 0);
  lv_label_set_text(self->labelThd, "THD: 0.10%");

  // Style: self->labelThd [LV_PART_MAIN|LV_STATE_DEFAULT]
  lv_obj_set_style_text_color(
    self->labelThd,
    lv_color_hex(0x202020),
    LV_PART_MAIN | LV_STATE_DEFAULT);
}

void UiFftChartMenu_Show(
  UiFftChartMenu *self
) {
  lv_screen_load(self->screen);
}

void UiFftChartMenu_SetResult(
  UiFftChartMenu *self,
  lv_coord_t *val,
  i32 count
) {
  lvgl_port_lock(0);
  for (i32 i = 0; i < kFftResultPoints; i++) {
    lv_coord_t v = i < count ? val[i] : 0;
    lv_chart_set_next_value(self->chart, self->series, v);
  }
  lvgl_port_unlock();
}

void UiFftChartMenu_SetFloatValues(
  UiFftChartMenu *self,
  f32 freq,
  f32 thd
) {
  char buffer[20];
  
  lvgl_port_lock(0);
  snprintf(buffer, sizeof(buffer), "Freq: %04.1f", freq);
  lv_label_set_text(self->labelFreq, buffer);
  snprintf(buffer, sizeof(buffer), "THD: %04.2f%%", thd);
  lv_label_set_text(self->labelThd, buffer);
  lvgl_port_unlock();
}

void UiFftChartMenu_Terminate(
  UiFftChartMenu *self
) {
  lv_chart_remove_series(self->chart, self->series);
  lv_obj_delete(self->chart);
  lv_obj_delete(self->screen);
}
