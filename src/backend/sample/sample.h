#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include <math.h>
#include <dc2.h>
#include "shared/packets.h"
#include "shared/constants.h"

#define kSampleRate 25600
#define kSampleTickFreq 50
#define kSampleTickInteval (kSampleRate / kSampleTickFreq)

#define VerifyEpsilon(val, epsilon) (fabsf((val)) > (epsilon))

#define UpdatePrescaled(tag, prescaler) \
  static u32 s_prescaler_ ## tag = 0;\
  s_prescaler_ ## tag ++;\
  if (s_prescaler_ ## tag >= prescaler && ((s_prescaler_ ## tag = 0), 1))

// Calibrated adc data.
typedef struct {
  // Voltage between L and N.
  f32 uLN;
  // Current of line L.
  f32 iL;
  // Current of line N.
  f32 iN;
  // RMS value of the above values.
  f32 rmsU;
  f32 rmsI;

  // Power.
  f32 p;
  f32 q;

  // Frequency.
  f32 freq;

  // Waveform
  f32 waveformPointsV[kWaveformPoints];
  f32 waveformPointsI[kWaveformPoints];
  i32 waveformIndex;
} SampleResult;

void Sample_Initialize(void);
void Sample_Terminate(void);

const SampleResult *Sample_GetResult(void);
i08 Sample_TryUpdate(void);

void Sample_SetCalibrate(
  SetCalibrateMode mode,
  f32 u,
  f32 iL,
  f32 iN);

#endif
