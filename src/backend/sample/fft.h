#ifndef __FFT_H__
#define __FFT_H__

#include <dc2.h>

#define kFftSize 1024

typedef f32(*FftResult)[kFftSize];

// Initialize FFT instance.
void Fft_Initialize(void);

// Add sample to FFT buffer. Must call in ADC convert complete interrupt.
void Fft_Update(f32 value);

// Reset FFT instance.
void Fft_Terminate(void);

// May cauculates FFT, returns 1 when FFT is completed. Must call in main loop.
FftResult Fft_TryGetResult(void);

#endif
