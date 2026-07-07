#include <stddef.h>
#include <dc2.h>
#include "shared/binaryStream.h"
#include "frontend/connection/connection.h"
#include "frontend/connection/uartPort.h"

void Connection_Initialize() {
  Uart_Initialize();
}

//extern void SendAllFr();
extern void HandleMsgFr(
  u08 cmd,
  BinaryStream *stream);

void Connection_Update() {
  u08 packetId;
  size_t length;
  u08 buf[kUartBufSize];
  BinaryStream stream;

  while (Uart_ReceiveMessage(&packetId, buf, &length)) {
    BinaryStream_Construct(&stream, buf, length, kBinaryStreamMode_Read);
    HandleMsgFr(packetId, &stream);
  }
  
  //SendAllFr();
  Uart_Update();
}
