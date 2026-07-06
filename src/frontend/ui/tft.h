#ifndef __FRONTEND_UI_TFT_H__
#define __FRONTEND_UI_TFT_H__

#include <lvgl.h>
#include <dc2.h>

#define kTftWidth  320
#define kTftHeight 240

void Tft_Initialize();
void Tft_CreatePanel();

void Tft_Update();

#endif
