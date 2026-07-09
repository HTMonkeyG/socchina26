#ifndef __FRONTEND_UI_INPUT_H__
#define __FRONTEND_UI_INPUT_H__

typedef enum {
  kInputKey_Left = 1,
  kInputKey_Right = 2,
  kInputKey_Middle = 3,
  kInputKey_Up = kInputKey_Right,
  kInputKey_Down = kInputKey_Left,
  kInputKey_Ok = kInputKey_Middle,
} InputKey;

void Input_Initialize();
void Input_Update();

i08 Input_IsPressed(
  InputKey key);

#endif
