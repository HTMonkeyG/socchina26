#ifndef __FRONTEND_CONTROL_MANAGER_H__
#define __FRONTEND_CONTROL_MANAGER_H__

#include <dc2.h>

typedef struct {
  i08 isFirstHeartbeat;
  u32 tickSinceLastHeartbeat;
  f32 lastFreqValue;
} Manager;

void Manager_Initialize();
void Manager_Update();

void Manager_RecvHearBeat();
void Manager_RecvMeasure(
  const MeasureMsg *msg);
i08 Manager_IsLoading();

Manager *GetManager();

#endif
