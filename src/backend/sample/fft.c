#include <stdalign.h>
#include <dc2.h>
#include <arm_math.h>
#include "internal.h"
#include "sample/sample.h"

// FFT double buffer.
static f32 gFftInput[2][kFftSize * 2] = {0};
// FFT spectrum output buffer.
static f32 gFftOutput[kFftSize] = {0};
// Points to the location in the currently active buffer where data should be written.
static u16 gFftSampleIdx = 0;
// Points to the currently buffer.
static u08 gFftBufferIdx = 0;
// FFT buffer filled flag.
static i08 gFftFillFlag = 0;
// Points to the currently read buffer.
static f32 *gFftBuffer = NULL;

// FFT instance.
static arm_cfft_instance_f32 gFft;

void FftInitialize() {
  arm_cfft_init_f32(&gFft, kFftSize);
  gFftBuffer = NULL;
}

void FftAddSample(
  f32 value
) {
  u16 idx = gFftSampleIdx << 1;
  // Set real part.
  gFftInput[gFftBufferIdx][idx] = value;
  // Reset imaginary part.
  gFftInput[gFftBufferIdx][idx + 1] = 0;

  gFftSampleIdx++;
  if (gFftSampleIdx >= kFftSize) {
    gFftSampleIdx = 0;
    gFftFillFlag = 1;
    gFftBuffer = (f32 *)gFftInput[gFftBufferIdx];
    gFftBufferIdx = gFftBufferIdx == 0 ? 1 : 0;
  }
}

i32 FftUpdate() {
  if (!gFftFillFlag || !gFftBuffer)
    return 0;

  arm_cfft_f32(&gFft, gFftBuffer, 0, 1);
  arm_cmplx_mag_f32(gFftBuffer, gFftOutput, kFftSize);
  gFftBuffer = NULL;
  gFftFillFlag = 0;

  return 1;
}
