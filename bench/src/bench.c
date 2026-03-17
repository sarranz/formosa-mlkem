/* Benchmarks for the KEM API.

   We benchmark each operation separately.
   For each operation, we run WARMUP iterations without recording cycles, then
   we run TIMES iterations of ITERS iterations each, recording the average
   cycles per iteration for each of the TIMES runs.
*/
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "api.h"
#include "namespace.h"
#include "randombytes1.h"

#include "aligned_alloc.c"
#include "config.h"
#include "cpucycles.c"

#define CRYPTO_SECRETKEYBYTES NAMESPACE(SECRETKEYBYTES)
#define CRYPTO_PUBLICKEYBYTES NAMESPACE(PUBLICKEYBYTES)
#define CRYPTO_KEYPAIRCOINBYTES NAMESPACE(KEYPAIRCOINBYTES)

#define CRYPTO_CIPHERTEXTBYTES NAMESPACE(CIPHERTEXTBYTES)
#define CRYPTO_BYTES NAMESPACE(BYTES)
#define CRYPTO_ENCCOINBYTES NAMESPACE(ENCCOINBYTES)

#define CRYPTO_ALGNAME NAMESPACE(ALGNAME)
#define CRYPTO_ARCH NAMESPACE(ARCH)
#define CRYPTO_IMPL NAMESPACE(IMPL)

#define crypto_kem_keypair NAMESPACE_LC(keypair)
#define crypto_kem_keypair_derand NAMESPACE_LC(keypair_derand)
#define crypto_kem_enc NAMESPACE_LC(enc)
#define crypto_kem_enc_derand NAMESPACE_LC(enc_derand)
#define crypto_kem_dec NAMESPACE_LC(dec)

#define OPS 5
#define OP_KG 0
#define OP_KGD 1
#define OP_ENC 2
#define OP_ENCD 3
#define OP_DEC 4

static const char *OP_NAMES[OPS] = {"keypair", "keypair derand", "enc",
                                    "enc derand", "dec"};

static void error(const char *operation, const char *message) {
  fprintf(stderr, "Error in %s: %s\n", operation, message);
  exit(-1);
}

static int init_coins(uint8_t *coins, size_t n) {
  size_t clen = alalloc_step(n);
  uint8_t *c = coins;
  int r = 0;

  for (size_t i = 0; i < RUNS; i++) {
    r |= randombytes(c, n);
    c += clen;
  }

  return r;
}

// Benchmark key generation.
// Arguments:
//     times: output cycle counts for each run.
//     pub: output buffer for RUNS aligned public keys.
//     sec: output buffer for RUNS aligned secret keys.
// Results:
//     zero means success, nonzero means failure.
static int bench_keypair(uint64_t times[TIMES], uint8_t *pub, uint8_t *sec) {
  size_t plen = alalloc_step(CRYPTO_PUBLICKEYBYTES);
  size_t slen = alalloc_step(CRYPTO_SECRETKEYBYTES);
  int r = 0;

  uint8_t *p = pub;
  uint8_t *s = sec;
  for (size_t i = 0; i < WARMUP; i++) {
    r |= crypto_kem_keypair(p, s);
    p += plen;
    s += slen;
  }

  p = pub;
  s = sec;
  for (size_t i = 0; i < TIMES; i++) {
    uint64_t t = cpucycles();
    for (size_t j = 0; j < ITERS; j++) {
      r |= crypto_kem_keypair(p, s);
      p += plen;
      s += slen;
    }
    times[i] = (cpucycles() - t) / ITERS;
  }

  return r;
}

// Benchmark deterministic key generation.
// Arguments:
//     times: output cycle counts for each run.
//     pub: output buffer for RUNS aligned public keys.
//     sec: output buffer for RUNS aligned secret keys.
//     coins: input buffer with RUNS aligned coin blocks.
// Results:
//     zero means success, nonzero means failure.
static int bench_keypair_derand(uint64_t times[TIMES], uint8_t *pub,
                                uint8_t *sec, uint8_t *coins) {
  size_t plen = alalloc_step(CRYPTO_PUBLICKEYBYTES);
  size_t slen = alalloc_step(CRYPTO_SECRETKEYBYTES);
  size_t clen = alalloc_step(CRYPTO_KEYPAIRCOINBYTES);
  int r = 0;

  uint8_t *p = pub;
  uint8_t *s = sec;
  uint8_t *c = coins;
  for (size_t i = 0; i < WARMUP; i++) {
    r |= crypto_kem_keypair_derand(p, s, c);
    p += plen;
    s += slen;
    c += clen;
  }

  p = pub;
  s = sec;
  c = coins;
  for (size_t i = 0; i < TIMES; i++) {
    uint64_t t = cpucycles();
    for (size_t j = 0; j < ITERS; j++) {
      r |= crypto_kem_keypair_derand(p, s, c);
      p += plen;
      s += slen;
      c += clen;
    }
    times[i] = (cpucycles() - t) / ITERS;
  }

  return r;
}

// Benchmark encapsulation.
// Arguments:
//     times: output cycle counts for each run.
//     ct: output buffer for RUNS aligned ciphertexts.
//     key: output buffer for RUNS aligned shared secrets.
//     pub: input buffer for RUNS aligned public keys.
// Results:
//     zero means success, nonzero means failure.
static int bench_enc(uint64_t times[TIMES], uint8_t *ct, uint8_t *key,
                     uint8_t *pub) {
  size_t clen = alalloc_step(CRYPTO_CIPHERTEXTBYTES);
  size_t klen = alalloc_step(CRYPTO_BYTES);
  size_t plen = alalloc_step(CRYPTO_PUBLICKEYBYTES);
  int r = 0;

  uint8_t *c = ct;
  uint8_t *k = key;
  uint8_t *p = pub;
  for (size_t i = 0; i < WARMUP; i++) {
    r |= crypto_kem_enc(c, k, p);
    c += clen;
    k += klen;
    p += plen;
  }

  c = ct;
  k = key;
  p = pub;
  for (size_t i = 0; i < TIMES; i++) {
    uint64_t t = cpucycles();
    for (size_t j = 0; j < ITERS; j++) {
      r |= crypto_kem_enc(c, k, p);
      c += clen;
      k += klen;
      p += plen;
    }
    times[i] = (cpucycles() - t) / ITERS;
  }

  return r;
}

// Benchmark deterministic encapsulation.
// Arguments:
//     times: output cycle counts for each run.
//     ct: output buffer for RUNS aligned ciphertexts.
//     key: output buffer for RUNS aligned shared secrets.
//     pub: input buffer for RUNS aligned public keys.
//     coins: input buffer with RUNS aligned coin blocks.
// Results:
//     zero means success, nonzero means failure.
static int bench_enc_derand(uint64_t times[TIMES], uint8_t *ct, uint8_t *key,
                            uint8_t *pub, uint8_t *coins) {
  size_t ctlen = alalloc_step(CRYPTO_CIPHERTEXTBYTES);
  size_t klen = alalloc_step(CRYPTO_BYTES);
  size_t plen = alalloc_step(CRYPTO_PUBLICKEYBYTES);
  size_t clen = alalloc_step(CRYPTO_ENCCOINBYTES);
  int r = 0;

  uint8_t *ctp = ct;
  uint8_t *k = key;
  uint8_t *p = pub;
  uint8_t *c = coins;
  for (size_t i = 0; i < WARMUP; i++) {
    r |= crypto_kem_enc_derand(ctp, k, p, c);
    ctp += ctlen;
    k += klen;
    p += plen;
    c += clen;
  }

  ctp = ct;
  k = key;
  p = pub;
  c = coins;
  for (size_t i = 0; i < TIMES; i++) {
    uint64_t t = cpucycles();
    for (size_t j = 0; j < ITERS; j++) {
      r |= crypto_kem_enc_derand(ctp, k, p, c);
      ctp += ctlen;
      k += klen;
      p += plen;
      c += clen;
    }
    times[i] = (cpucycles() - t) / ITERS;
  }

  return r;
}

// Benchmark decapsulation.
// Arguments:
//     times: output cycle counts for each run.
//     key: output buffer for RUNS aligned shared secrets.
//     ct: input buffer for RUNS aligned ciphertexts.
//     sec: input buffer for RUNS aligned secret keys.
// Results:
//     zero means success, nonzero means failure.
static int bench_dec(uint64_t times[TIMES], uint8_t *key, uint8_t *ct,
                     uint8_t *sec) {
  size_t klen = alalloc_step(CRYPTO_BYTES);
  size_t clen = alalloc_step(CRYPTO_CIPHERTEXTBYTES);
  size_t slen = alalloc_step(CRYPTO_SECRETKEYBYTES);
  int r = 0;

  uint8_t *k = key;
  uint8_t *c = ct;
  uint8_t *s = sec;
  for (size_t i = 0; i < WARMUP; i++) {
    r |= crypto_kem_dec(k, c, s);
    k += klen;
    c += clen;
    s += slen;
  }

  k = key;
  c = ct;
  s = sec;
  for (size_t i = 0; i < TIMES; i++) {
    uint64_t t = cpucycles();
    for (size_t j = 0; j < ITERS; j++) {
      r |= crypto_kem_dec(k, c, s);
      k += klen;
      c += clen;
      s += slen;
    }
    times[i] = (cpucycles() - t) / ITERS;
  }

  return r;
}

static void print_timings(uint64_t times[OPS][TIMES]) {
  for (size_t op = 0; op < OPS; op++) {
    printf("%s\n", OP_NAMES[op]);
    for (size_t i = 0; i < TIMES; i++) {
      printf("%" PRIu64 "\n", times[op][i]);
    }
    printf("\n");
  }
}

int main(void) {
  uint64_t times[OPS][TIMES];

  uint8_t *_pub = NULL;
  uint8_t *pub = alalloc(&_pub, RUNS, CRYPTO_PUBLICKEYBYTES);
  if (pub == NULL || _pub == NULL)
    error("main", "failed to allocate pub");

  uint8_t *_sec = NULL;
  uint8_t *sec = alalloc(&_sec, RUNS, CRYPTO_SECRETKEYBYTES);
  if (sec == NULL || _sec == NULL)
    error("main", "failed to allocate sec");

  uint8_t *_ct = NULL;
  uint8_t *ct = alalloc(&_ct, RUNS, CRYPTO_CIPHERTEXTBYTES);
  if (ct == NULL || _ct == NULL)
    error("main", "failed to allocate ct");

  uint8_t *_key = NULL;
  uint8_t *key = alalloc(&_key, RUNS, CRYPTO_BYTES);
  if (key == NULL || _key == NULL)
    error("main", "failed to allocate key");

  uint8_t *_coins_kg = NULL;
  uint8_t *coins_kg = alalloc(&_coins_kg, RUNS, CRYPTO_KEYPAIRCOINBYTES);
  if (coins_kg == NULL || _coins_kg == NULL)
    error("main", "failed to allocate coins");

  uint8_t *_coins_enc = NULL;
  uint8_t *coins_enc = alalloc(&_coins_enc, RUNS, CRYPTO_ENCCOINBYTES);
  if (coins_enc == NULL || _coins_enc == NULL)
    error("main", "failed to allocate coins");

  int r = init_coins(coins_kg, CRYPTO_KEYPAIRCOINBYTES);
  if (r != 0)
    error("keypair derand", "randombytes failed");

  r = init_coins(coins_enc, CRYPTO_ENCCOINBYTES);
  if (r != 0)
    error("enc derand", "randombytes failed");

  r = bench_keypair(times[OP_KG], pub, sec);
  if (r != 0)
    error("keypair", "benchmarking");

  r = bench_keypair_derand(times[OP_KGD], pub, sec, coins_kg);
  if (r != 0)
    error("keypair derand", "benchmarking");

  r = bench_enc(times[OP_ENC], ct, key, pub);
  if (r != 0)
    error("enc", "benchmarking");

  r = bench_enc_derand(times[OP_ENCD], ct, key, pub, coins_enc);
  if (r != 0)
    error("enc derand", "benchmarking");

  r = bench_dec(times[OP_DEC], key, ct, sec);
  if (r != 0)
    error("dec", "benchmarking");

  free(_pub);
  free(_sec);
  free(_ct);
  free(_key);
  free(_coins_kg);
  free(_coins_enc);

  print_timings(times);

  return 0;
}
