#ifndef __BACKEND_CONNECTION_UARTPORT_H__
#define __BACKEND_CONNECTION_UARTPORT_H__

#include <stddef.h>
#include <dc2.h>

void Uart_Initialize(void);
void Uart_Update(void);

void Uart_SendMessage(
  u08 packetId,
  const u08 *data,
  size_t length);

#endif
