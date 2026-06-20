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
  
  if (S->mode == BinaryStreamMode_Write) {
    for (size_t i = 0; i < size; i++)
      S->cursor[i] = data[i];
  } else {
    for (size_t i = 0; i < size; i++)
      data[i] = S->cursor[i];
  }

  S->cursor += size;
}
