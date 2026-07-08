#include <dc2.h>
#include <freertos/FreeRTOS.h>
#include "shared/packets.h"
#include "frontend/control/manager.h"

#define kManagerHeartbeatToLoading (2000 / portTICK_PERIOD_MS)

static Manager gManager = {0};

void Manager_Initialize() {
  gManager.isFirstHeartbeat = 1;
}

void Manager_Update() {

}

void Manager_RecvMeasure(
  const MeasureMsg *msg
) {
  gManager.lastFreqValue = msg->freq;
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
