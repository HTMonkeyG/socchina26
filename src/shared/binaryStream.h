#ifndef __SHARED_BINARYSTREAM_H__
#define __SHARED_BINARYSTREAM_H__

#include <stddef.h>
#include <dc2.h>

typedef enum {
  kBinaryStreamMode_Read = 0,
  kBinaryStreamMode_Write = 1,
} BinaryStreamMode;

typedef struct {
  BinaryStreamMode mode;
  i08 errorFlag;
  size_t maxSize;
  u08 *pBuffer;
  u08 *cursor;
} BinaryStream;

void BinaryStream_Construct(
  BinaryStream *S,
  u08 *pBuffer,
  size_t maxSize,
  BinaryStreamMode mode);

void BinaryStream_Reset(
  BinaryStream *S);

void BinaryStream_Seek(
  BinaryStream *S,
  size_t location);

void BinaryStream_Skip(
  BinaryStream *S,
  size_t count);

size_t BinaryStream_GetNumBytesUsed(
  BinaryStream *S);

void BinaryStream_SerializeBytes(
  BinaryStream *S,
  u08 *data,
  size_t size);

// We need the compiler to perform type checking on pointers, so we do not
// use macro declarations. 
#define BinaryStream_SerializeProto(_Name, _Type) \
static inline void _Name(\
  BinaryStream *S,\
  _Type *data\
) {\
  BinaryStream_SerializeBytes(S, (u08 *)data, sizeof(_Type));\
}

BinaryStream_SerializeProto(BinaryStream_SerializeInt8, i08)
BinaryStream_SerializeProto(BinaryStream_SerializeUint8, u08)
BinaryStream_SerializeProto(BinaryStream_SerializeInt16, i16)
BinaryStream_SerializeProto(BinaryStream_SerializeUint16, u16)
BinaryStream_SerializeProto(BinaryStream_SerializeInt32, i32)
BinaryStream_SerializeProto(BinaryStream_SerializeUint32, u32)
BinaryStream_SerializeProto(BinaryStream_SerializeInt64, i64)
BinaryStream_SerializeProto(BinaryStream_SerializeUint64, u64)
BinaryStream_SerializeProto(BinaryStream_SerializeFloat32, f32)
BinaryStream_SerializeProto(BinaryStream_SerializeFloat64, f64)

#endif
