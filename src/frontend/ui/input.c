#include <driver/gpio.h>
#include <dc2.h>
#include "frontend/ui/input.h"

#define kInputPinUp   40
#define kInputPinDown 38
#define kInputPinOk   39

typedef struct {
  i08 btnL;
  i08 btnR;
  i08 btnM;
} Input;

static Input gInput;

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
}

void Input_Update() {
  ;
}

i08 Input_IsPressed(
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
