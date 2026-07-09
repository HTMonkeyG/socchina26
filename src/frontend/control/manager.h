#ifndef __FRONTEND_CONTROL_MANAGER_H__
#define __FRONTEND_CONTROL_MANAGER_H__

#include <dc2.h>
#include "shared/packets.h"

typedef struct {
  // - Backend states.
  i08 isLastLoading;
  i08 isFirstHeartbeat;
  i08 relayState;
  u32 tickSinceLastHeartbeat;

  // - Measured values.
  f32 urms;
  f32 irms;
  f32 freq;
  f32 p;
  f32 q;
  f32 s;
  f32 pf;
  f32 thd;

  // - Fft values.
  i32 fft[kFftResultPoints];
} Manager;

void Manager_Initialize();
void Manager_Update();

void Manager_RecvHearBeat();
void Manager_RecvFft(
  const FftResultMsg *msg);
void Manager_RecvMeasure(
  const MeasureMsg *msg);
i08 Manager_IsLoading();

Manager *GetManager();

#endif
