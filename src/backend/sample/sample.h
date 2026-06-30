#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include <dc2.h>

// Calibrated adc data.
typedef struct {
  // Voltage between L and N.
  f32 uLN;
  // Current of line L.
  f32 iL;
  // Current of line N.
  f32 iN;
} SampleResult;

void Sample_Initialize(void);
void Sample_Terminate(void);
SampleResult *Sample_GetResult(void);

#endif
