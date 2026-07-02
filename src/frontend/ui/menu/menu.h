#ifndef __FRONTEND_UI_MENU_MENU_H__
#define __FRONTEND_UI_MENU_MENU_H__

#include <dc2.h>

#define MenuInitializeOnce(var, init) \
  do {\
    if (!var) {\
      var = (init);\
    }\
  } while(0)

// Chart menu.
void UiChartMenu_Initialize();
void UiChartMenu_Update(
  lv_coord_t *val,
  i32 count);
void UiChartMenu_Terminate();

#endif
