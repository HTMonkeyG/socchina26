#include <dc2.h>
#include <stddef.h>
#include <stdio.h>
#include "shared/packets.h"
#include "shared/binaryStream.h"
#include "frontend/ui/tft.h"

void HandleMsgFr(
  u08 cmd,
  BinaryStream *stream
) {
  //printf("Msg %d\n", cmd);
  switch (cmd) {
    case kPacketId_FftResultMsg: {
      // Handle FFT result.
      FftResultMsg msg;
      BinaryStream_SerializeBytes(stream, (u08 *)&msg, sizeof(FftResultMsg));
      Tft_SetFftResult(&msg);
      break;
    }
    default:
      printf("Unknown message %d", cmd);
      break;
  }
}
