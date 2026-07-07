#include <lwrb/lwrb.h>
#include <lwpkt/lwpkt.h>
#include "backend/cubemx.h"
#include "backend/internal.h"
#include "backend/connection/uartPort.h"

#define kRingBufferLength 512

static lwpkt_t gPkt;
static lwrb_t gTxRb, gRxRb;
static u08 gTxRbData[kRingBufferLength], gRxRbData[kRingBufferLength];

void Uart_Initialize() {
  // Init lwpkt.
  lwrb_init(&gTxRb, gTxRbData, kRingBufferLength);
  lwrb_init(&gRxRb, gRxRbData, kRingBufferLength);
  lwpkt_init(&gPkt, &gTxRb, &gRxRb);
  lwpkt_set_addr(&gPkt, 114);
}

void Uart_Update() {
  u08 buf[kRingBufferLength];

  // Handle send.
  size_t len = lwrb_get_full(&gTxRb);
  if (len) {
    lwrb_read(&gTxRb, buf, len);
    HAL_UART_Transmit(&huart3, buf, len, 100);
  }
}

void Uart_SendMessage(
  u08 packetId,
  const u08 *data,
  size_t length
) {
  lwpkt_write(&gPkt, 191, packetId, data, length);
}
