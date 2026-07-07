#include <dc2.h>
#include "backend/connection/connection.h"
#include "backend/connection/uartPort.h"

void Connection_Initialize() {
  Uart_Initialize();
}

extern void SendAllBk();

void Connection_Update() {
  SendAllBk();
  Uart_Update();
}
