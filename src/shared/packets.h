#ifndef __SHARED_PACKETS_H__
#define __SHARED_PACKETS_H__

#include <dc2.h>

typedef enum {
  kPacketId_OcpMsg = 1,
  kPacketId_MeasureMsg,
  kPacketId_FftResultMsg,
  kPacketId_WaveformMsg,
  kPacketId_HeartbeatMsg,
  kPacketId_SetCalibrateMsg,
} PacketId;

// - Backend to frontend.

// kPacketId_OcpMsg = 1
typedef struct {
  f32 peakCurrent;
  f32 maxCurrent;
} OcpMsg;

typedef struct {
  f32 uL;
  f32 iL;
  f32 iN;
} MeasureResult;

// kPacketId_MeasureMsg = 2
typedef struct {
  MeasureResult inst;
  MeasureResult rms;
  f32 p;
  f32 q;
  f32 freq;
  i08 valid;
} MeasureMsg;

#define kFftResultPoints 10

// kPacketId_FftResultMsg = 3
typedef struct {
  u16 points[kFftResultPoints];
  i08 shutdown;
} FftResultMsg;

#define kWaveformPoints 64

// kPacketId_WaveformMsg = 4
typedef struct {
  f32 peakValue;
  i08 points[kWaveformPoints];
} WaveformMsg;

// - Frontend to backend.

typedef struct {
  f32 maxCurrent;
} SetOcpMsg;

typedef enum {
  kSetCalibrateMode_None = 0,
  kSetCalibrateMode_IL = 1 << 0,
  kSetCalibrateMode_IN = 1 << 1,
  kSetCalibrateMode_ULN = 1 << 2,
} SetCalibrateMode;

// kPacketId_SetCalibrateMsg = 6
typedef struct {
  u08 mode;
  f32 iL;
  f32 iN;
  f32 uLN;
} SetCalibrateMsg;

// - Bi-directional.

// kPacketId_HeartbeatMsg = 5
typedef struct {
  i08 flag;
} HeartbeatMsg;

#endif
