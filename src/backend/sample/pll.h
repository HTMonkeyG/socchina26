#ifndef __BACKEND_SAMPLE_PLL_H__
#define __BACKEND_SAMPLE_PLL_H__

#include <dc2.h>
#include <math.h>

// SOGI phase lock loop states.
typedef struct {
  // - PI params.
  f32 Kp;
  f32 Ki;

  // - PLL params.

  // Nominal angular frequency.
  f32 omega0;
  // Sampling period.    
  f32 Ts;
  // SOGI damping factor. Typically taken as sqrt(2).
  f32 k;

  // - Results.

  // Current phase estimate (rad).
  f32 phase;
  // Current frequency estimate (rad/s).
  f32 freq;

  // - Intermediate variables.

  // PI integral accumulation term.
  f32 sum;
  // In-phase component (v').
  f32 x1;
  // Quadrature component (qv'), lagging by 90 degrees.
  f32 x2;
} Pll;

static inline void Pll_Initialize(
  Pll *P
) {
  P->freq = P->omega0;
  P->phase = P->sum = P->x1 = P->x2 = 0.0f;
}

static inline void Pll_Update(
  Pll *P,
  f32 val
) {
  // Use the current estimated frequency as the resonant frequency of the SOGI.
  f32 omega = P->freq;

  // SOGI update.
  // dx1/dt = -k*ω*x1 - ω*x2 + k*ω*val
  // dx2/dt =  ω*x1
  f32 dx1 = -P->k * omega * P->x1 - omega * P->x2 + P->k * omega * val;
  f32 dx2 = omega * P->x1;
  P->x1 += dx1 * P->Ts;
  P->x2 += dx2 * P->Ts;

  // Phase detector (Park Transform).
  // v_alpha = x1  (in phase with input)
  // v_beta  = x2  (lagging by 90°, same amplitude as input)
  // Calculate q-axis error: v_q = -v_alpha * sin(θ) + v_beta * cos(θ)
  f32 e = -P->x1 * sinf(P->phase) + P->x2 * cosf(P->phase);

  // PI modifier.
  P->sum += P->Ki * e;
  f32 dfreq = P->Kp * e + P->sum;

  // Update frequency estimate.
  P->freq = P->omega0 + dfreq;

  // Frequency limit (10Hz).
  const f32 freq_limit = 62.8f;
  if (P->freq > P->omega0 + freq_limit) P->freq = P->omega0 + freq_limit;
  if (P->freq < P->omega0 - freq_limit) P->freq = P->omega0 - freq_limit;

  // Update phase estimate.
  P->phase += P->freq * P->Ts;

  // Phase normalize to [0, 2π).
  while (P->phase >= DC2_PI_F * 2) P->phase -= DC2_PI_F * 2;
  while (P->phase < 0.0f) P->phase += DC2_PI_F * 2;
}

#endif
