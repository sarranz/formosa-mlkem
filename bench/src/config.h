#ifndef CONFIG_H
#define CONFIG_H

#define TIMES 1000
#define ITERS 100
#define RUNS (TIMES * ITERS)

#define WARMUP 50

#if WARMUP > RUNS
#error "WARMUP must be at most RUNS"
#endif

#endif
