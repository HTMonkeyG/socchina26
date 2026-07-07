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
  for (i32 i = 0; i < kFftResultPoints; i++) {
    lv_coord_t v = i < count ? val[i] : 0;
    lv_chart_set_next_value(self->chart, self->series, v);
  }
}

void UiFftChartMenu_Terminate(
  UiFftChartMenu *self
) {
  lv_chart_remove_series(self->chart, self->series);
  lv_obj_delete(self->chart);
  lv_obj_delete(self->screen);
}
