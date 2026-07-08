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
  // Normalize by the estimated amplitude so that the phase-detector gain is
  // ~1 regardless of the input signal amplitude. Without this the loop gain
  // scales with the grid amplitude (hundreds), driving the frequency into the
  // limit on tiny errors and preventing the loop from ever settling.
  f32 amp = sqrtf(P->x1 * P->x1 + P->x2 * P->x2);
  f32 e = 0.0f;
  if (amp > 1e-3f)
    e = (-P->x1 * sinf(P->phase) + P->x2 * cosf(P->phase)) / amp;

  // Frequency correction limit (10Hz).
  const f32 freq_limit = 62.8f;

  // PI modifier. The integral term is scaled by Ts (discrete integration), so
  // Ki is a proper continuous-domain gain. The integrator is clamped
  // (anti-windup) so it cannot wind up while the output is saturated.
  P->sum += P->Ki * e * P->Ts;
  if (P->sum > freq_limit) P->sum = freq_limit;
  if (P->sum < -freq_limit) P->sum = -freq_limit;
  f32 dfreq = P->Kp * e + P->sum;

  // Frequency limit (10Hz).
  if (dfreq > freq_limit) dfreq = freq_limit;
  if (dfreq < -freq_limit) dfreq = -freq_limit;

  // Update frequency estimate.
  P->freq = P->omega0 + dfreq;

  // Update phase estimate.
  P->phase += P->freq * P->Ts;

  // Phase normalize to [0, 2π).
  while (P->phase >= DC2_PI_F * 2) P->phase -= DC2_PI_F * 2;
  while (P->phase < 0.0f) P->phase += DC2_PI_F * 2;
}

#endif
