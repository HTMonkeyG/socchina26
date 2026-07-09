#ifndef __BACKEND_CONNECTION_UARTPORT_H__
#define __BACKEND_CONNECTION_UARTPORT_H__

#include <stddef.h>
#include <dc2.h>

#define kRingBufferLength 512

void Uart_Initialize(void);
void Uart_Update(void);

void Uart_SendMessage(
  u08 packetId,
  const u08 *data,
  size_t length);

i32 Uart_ReceiveMessage(
  u08 *packetId,
  u08 *data,
  size_t *length);

#endif
