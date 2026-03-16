#ifndef ALIGNED_ALLOC_C
#define ALIGNED_ALLOC_C

#include <stdint.h>
#include <stdlib.h>

#define ALIGN 8
#define TWOALIGN (1 << ALIGN)

// Rounds len up to the next multiple of TWOALIGN.
static inline size_t aligned_alloc_step(size_t len) {
  return len + ((TWOALIGN - 1) & -len);
}

// Allocate enough space for n aligned subarrays of len bytes each.
// Arguments:
//     _x: output pointer for the original allocated pointer (for later free).
//     n: number of aligned subarrays.
//     len: size in bytes of each subarray before alignment step rounding.
// Returns:
//     pointer to the first aligned subarray, or NULL on failure.
static uint8_t *aligned_alloc(uint8_t **_x, size_t n, size_t len) {
  if (_x == NULL) {
    return NULL;
  }

  size_t step = aligned_alloc_step(len);
  size_t total = n * step;

  if ((step != 0 && n > SIZE_MAX / step) || (total > SIZE_MAX - 2 * TWOALIGN)) {
    return NULL;
  }

  uint8_t *x = (uint8_t *)malloc(total + 2 * TWOALIGN);
  if (x == NULL) {
    return NULL;
  }

  *_x = x;
  x += (TWOALIGN - 1) & (-(uintptr_t)x);
  return x;
}

#endif
