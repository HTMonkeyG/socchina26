#include <dc2.h>
#include <stddef.h>
#include <stdio.h>
#include "shared/packets.h"
#include "shared/binaryStream.h"
#include "frontend/control/manager.h"
#include "frontend/ui/tft.h"

void HandleMsgFr(
  u08 cmd,
  BinaryStream *stream
) {
  //printf("msg %d\n", cmd);
  switch (cmd) {
    case kPacketId_MeasureMsg: {
      MeasureMsg msg;
      BinaryStream_SerializeBytes(stream, (u08 *)&msg, sizeof(MeasureMsg));
      Manager_RecvMeasure(&msg);
      break;
    }
    case kPacketId_FftResultMsg: {
      // Handle FFT result.
      FftResultMsg msg;
      BinaryStream_SerializeBytes(stream, (u08 *)&msg, sizeof(FftResultMsg));
      Tft_SetFftResult(&msg);
      break;
    }
    case kPacketId_HeartbeatMsg: {
      Manager_RecvHearBeat();
      break;
    }
    default:
      printf("unknown message %d\n", cmd);
      break;
  }
}
