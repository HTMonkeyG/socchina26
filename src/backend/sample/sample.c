#include <dc2.h>
#include "backend/cubemx.h"
#include "backend/internal.h"
#include "backend/sample/sample.h"
#include "backend/sample/fft.h"
#include "backend/sample/pll.h"
#include "backend/sample/rms.h"

// Raw adc data.
typedef struct {
  // Voltage between L and N.
  u16 uLN;
  // Current of line L.
  u16 iL;
  // Current of line N.
  u16 iN;
} SampleRaw;

static SampleRaw gRawSampleData;
static SampleResult gSampleResult;
static Rms gRmsV, gRmsI;
static i08 gSysTickFlag = 0;

void Sample_Initialize() {
  Fft_Initialize();

  Rms_Initialize(&gRmsI);
  Rms_Initialize(&gRmsV);

  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)&gRawSampleData, 3);
  //__HAL_HRTIM_MASTER_ENABLE_IT(&hhrtim1, HRTIM_MASTER_IT_MREP);
  HAL_HRTIM_WaveformCounterStart(&hhrtim1, HRTIM_TIMERID_MASTER);
}

void Sample_Terminate() {
  Fft_Terminate();
  HAL_HRTIM_WaveformCounterStop(&hhrtim1, HRTIM_TIMERID_MASTER);
}

const SampleResult *Sample_GetResult() {
  return &gSampleResult;
}

i08 Sample_TryUpdate() {
	if (gSysTickFlag) {
		gSysTickFlag = 0;
		return 1;
	}
  return 0;
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

  gSampleResult.uLN = Dc2GetRealVoltage(gRawSampleData.uLN, 11, 0);
  gSampleResult.iL = Dc2GetRealCurrent(gRawSampleData.iL, 100, 0);
  gSampleResult.iN = Dc2GetRealCurrent(gRawSampleData.iN, 100, 0);

  Fft_Update(gSampleResult.uLN);
  UpdatePrescaled(Rms, kRmsInteval) {
    Rms_Update(&gRmsI, gSampleResult.iL);
    Rms_Update(&gRmsV, gSampleResult.uLN);

    // Store the value.
    gSampleResult.rmsI = gRmsI.value;
    gSampleResult.rmsU = gRmsV.value;
  }
  //Dc2SlideWindowAvgUpdate()
  // Pll_Update();

  UpdatePrescaled(SysTick, kSampleTickInteval) {
    gSysTickFlag = 1;
  }
}
