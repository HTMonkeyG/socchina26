#include "cubemx.h"

static void MX_SystemInit() {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_ADC2_Init();
  MX_USART3_UART_Init();
}

int main() {
  MX_SystemInit();

  while(1) {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
  }

  return 0;
}