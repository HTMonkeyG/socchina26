#include <dc2.h>
#include <esp_lvgl_port.h>
#include <esp_lcd_panel_dev.h>
#include <esp_lcd_ili9341.h>
#include <esp_log.h>
#include "frontend/ui/menu/menu.h"

#define kChartDataLen 10

typedef struct {
  lv_obj_t *chart;
  lv_chart_series_t *series;
  lv_coord_t data[kChartDataLen];
} UiChartMenu;

static UiChartMenu gChart = {0};

void UiChartMenu_Initialize() {
  MenuInitializeOnce(gChart.chart, lv_chart_create(lv_scr_act()));
  lv_obj_t *chart = gChart.chart;

  lv_chart_set_point_count(chart, kChartDataLen);
  lv_chart_set_type(chart, LV_CHART_TYPE_BAR);
  lv_obj_set_size(chart, 310, 200);
  lv_obj_align(chart, LV_ALIGN_TOP_MID, 0, 0);

  gChart.series = lv_chart_add_series(
    chart,
    lv_color_hex(0xFF00FF),
    LV_CHART_AXIS_PRIMARY_Y);

  lv_chart_set_ext_y_array(chart, gChart.series, gChart.data);
  lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_CIRCULAR);
}

void UiChartMenu_Update(
  lv_coord_t *val,
  i32 count
) {
  for (i32 i = 0; i < kChartDataLen; i++) {
    lv_coord_t v = i < count ? val[i] : 0;
    lv_chart_set_next_value(gChart.chart, gChart.series, v);
    //gChart.data[i] = v;
  }
}

void UiChartMenu_Terminate() {
  lv_chart_remove_series(gChart.chart, gChart.series);
  lv_obj_delete(gChart.chart);
}
