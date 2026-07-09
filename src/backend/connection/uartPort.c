#include <lwrb/lwrb.h>
#include <lwpkt/lwpkt.h>
#include "backend/cubemx.h"
#include "backend/internal.h"
#include "backend/connection/uartPort.h"

static lwpkt_t gPkt;
static lwrb_t gTxRb, gRxRb;
static u08 gTxRbData[kRingBufferLength], gRxRbData[kRingBufferLength];
static u08 gRxData[kRingBufferLength];

void Uart_Initialize() {
  // Init lwpkt.
  lwrb_init(&gTxRb, gTxRbData, kRingBufferLength);
  lwrb_init(&gRxRb, gRxRbData, kRingBufferLength);
  lwpkt_init(&gPkt, &gTxRb, &gRxRb);
  lwpkt_set_addr(&gPkt, 114);

  // Enable UART interrupt.
  HAL_UARTEx_ReceiveToIdle_DMA(&huart3, gRxData, kRingBufferLength);
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

i32 Uart_ReceiveMessage(
  u08 *packetId,
  u08 *data,
  size_t *length
) {
  // Handle receive.
  // ENTER_CRITICAL
  __disable_irq();
  if (lwpkt_read(&gPkt) != lwpktVALID) {
    // LEAVE_CRITICAL
    __enable_irq();
    return 0;
  }

  size_t len = lwpkt_get_data_len(&gPkt);
  *packetId = lwpkt_get_cmd(&gPkt);
  memcpy(data, lwpkt_get_data(&gPkt), len);
  *length = len;

  // LEAVE_CRITICAL
  __enable_irq();
  return 1;
}

void HAL_UARTEx_RxEventCallback(
  UART_HandleTypeDef *hUart,
  u16 size
) {
  if (hUart->Instance != USART3)
    return;

  lwrb_write(&gRxRb, gRxData, size);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart3, gRxData, kRingBufferLength);
}
