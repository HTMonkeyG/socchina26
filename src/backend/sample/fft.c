#include <stdalign.h>
#include <dc2.h>
#include <arm_math.h>
#include "backend/internal.h"
#include "backend/sample/fft.h"
#include "backend/sample/sample.h"

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

void Fft_Initialize() {
  arm_cfft_init_f32(&gFft, kFftSize);
  gFftBuffer = NULL;
}

void Fft_Update(
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

    // ENTER_CRITICAL
    __disable_irq();
    gFftFillFlag = 1;
    gFftBuffer = (f32 *)gFftInput[gFftBufferIdx];
    gFftBufferIdx = gFftBufferIdx == 0 ? 1 : 0;
    // LEAVE_CRITICAL
    __enable_irq();
  }
}

void Fft_Terminate() {
  memset(gFftInput, 0, sizeof(gFftInput));
  memset(gFftOutput, 0, sizeof(gFftOutput));
  gFftSampleIdx = 0;
  gFftBufferIdx = 0;
  gFftFillFlag = 0;
  gFftBuffer = NULL;
}

FftResult Fft_TryGetResult() {
  f32 *buffer = NULL;

  // ENTER_CRITICAL
  __disable_irq();
  if (gFftFillFlag && gFftBuffer) {
    buffer = gFftBuffer;
    gFftFillFlag = 0;
    gFftBuffer = NULL;
  }
  // LEAVE_CRITICAL
  __enable_irq();

  if (!buffer)
    return NULL;

  // Calculate FFT.
  arm_cfft_f32(&gFft, buffer, 0, 1);
  arm_cmplx_mag_f32(buffer, gFftOutput, kFftSize);

  // Normalize.
  const f32 kNormVal = (f32)kFftSize / 2.0f;
  gFftOutput[0] /= (f32)kFftSize;
  for (int i = 1; i < kFftSize; i++)
    gFftOutput[i] /= kNormVal;

  return &gFftOutput;
}
