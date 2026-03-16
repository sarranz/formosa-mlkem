#ifndef CONFIG_H
#define CONFIG_H

#define RUNS 10000

#define WARMUP 50

#if WARMUP > RUNS
#error "WARMUP must be at most RUNS"
#endif

#endif

