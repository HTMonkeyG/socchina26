#ifndef __FRONTEND_UI_TFT_H__
#define __FRONTEND_UI_TFT_H__

#include <dc2.h>
#include "shared/packets.h"

#define kTftWidth  320
#define kTftHeight 240

void Tft_Initialize();
void Tft_CreatePanel();

void Tft_Update();

void Tft_SetFftResult(
  FftResultMsg *msg);

#endif
