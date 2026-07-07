#ifndef __FRONTEND_UI_MENU_MENU_H__
#define __FRONTEND_UI_MENU_MENU_H__

#include <lvgl.h>
#include <dc2.h>
#include "shared/packets.h"

#define MenuInitializeOnce(var, init) \
  do {\
    if (!var) {\
      var = (init);\
    }\
  } while(0)

// ----------------------------------------------------------------------------
// [SECTION] UiFftChartMenu
// ----------------------------------------------------------------------------

typedef struct {
  lv_obj_t *screen;
  lv_obj_t *chart;
  lv_chart_series_t *series;
  lv_coord_t data[kFftResultPoints];
} UiFftChartMenu;

// Chart menu.
void UiFftChartMenu_Initialize(
  UiFftChartMenu *self);
void UiFftChartMenu_Show(
  UiFftChartMenu *self);
void UiFftChartMenu_SetResult(
  UiFftChartMenu *self,
  lv_coord_t *val,
  i32 count);
void UiFftChartMenu_Terminate(
  UiFftChartMenu *self);

// ----------------------------------------------------------------------------
// [SECTION] UiOcpMenu
// ----------------------------------------------------------------------------

typedef struct {
  lv_obj_t *screen;
  lv_obj_t *line1;
  lv_obj_t *arc1;
  // "OCP Triggered"
  lv_obj_t *label1;
  // "The system has been shut down."
  lv_obj_t *label2;
} UiOcpMenu;

void UiOcpMenu_Initialize(
  UiOcpMenu *self);
void UiOcpMenu_Show(
  UiOcpMenu *self);
void UiOcpMenu_Terminate(
  UiOcpMenu *self);

#endif
