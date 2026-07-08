#include <stdlib.h>
#include <dc2.h>
#include "shared/packets.h"
#include "backend/cubemx.h"
#include "backend/sample/sample.h"
#include "backend/sample/fft.h"
#include "backend/connection/uartPort.h"

// Send heartbeat when backend is ready.
static void SerializeHeartbeat() {
  HeartbeatMsg msg = {0};
  Uart_SendMessage(kPacketId_HeartbeatMsg, (u08 *)&msg, sizeof(HeartbeatMsg));
}

static void SerializeFft() {
  FftResult result = Fft_TryGetResult();
  if (!result)
    // FFT is not ready, return directly.
    return;

  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
  FftResultMsg msg = {0};

  if (!VerifyEpsilon(Sample_GetResult()->rmsU, kSampleEpsilonV))
    // The voltage is too small, just ignored the value.
    msg.shutdown = 1;
  else {
    for (int i = 0; i < kFftResultPoints; i++)
      // Divide the normalized FFT result by the current maximum value, and map it
      // to the 0~0x7FFF range required by the frontend.
      msg.points[i] = (u16)((*result)[i << 1] / Sample_GetResult()->rmsU / DC2_SQRT2_F * 32767.0f);
  }

  Uart_SendMessage(kPacketId_FftResultMsg, (u08 *)&msg, sizeof(FftResultMsg));
}

static void SerializeMeasure() {
  const SampleResult *result = Sample_GetResult();

  MeasureMsg msg = {0};
  msg.rms.uL = result->rmsU;
  msg.rms.iL = result->rmsI;

  Uart_SendMessage(kPacketId_MeasureMsg, (u08 *)&msg, sizeof(MeasureMsg));
}

void SendAllBk() {
  if (!Sample_TryUpdate())
    return;

  SerializeHeartbeat();
  SerializeFft();
  SerializeMeasure();
}
