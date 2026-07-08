#include <dc2.h>
#include "backend/cubemx.h"
#include "backend/internal.h"
#include "backend/sample/sample.h"
#include "backend/sample/fft.h"
#include "backend/sample/pll.h"
#include "backend/sample/rms.h"

#define kPowerAvgPoints 128
#define kPowerAvgFreq 4
// 4Hz window time, 256 points a window.
#define kPowerAvgInteval (kSampleRate / kPowerAvgFreq / kPowerAvgPoints)

// ----------------------------------------------------------------------------
// [SECTION] declarations
// ----------------------------------------------------------------------------

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
static f32 gSampleCalibrate[3] = {0};
static Rms gRmsV, gRmsI;
static Pll gPllV = {
  .Kp = 133.0f,
  .Ki = 8883.0f,
  .omega0 = 50.0f * DC2_PI_F * 2.0f,
  .k = DC2_SQRT2_F,
  .Ts = 1.0f / kSampleRate
};
static i08 gSysTickFlag = 0;
static f32 gAvgScratchP[kPowerAvgPoints]
  , gAvgScratchQ[kPowerAvgPoints];
static Dc2SlideWindowCtx gAvgP, gAvgQ;

// ----------------------------------------------------------------------------
// [SECTION] functions
// ----------------------------------------------------------------------------

void Sample_Initialize() {
  Fft_Initialize();

  Rms_Initialize(&gRmsI);
  Rms_Initialize(&gRmsV);

  Pll_Initialize(&gPllV);

  Dc2SlideWindowAvgInit(&gAvgP, gAvgScratchP, kPowerAvgPoints);
  Dc2SlideWindowAvgInit(&gAvgQ, gAvgScratchQ, kPowerAvgPoints);

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

void Sample_SetCalibrate(
  SetCalibrateMode mode,
  f32 value
) {
  if (mode & kSetCalibrateMode_ULN)
    gSampleCalibrate[0] = value;
  if (mode & kSetCalibrateMode_IL)
    gSampleCalibrate[1] = value;
  if (mode & kSetCalibrateMode_IN)
    gSampleCalibrate[2] = value;
}

// ----------------------------------------------------------------------------
// [SECTION] isr
// ----------------------------------------------------------------------------

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

  gSampleResult.uLN = Dc2GetRealVoltage(gRawSampleData.uLN, 11, gSampleCalibrate[0]);
  gSampleResult.iL = Dc2GetRealCurrent(gRawSampleData.iL, 100, gSampleCalibrate[1]);
  gSampleResult.iN = Dc2GetRealCurrent(gRawSampleData.iN, 100, gSampleCalibrate[2]);

  Fft_Update(gSampleResult.uLN);
  UpdatePrescaled(Rms, kRmsInteval) {
    Rms_Update(&gRmsI, gSampleResult.iL);
    Rms_Update(&gRmsV, gSampleResult.uLN);

    // Store the value.
    gSampleResult.rmsI = gRmsI.value;
    gSampleResult.rmsU = gRmsV.value;
  }

  // Update PLL to get the active power and reactive power.
  Pll_Update(&gPllV, gSampleResult.uLN);
  f32 p = sinf(gPllV.phase - DC2_PI_F / 2) * gSampleResult.iL
    , q = cosf(gPllV.phase - DC2_PI_F / 2) * gSampleResult.iL;

  // Get the frequency in Hz.
  gSampleResult.freq = gPllV.freq / DC2_PI_F / 2;

  // Average of active power and reactive power values.
  UpdatePrescaled(Power, kPowerAvgInteval) {
    gSampleResult.p = Dc2SlideWindowAvgUpdate(&gAvgP, p);
    gSampleResult.q = Dc2SlideWindowAvgUpdate(&gAvgQ, q);
  }

  UpdatePrescaled(SysTick, kSampleTickInteval) {
    gSysTickFlag = 1;
  }
}
