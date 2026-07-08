#ifndef __BACKEND_SAMPLE_RMS_H__
#define __BACKEND_SAMPLE_RMS_H__

#include <dc2.h>
#include <math.h>
#include "backend/sample/sample.h"

#define kRmsPoints 128
#define kRmsFreq 4
#define kRmsInteval (kSampleRate / kRmsFreq / kRmsPoints)

typedef struct {
  Dc2SlideWindowCtx ctx;
  f32 scratch[kRmsPoints];
  f32 value;
} Rms;

static inline void Rms_Initialize(
  Rms *P
) {
  Dc2SlideWindowAvgInit(&P->ctx, P->scratch, kRmsPoints);
}

static inline void Rms_Update(
  Rms *P,
  f32 val
) {
  P->value = sqrtf(Dc2SlideWindowAvgUpdate(&P->ctx, val * val));
}

#endif
