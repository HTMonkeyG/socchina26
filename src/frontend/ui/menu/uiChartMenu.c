#include <dc2.h>
#include <esp_lvgl_port.h>
#include <esp_lcd_panel_dev.h>
#include <esp_lcd_ili9341.h>
#include <esp_log.h>
#include "frontend/ui/menu/menu.h"

void UiChartMenu_Initialize(
  UiChartMenu *self
) {
  // Screen.
  self->screen = lv_obj_create(NULL);
  lv_obj_set_user_data(self->screen, self);

  // Chart.
  self->chart = lv_chart_create(self->screen);
  lv_obj_t *c = self->chart;

  lv_chart_set_point_count(c, UiChartMenu_kChartDataLen);
  lv_chart_set_type(c, LV_CHART_TYPE_BAR);
  lv_obj_set_size(c, 310, 200);
  lv_obj_align(c, LV_ALIGN_TOP_MID, 0, 0);

  // Chart series.
  self->series = lv_chart_add_series(
    c,
    lv_color_hex(0xFF00FF),
    LV_CHART_AXIS_PRIMARY_Y);

  lv_chart_set_ext_y_array(c, self->series, self->data);
  lv_chart_set_update_mode(c, LV_CHART_UPDATE_MODE_CIRCULAR);
}

void UiChartMenu_Show(
  UiChartMenu *self
) {
  lv_screen_load(self->screen);
}

void UiChartMenu_Update(
  UiChartMenu *self,
  lv_coord_t *val,
  i32 count
) {
  for (i32 i = 0; i < UiChartMenu_kChartDataLen; i++) {
    lv_coord_t v = i < count ? val[i] : 0;
    lv_chart_set_next_value(self->chart, self->series, v);
  }
}

void UiChartMenu_Terminate(
  UiChartMenu *self
) {
  lv_chart_remove_series(self->chart, self->series);
  lv_obj_delete(self->chart);
  lv_obj_delete(self->screen);
  self->chart = NULL;
}
