#include <stdlib.h>
#include <dc2.h>
#include "shared/packets.h"
#include "backend/cubemx.h"
#include "backend/sample/sample.h"
#include "backend/sample/fft.h"
#include "backend/connection/uartPort.h"

static void SerializeFft() {
  FftResult result = Fft_TryGetResult();
  if (!result)
    return;
  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
  FftResultMsg msg = {0};

  for (int i = 0; i < kFftResultPoints; i++)
    msg.points[i] = (u16)((*result)[i << 1] / Sample_GetResult()->rmsU / DC2_SQRT2_F * 100.0f);

  Uart_SendMessage(kPacketId_FftResultMsg, (u08 *)&msg, sizeof(FftResultMsg));
}

void SendAllBk() {
  if (!Sample_TryUpdate())
    return;

  SerializeFft();
}
