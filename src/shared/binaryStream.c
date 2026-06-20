#include <dc2.h>
#include "shared/binaryStream.h"

static i08 BinaryStream_Verify(
  BinaryStream *S,
  ptrdiff_t consume
) {
  if (!S->pBuffer || S->errorFlag)
    return S->errorFlag = 1;

  if (S->cursor - S->pBuffer + consume > S->maxSize)
    return S->errorFlag = 1;

  return 0;
}

void BinaryStream_Construct(
  BinaryStream *S,
  u08 *pBuffer,
  size_t maxSize,
  BinaryStreamMode mode
) {
  S->errorFlag = 0;
  S->cursor = S->pBuffer = pBuffer;
  S->maxSize = maxSize;
  S->mode = mode;
}

void BinaryStream_SerializeBytes(
  BinaryStream *S,
  u08 *data,
  size_t size
) {
  if (BinaryStream_Verify(S, size))
    return;

  for (size_t i = 0; i < size; i++) {
    if (S->mode == BinaryStreamMode_Write)
      S->cursor[i] = data[i];
    else
      data[i] = S->cursor[i];
  }

  S->cursor += size;
}

/*
// We need the compiler to perform type checking on pointers, so we do not
// use macro declarations. 
#define BinaryStream_SerializeProto(_Name, _Type) \
void _Name(\
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
*/