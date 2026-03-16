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

ALL := \
	bin/formosa-mlkem-768-avx2 \
	bin/formosa-mlkem-1024-avx2 \
	bin/mlkem-native-768-avx2 \
	bin/mlkem-native-1024-avx2

default: run

compile: $(ALL)

run: compile
	@./bin/mlkem-native-768-avx2 "" "nat.mlkem768 avx2"
	@./bin/mlkem-native-1024-avx2 "" "nat.mlkem1024 avx2"
	@./bin/formosa-mlkem-768-avx2 "" "fmlkem768 avx2"
	@./bin/formosa-mlkem-1024-avx2 "" "fmlkem1024 avx2"

#-- formosa-mlkem (this repository ----------------------------------
#-- avx2
bin/formosa-mlkem-768-avx2: randombytes/librandombytes1.a
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=JADE_KEM_mlkem_mlkem768_amd64_avx2 \
		-DJADE_NAMESPACE_LC=jade_kem_mlkem_mlkem768_amd64_avx2 \
		-Irandombytes/ \
		-I../code/jasmin/768/avx2/include/ \
    ../code/jasmin/768/avx2/jkem.s \
    randombytes/librandombytes1.a

bin/formosa-mlkem-1024-avx2: randombytes/librandombytes1.a
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=JADE_KEM_mlkem_mlkem1024_amd64_avx2 \
		-DJADE_NAMESPACE_LC=jade_kem_mlkem_mlkem1024_amd64_avx2 \
		-Irandombytes/ \
		-I../code/jasmin/1024/avx2/include/ \
    ../code/jasmin/1024/avx2/jkem.s \
    randombytes/librandombytes1.a

#--ref
bin/formosa-mlkem-768-ref: randombytes/librandombytes1.a
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=JADE_KEM_mlkem_mlkem768_amd64_ref \
		-DJADE_NAMESPACE_LC=jade_kem_mlkem_mlkem768_amd64_ref \
		-Irandombytes/ \
		-I../code/jasmin/768/ref/include/ \
    ../code/jasmin/768/ref/jkem.s \
    randombytes/librandombytes1.a

bin/formosa-mlkem-1024-ref: randombytes/librandombytes1.a
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=JADE_KEM_mlkem_mlkem1024_amd64_ref \
		-DJADE_NAMESPACE_LC=jade_kem_mlkem_mlkem1024_amd64_ref \
		-Irandombytes/ \
		-I../code/jasmin/1024/ref/include/ \
    ../code/jasmin/1024/ref/jkem.s \
    randombytes/librandombytes1.a

#-- mlkem-libjade (from pq-code-package) ----------------------------
#--avx2
bin/pqcp-mlkem-768-avx2: randombytes/librandombytes1.a
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=JADE_KEM_mlkem_mlkem768_amd64_avx2 \
		-DJADE_NAMESPACE_LC=jade_kem_mlkem_mlkem768_amd64_avx2 \
		-Irandombytes/ \
		-I../code/jasmin/768/avx2/include/ \
		third_party/mlkem-libjade/src/mlkem768_amd64_avx2/mlkem768_amd64_avx2.s \
    randombytes/librandombytes1.a

bin/pqcp-mlkem-1024-avx2: randombytes/librandombytes1.a
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=JADE_KEM_mlkem_mlkem1024_amd64_avx2 \
		-DJADE_NAMESPACE_LC=jade_kem_mlkem_mlkem1024_amd64_avx2 \
		-Irandombytes/ \
		-I../code/jasmin/1024/avx2/include/ \
		third_party/mlkem-libjade/src/mlkem1024_amd64_avx2/mlkem1024_amd64_avx2.s \
    randombytes/librandombytes1.a

#-- mlkem-native (from pq-code-package) -----------------------------
#--avx2
bin/mlkem-native-768-avx2: randombytes/librandombytes1.a third_party/mlkem-native/test/build/libmlkem768.a
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=PQCP_MLKEM_NATIVE_MLKEM768 \
		-DJADE_NAMESPACE_LC=PQCP_MLKEM_NATIVE_MLKEM768 \
		-Irandombytes/ \
		-Ithird_party/bindings/mlkem-native-768/ \
		third_party/mlkem-native/test/build/libmlkem768.a \
    randombytes/librandombytes1.a

bin/mlkem-native-1024-avx2: randombytes/librandombytes1.a third_party/mlkem-native/test/build/libmlkem1024.a
	$(CC) $(CFLAGS) -o $@ $(BENCH_MAIN) \
		-DJADE_NAMESPACE=PQCP_MLKEM_NATIVE_MLKEM1024 \
		-DJADE_NAMESPACE_LC=PQCP_MLKEM_NATIVE_MLKEM1024 \
		-Irandombytes/ \
		-Ithird_party/bindings/mlkem-native-1024/ \
		third_party/mlkem-native/test/build/libmlkem1024.a \
    randombytes/librandombytes1.a

third_party/mlkem-native/test/build/libmlkem768.a:
	$(MAKE) \
		MK_COMPILER_SUPPORTS_AVX2=1 \
		MK_COMPILER_SUPPORTS_BMI2=1 \
		MK_COMPILER_SUPPORTS_SSE2=1 \
		MK_HOST_SUPPORTS_AVX2=1 \
		MK_HOST_SUPPORTS_BMI2=1 \
		MK_HOST_SUPPORTS_SSE2=1 \
		AUTO=1 \
		OPT=1 \
		-C third_party/mlkem-native/ build

 third_party/mlkem-native/test/build/libmlkem1024.a:
	$(MAKE) \
		-C third_party/mlkem-native/ build

#-- randombytes -----------------------------------------------------
randombytes/librandombytes1.a:
	$(MAKE) -C randombytes/

#-- misc ------------------------------------------------------------
.PHONY: clean
clean:
	$(RM) -f bin/formosa-mlkem-*
	$(RM) -f bin/mlkem-native-*
	$(RM) -f bin/pqcp-mlkem-*
	$(MAKE) -C randombytes/ clean
	$(MAKE) -C third_party/mlkem-native clean
