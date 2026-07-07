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

void BinaryStream_Reset(
  BinaryStream *S
) {
  S->errorFlag = 0;
  S->cursor = S->pBuffer;
}

void BinaryStream_Seek(
  BinaryStream *S,
  size_t location
) {
  if (location > S->maxSize) {
    S->errorFlag = 1;
    return;
  }

  S->cursor = S->pBuffer + location;
}

void BinaryStream_Skip(
  BinaryStream *S,
  size_t count
) {
  if (BinaryStream_Verify(S, count))
    return;
  S->cursor += count;
}

size_t BinaryStream_GetNumBytesUsed(
  BinaryStream *S
) {
  return S->cursor - S->pBuffer;
}

void BinaryStream_SerializeBytes(
  BinaryStream *S,
  u08 *data,
  size_t size
) {
  if (BinaryStream_Verify(S, size))
    return;
  
  if (S->mode == kBinaryStreamMode_Write) {
    for (size_t i = 0; i < size; i++)
      S->cursor[i] = data[i];
  } else {
    for (size_t i = 0; i < size; i++)
      data[i] = S->cursor[i];
  }

  S->cursor += size;
}
