#include <dc2.h>
#include <stddef.h>
#include "shared/packets.h"
#include "shared/binaryStream.h"
#include "backend/sample/sample.h"
#include "backend/control/relay.h"

void HandleMsgBk(
  u08 cmd,
  BinaryStream *stream
) {
  switch (cmd) {
    case kPacketId_SetCalibrateMsg: {
      SetCalibrateMsg msg;
      BinaryStream_SerializeBytes(stream, (u08 *)&msg, sizeof(SetCalibrateMsg));
      if (stream->errorFlag)
        break;
      Sample_SetCalibrate(msg.mode, msg.uLN, msg.iL, msg.iN);
      break;
    }
    case kPacketId_SetRelayMsg: {
      SetRelayMsg msg;
      BinaryStream_SerializeBytes(stream, (u08 *)&msg, sizeof(SetRelayMsg));
      if (stream->errorFlag)
        break;
      Relay_SetStatus(msg.state);
      break;
    }
    default:
      break;
  }
}