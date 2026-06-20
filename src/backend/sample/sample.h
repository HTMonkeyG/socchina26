#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include <dc2.h>

#define kFftSize 1024

// Calibrated adc data.
typedef struct {
  // Voltage between L and N.
  f32 uLN;
  // Current of line L.
  f32 iL;
  // Current of line N.
  f32 iN;
} SampleResult;

void SampleInitialize(void);
void SampleTerminate(void);

// Initialize FFT instance.
void FftInitialize(void);
// Add sample to FFT buffer. Must call in ADC convert complete interrupt.
void FftAddSample(f32 value);
// May cauculates FFT, returns 1 when FFT is completed. Must call in main loop.
i32 FftUpdate(void);

#endif
