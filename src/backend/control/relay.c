#include "backend/cubemx.h"
#include "backend/control/relay.h"

void Relay_Initialize() {
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
}

void Relay_SetStatus(
  i08 status
) {
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, !!status);
}
