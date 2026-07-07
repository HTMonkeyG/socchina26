#ifndef __SHARED_PACKETS_H__
#define __SHARED_PACKETS_H__

#include <dc2.h>

typedef enum {
  kPacketId_OcpMsg = 1,
  kPacketId_MeasureMsg,
  kPacketId_FftResultMsg,
  kPacketId_WaveformMsg,
  kPacketId_HeartbeatMsg,
} PacketId;

typedef struct {
  f32 peakCurrent;
  f32 maxCurrent;
} OcpMsg;

typedef struct {
  f32 maxCurrent;
} SetOcpMsg;

typedef struct {
  f32 uL;
  f32 iL;
  f32 iN;
} MeasureResult;

typedef struct {
  MeasureResult inst;
  MeasureResult rms;
  f32 phaseU;
  f32 phaseI;
  f32 freq;
  i08 valid;
} MeasureMsg;

#define kFftResultPoints 10

typedef struct {
  u16 points[kFftResultPoints];
} FftResultMsg;

#define kWaveformPoints 64

typedef struct {
  f32 peakValue;
  i08 points[kWaveformPoints];
} WaveformMsg;

typedef struct {
  i08 flag;
} HeartbeatMsg;

#endif
