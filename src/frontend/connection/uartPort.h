#ifndef __FRONTEND_CONNECTION_UARTPORT_H__
#define __FRONTEND_CONNECTION_UARTPORT_H__

#include <dc2.h>

#define kUartBufSize 512

// Intialize UART1 driver.
void Uart_Initialize();

// Handle hardware send and receive.
void Uart_Update();

// Uninstall UART1 driver.
void Uart_Terminate();

// Send message through UART1.
void Uart_SendMessage(
  u08 packetId,
  const u08 *data,
  size_t length);

// Receive a packet from UART1. Returns true if successfully got a packet.
i08 Uart_ReceiveMessage(
  u08 *packetId,
  u08 *data,
  size_t *length);

#endif
