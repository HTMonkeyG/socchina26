#include <driver/gpio.h>
#include <esp_timer.h>
#include <dc2.h>
#include "frontend/ui/input.h"

#define kInputPinUp   40
#define kInputPinDown 38
#define kInputPinOk   39

typedef struct {
  esp_timer_handle_t hTimer;
  Dc2ButtonState btnL;
  Dc2ButtonState btnR;
  Dc2ButtonState btnM;
} Input;

static Input gInput;

void s_InputHandler(
  void *arg
) {
  Dc2ButtonUpdate(&gInput.btnL, !gpio_get_level(kInputPinDown));
  Dc2ButtonUpdate(&gInput.btnR, !gpio_get_level(kInputPinUp));
  Dc2ButtonUpdate(&gInput.btnM, !gpio_get_level(kInputPinOk));
}

void Input_Initialize() {
  gpio_config_t cfg = {
    .pin_bit_mask = (1ULL << kInputPinUp)
      | (1ULL << kInputPinDown)
      | (1ULL << kInputPinOk),
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
  };
  gpio_config(&cfg);

  esp_timer_create_args_t timer_args = {
    .callback = s_InputHandler,
    .name = "s_InputHandler"
  };
  esp_timer_create(&timer_args, &gInput.hTimer);
  esp_timer_start_periodic(gInput.hTimer, 5000);
}

void Input_Update() {
  ;
}

i08 Input_IsDown(
  InputKey key
) {
  if (key == kInputKey_Left)
    return !gpio_get_level(kInputPinDown);
  if (key == kInputKey_Right)
    return !gpio_get_level(kInputPinUp);
  if (key == kInputKey_Middle)
    return !gpio_get_level(kInputPinOk);
  return 0;
}

i08 Input_IsPressed(
  InputKey key
) {
  if (key == kInputKey_Left && (gInput.btnL.eventFlag & Dc2ButtonEvent_Pressed)) {
    gInput.btnL.eventFlag &= ~Dc2ButtonEvent_Pressed;
    return 1;
  }
  if (key == kInputKey_Right && (gInput.btnR.eventFlag & Dc2ButtonEvent_Pressed)) {
    gInput.btnR.eventFlag &= ~Dc2ButtonEvent_Pressed;
    return 1;
  }
  if (key == kInputKey_Middle && (gInput.btnM.eventFlag & Dc2ButtonEvent_Pressed)) {
    gInput.btnM.eventFlag &= ~Dc2ButtonEvent_Pressed;
    return 1;
  }
  return 0;
}

i08 Input_IsReleased(
  InputKey key
) {
  if (key == kInputKey_Left && (gInput.btnL.eventFlag & Dc2ButtonEvent_Released)) {
    gInput.btnL.eventFlag &= ~Dc2ButtonEvent_Released;
    return 1;
  }
  if (key == kInputKey_Right && (gInput.btnR.eventFlag & Dc2ButtonEvent_Released)) {
    gInput.btnR.eventFlag &= ~Dc2ButtonEvent_Released;
    return 1;
  }
  if (key == kInputKey_Middle && (gInput.btnM.eventFlag & Dc2ButtonEvent_Released)) {
    gInput.btnM.eventFlag &= ~Dc2ButtonEvent_Released;
    return 1;
  }
  return 0;
}

void Input_Terminate() {
  esp_timer_stop(gInput.hTimer);
}
