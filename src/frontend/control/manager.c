#include <math.h>
#include <dc2.h>
#include <freertos/FreeRTOS.h>
#include "shared/packets.h"
#include "frontend/ui/input.h"
#include "frontend/control/manager.h"
#include "frontend/connection/uartPort.h"

#define kManagerHeartbeatToLoading (2000 / portTICK_PERIOD_MS)

static Manager gManager = {0};

void Manager_Initialize() {
  gManager.isFirstHeartbeat = 1;
}

void Manager_Update() {
  // Set loading flag.
  if (Manager_IsLoading() && !gManager.isLastLoading)
    gManager.isLastLoading = 1;
  else if (!Manager_IsLoading() && gManager.isLastLoading) {
    // Ensure the relay is open.
    gManager.isLastLoading = 0;
    SetRelayMsg msg;
    gManager.relayState = msg.state = 0;
    Uart_SendMessage(kPacketId_SetRelayMsg, (u08 *)&msg, sizeof(SetRelayMsg));
  }

  if (Input_IsReleased(kInputKey_Middle)) {
    gManager.relayState = !gManager.relayState;
    SetRelayMsg msg;
    msg.state = gManager.relayState;
    Uart_SendMessage(kPacketId_SetRelayMsg, (u08 *)&msg, sizeof(SetRelayMsg));
    printf("relay state: %d\n", gManager.relayState);
  }
}

void Manager_RecvMeasure(
  const MeasureMsg *msg
) {
  gManager.irms = msg->rms.iL;
  gManager.urms = msg->rms.uL;
  gManager.freq = msg->freq;
  gManager.p = msg->p;
  gManager.q = msg->q;
  gManager.s = sqrtf(gManager.p * gManager.p + gManager.q * gManager.q);
  if (gManager.s > 1e-7)
    gManager.pf = gManager.p / gManager.s;
  else
    gManager.pf = 0;
}

void Manager_RecvFft(
  const FftResultMsg *msg
) {
  // Convert fft points.
  for (int i = 0; i < kFftResultPoints; i++)
    gManager.fft[i] = (u32)msg->points[i] * 100 / 32767;

  // Calculate fft.
  f32 thd = 0
    , base = (f32)msg->points[1];
  if (base > 1e-6) {
    for (int i = 2; i < kFftResultPoints; i++)
      thd += (f32)msg->points[i] * (f32)msg->points[i];
    thd = sqrtf(thd) / base * 100.0f;
  }

  gManager.thd = thd;
}

void Manager_RecvHearBeat() {
  gManager.isFirstHeartbeat = 0;
  gManager.tickSinceLastHeartbeat = xTaskGetTickCount();
}

i08 Manager_IsLoading() {
  return gManager.isFirstHeartbeat || 
    xTaskGetTickCount() - gManager.tickSinceLastHeartbeat > kManagerHeartbeatToLoading;
}

Manager *GetManager() {
  return &gManager;
}
