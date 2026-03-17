CFLAGS += \
	-Wall \
	-Wextra \
	-Werror=unused-result \
	-Wpedantic \
	-Werror \
	-Wmissing-prototypes \
	-Wshadow \
	-Wpointer-arith \
	-Wredundant-decls \
	-Wconversion \
	-Wsign-conversion \
	-Wno-long-long \
	-Wno-unknown-pragmas \
	-O3 \
	-g \
	-fomit-frame-pointer \
	-std=c99 \
	-pedantic

VALGRIND ?= valgrind
VALGRIND_FLAGS += \
	--leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	--verbose

PERF ?= perf

BENCH_MAIN := src/bench.c
RESULTS_DIR := results
BIN_DIR := bin

BINARIES := \
	$(BIN_DIR)/formosa-mlkem-768-avx2 \
	$(BIN_DIR)/formosa-mlkem-1024-avx2 \
	$(BIN_DIR)/mlkem-native-768-avx2 \
	$(BIN_DIR)/mlkem-native-1024-avx2

default: run

.PHONY:
	clean
	format
	run
	valgrind
	perf
	all

compile: $(BINARIES)

run: compile $(RESULTS_DIR) \
	$(RESULTS_DIR)/formosa-mlkem-768-avx2.txt \
	$(RESULTS_DIR)/formosa-mlkem-1024-avx2.txt \
	$(RESULTS_DIR)/mlkem-native-768-avx2.txt \
	$(RESULTS_DIR)/mlkem-native-1024-avx2.txt

perf: run
	$(PERF) record -o $(RESULTS_DIR)/formosa_768.data \
		taskset -c 0 ./$(BIN_DIR)/formosa-mlkem-768-avx2 > /dev/null
	$(PERF) record -o $(RESULTS_DIR)/formosa_1024.data \
		taskset -c 0 ./$(BIN_DIR)/formosa-mlkem-1024-avx2 > /dev/null
	$(PERF) record -o $(RESULTS_DIR)/native_768.data \
		taskset -c 0 ./$(BIN_DIR)/mlkem-native-768-avx2 > /dev/null
	$(PERF) record -o $(RESULTS_DIR)/native_1024.data \
		taskset -c 0 ./$(BIN_DIR)/mlkem-native-1024-avx2 > /dev/null

format:
	clang-format -i src/*.c src/*.h

valgrind: compile
	$(VALGRIND) $(VALGRIND_FLAGS) ./$(BIN_DIR)/formosa-mlkem-768-avx2 > /dev/null
	$(VALGRIND) $(VALGRIND_FLAGS) ./$(BIN_DIR)/mlkem-native-768-avx2 > /dev/null

clean:
	$(RM) -f $(BIN_DIR)/formosa-mlkem-*
	$(RM) -f $(BIN_DIR)/mlkem-native-*
	$(RM) -f $(BIN_DIR)/pqcp-mlkem-*
	$(RM) -rf $(RESULTS_DIR)
	$(MAKE) -C randombytes/ clean
	$(MAKE) -C third_party/mlkem-native clean

$(RESULTS_DIR):
	mkdir -p $(RESULTS_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(RESULTS_DIR)/%.txt: $(BIN_DIR)/%
	taskset -c 0 $< > $@

$(BIN_DIR)/formosa-mlkem-768-avx2: randombytes/librandombytes1.a $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=JADE_KEM_mlkem_mlkem768_amd64_avx2 \
		-DJADE_NAMESPACE_LC=jade_kem_mlkem_mlkem768_amd64_avx2 \
		-Irandombytes/ \
		-I../code/jasmin/768/avx2/include/ \
    ../code/jasmin/768/avx2/jkem.s \
    randombytes/librandombytes1.a

$(BIN_DIR)/formosa-mlkem-1024-avx2: randombytes/librandombytes1.a $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=JADE_KEM_mlkem_mlkem1024_amd64_avx2 \
		-DJADE_NAMESPACE_LC=jade_kem_mlkem_mlkem1024_amd64_avx2 \
		-Irandombytes/ \
		-I../code/jasmin/1024/avx2/include/ \
    ../code/jasmin/1024/avx2/jkem.s \
    randombytes/librandombytes1.a

$(BIN_DIR)/pqcp-mlkem-768-avx2: randombytes/librandombytes1.a $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=JADE_KEM_mlkem_mlkem768_amd64_avx2 \
		-DJADE_NAMESPACE_LC=JADE_KEM_MLKEM_MLKEM768_AMD64_AVX2 \
		-Irandombytes/ \
		-I../code/jasmin/768/avx2/include/ \
		third_party/mlkem-libjade/src/mlkem768_amd64_avx2/mlkem768_amd64_avx2.s \
    randombytes/librandombytes1.a

$(BIN_DIR)/pqcp-mlkem-1024-avx2: randombytes/librandombytes1.a $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=JADE_KEM_mlkem_mlkem1024_amd64_avx2 \
		-DJADE_NAMESPACE_LC=JADE_KEM_MLKEM_MLKEM1024_AMD64_AVX2 \
		-Irandombytes/ \
		-I../code/jasmin/1024/avx2/include/ \
		third_party/mlkem-libjade/src/mlkem1024_amd64_avx2/mlkem1024_amd64_avx2.s \
    randombytes/librandombytes1.a

$(BIN_DIR)/mlkem-native-768-avx2: randombytes/librandombytes1.a \
	third_party/mlkem-native/test/build/libmlkem768.a  $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=PQCP_MLKEM_NATIVE_MLKEM768 \
		-DJADE_NAMESPACE_LC=PQCP_MLKEM_NATIVE_MLKEM768 \
		-Irandombytes/ \
		-Ithird_party/bindings/mlkem-native-768/ \
		third_party/mlkem-native/test/build/libmlkem768.a \
    randombytes/librandombytes1.a

$(BIN_DIR)/mlkem-native-1024-avx2: randombytes/librandombytes1.a \
	third_party/mlkem-native/test/build/libmlkem1024.a $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=PQCP_MLKEM_NATIVE_MLKEM1024 \
		-DJADE_NAMESPACE_LC=PQCP_MLKEM_NATIVE_MLKEM1024 \
		-Irandombytes/ \
		-Ithird_party/bindings/mlkem-native-1024/ \
		third_party/mlkem-native/test/build/libmlkem1024.a \
    randombytes/librandombytes1.a

third_party/mlkem-native/test/build/libmlkem768.a:
	$(MAKE) -C third_party/mlkem-native/ OPT=1 AUTO=1 func_768

third_party/mlkem-native/test/build/libmlkem1024.a:
	$(MAKE) -C third_party/mlkem-native/ OPT=1 AUTO=1 func_1024

randombytes/librandombytes1.a:
	$(MAKE) -C randombytes/
