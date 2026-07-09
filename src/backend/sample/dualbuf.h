#ifndef __BACKEND_SAMPLE_DUALBUF_H__
#define __BACKEND_SAMPLE_DUALBUF_H__

#include <stddef.h>
#include <dc2.h>

typedef struct {
  // Size of single buffer.
  i32 size;
  // Points to the memory block of f32[2 * size].
  f32 *scratch;
  // Points to the active writing buffer (f32[size]).
  f32 *active;
  // Points to the currently ready buffer.
  f32 *ready;
  // Element index inside of currently writing buffer.
  i32 index;
  // Buffer index of currently writing buffer.
  i08 bufIdx;
} DualBuf;

static inline void DualBuf_Initialize(
  DualBuf *D,
  f32 *scratch,
  i32 size
) {
  D->size = size;
  D->index = D->bufIdx = 0;
  D->scratch = D->active = scratch;
  D->ready = NULL;
}

void DualBuf_Append(
  DualBuf *D,
  f32 val
) {
  if (D->index >= D->size) {
    // Exchange buffer.
    D->bufIdx = !D->bufIdx;
    D->ready = D->active;
    D->active = D->scratch + D->size * D->bufIdx;
    D->index = 0;
  }
  // Append value.
  D->active[D->index] = val;
  D->index++;
}

f32 *DualBuf_GetReady(
  const DualBuf *D
) {
  return D->ready;
}

f32 *DualBuf_TakeReady(
  DualBuf *D
) {
  f32 *ret = D->ready;
  D->ready = NULL;
  return ret;
}

#endif
