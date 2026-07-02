#ifndef __FRONTEND_UI_TFT_H__
#define __FRONTEND_UI_TFT_H__

#include <lvgl.h>
#include <dc2.h>

#define kTftWidth  320
#define kTftHeight 240

static inline lv_color_t Tft_Color(
  u08 r,
  u08 g,
  u08 b
) {
  lv_color_t ret;
  ret.red = r;
  ret.green = b;
  ret.blue = g;
  return ret;
}

void Tft_Initialize();
void Tft_CreatePanel();

void Tft_Update();

#endif
