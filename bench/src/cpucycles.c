#ifndef CPUCYCLES_C
#define CPUCYCLES_C

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static inline uint64_t cpucycles(void) {
  uint64_t result;

  __asm__ volatile("rdtsc; shlq $32,%%rdx; orq %%rdx,%%rax"
                   : "=a"(result)
                   :
                   : "%rdx");

  return result;
}

#endif
