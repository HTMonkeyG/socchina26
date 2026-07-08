#include <dc2.h>
#include "backend/cubemx.h"
#include "backend/sample/sample.h"
#include "backend/sample/fft.h"
#include "backend/connection/connection.h"
#include "backend/control/relay.h"

static void MX_SystemInit() {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_HRTIM1_Init();
  MX_SPI2_Init();
  MX_ADC2_Init();
  MX_USART3_UART_Init();
}

int main() {
  MX_SystemInit();

  Relay_Initialize();
  Connection_Initialize();
  Sample_Initialize();

  while(1) {
    Connection_Update();
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
  }

  return 0;
}