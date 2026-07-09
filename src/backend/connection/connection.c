#include <dc2.h>
#include "shared/binaryStream.h"
#include "backend/connection/connection.h"
#include "backend/connection/uartPort.h"

void Connection_Initialize() {
  Uart_Initialize();
}

extern void SendAllBk();
extern void HandleMsgBk(
  u08 cmd,
  BinaryStream *stream);

void Connection_Update() {
  u08 packetId;
  size_t length;
  u08 buf[kRingBufferLength];
  BinaryStream stream;
  u08 batch = 0;

	SendAllBk();
	Uart_Update();
	
  while (Uart_ReceiveMessage(&packetId, buf, &length)) {
    BinaryStream_Construct(&stream, buf, length, kBinaryStreamMode_Read);
    HandleMsgBk(packetId, &stream);

    batch++;
    if (batch >= kConnectionMaxPacketBatch) {
      break;
    }
  }
}
