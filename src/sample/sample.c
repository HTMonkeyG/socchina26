#include <dc2.h>
#include "cubemx.h"
#include "sample/sample.h"

// [0]: 
// [1]: 
// [2]: 
static u16 gAdcRawData[3];

void SampleInitialize() {
  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)gAdcRawData, 3);
  __HAL_HRTIM_MASTER_ENABLE_IT(&hhrtim1, HRTIM_MASTER_IT_MREP);
  HAL_HRTIM_WaveformCounterStart(&hhrtim1, HRTIM_TIMERID_MASTER);
}

void HAL_HRTIM_RepetitionEventCallback(
  HRTIM_HandleTypeDef *hhrtim,
  uint32_t TimerIdx
) {
  if (TimerIdx != HRTIM_TIMERINDEX_MASTER)
    return;
}

void HAL_ADC_ConvCpltCallback(
  ADC_HandleTypeDef *hAdc
) {
  if (hAdc->Instance != ADC2)
    return;
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
}
