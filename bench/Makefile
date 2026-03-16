CFLAGS ?= \
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
	-fomit-frame-pointer \
	-std=c99 \
	-pedantic

BENCH_MAIN := src/bench.c
RESULTS_DIR := results

ALL := \
	bin/formosa-mlkem-768-avx2 \
	bin/formosa-mlkem-1024-avx2 \
	bin/mlkem-native-768-avx2 \
	bin/mlkem-native-1024-avx2

default: run

compile: $(ALL)

run: compile
	[ -d $(RESULTS_DIR) ] || mkdir $(RESULTS_DIR)
	@./bin/formosa-mlkem-768-avx2 > $(RESULTS_DIR)/formosa_768.txt
	@./bin/formosa-mlkem-1024-avx2 > $(RESULTS_DIR)/formosa_1024.txt
	@./bin/mlkem-native-768-avx2 > $(RESULTS_DIR)/native_768.txt
	@./bin/mlkem-native-1024-avx2 > $(RESULTS_DIR)/native_1024.txt

bin/formosa-mlkem-768-avx2: randombytes/librandombytes1.a
	[ -d bin ] || mkdir bin
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=JADE_KEM_mlkem_mlkem768_amd64_avx2 \
		-DJADE_NAMESPACE_LC=jade_kem_mlkem_mlkem768_amd64_avx2 \
		-Irandombytes/ \
		-I../code/jasmin/768/avx2/include/ \
    ../code/jasmin/768/avx2/jkem.s \
    randombytes/librandombytes1.a

bin/formosa-mlkem-1024-avx2: randombytes/librandombytes1.a
	[ -d bin ] || mkdir bin
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=JADE_KEM_mlkem_mlkem1024_amd64_avx2 \
		-DJADE_NAMESPACE_LC=jade_kem_mlkem_mlkem1024_amd64_avx2 \
		-Irandombytes/ \
		-I../code/jasmin/1024/avx2/include/ \
    ../code/jasmin/1024/avx2/jkem.s \
    randombytes/librandombytes1.a

bin/pqcp-mlkem-768-avx2: randombytes/librandombytes1.a
	[ -d bin ] || mkdir bin
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=JADE_KEM_mlkem_mlkem768_amd64_avx2 \
		-DJADE_NAMESPACE_LC=JADE_KEM_MLKEM_MLKEM768_AMD64_AVX2 \
		-Irandombytes/ \
		-I../code/jasmin/768/avx2/include/ \
		third_party/mlkem-libjade/src/mlkem768_amd64_avx2/mlkem768_amd64_avx2.s \
    randombytes/librandombytes1.a

bin/pqcp-mlkem-1024-avx2: randombytes/librandombytes1.a
	[ -d bin ] || mkdir bin
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=JADE_KEM_mlkem_mlkem1024_amd64_avx2 \
		-DJADE_NAMESPACE_LC=JADE_KEM_MLKEM_MLKEM1024_AMD64_AVX2 \
		-Irandombytes/ \
		-I../code/jasmin/1024/avx2/include/ \
		third_party/mlkem-libjade/src/mlkem1024_amd64_avx2/mlkem1024_amd64_avx2.s \
    randombytes/librandombytes1.a

bin/mlkem-native-768-avx2: randombytes/librandombytes1.a third_party/mlkem-native/test/build/libmlkem768.a
	[ -d bin ] || mkdir bin
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=PQCP_MLKEM_NATIVE_MLKEM768 \
		-DJADE_NAMESPACE_LC=PQCP_MLKEM_NATIVE_MLKEM768 \
		-Irandombytes/ \
		-Ithird_party/bindings/mlkem-native-768/ \
		third_party/mlkem-native/test/build/libmlkem768.a \
    randombytes/librandombytes1.a

bin/mlkem-native-1024-avx2: randombytes/librandombytes1.a third_party/mlkem-native/test/build/libmlkem1024.a
	[ -d bin ] || mkdir bin
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

#-- randombytes -----------------------------------------------------
randombytes/librandombytes1.a:
	$(MAKE) -C randombytes/

#-- misc ------------------------------------------------------------
.PHONY: clean
clean:
	$(RM) -f bin/formosa-mlkem-*
	$(RM) -f bin/mlkem-native-*
	$(RM) -f bin/pqcp-mlkem-*
	$(RM) -rf $(RESULTS_DIR)
	$(MAKE) -C randombytes/ clean
	$(MAKE) -C third_party/mlkem-native clean
