SML_FAST_REAL_SOURCES=../lib/github.com/shwestrick/sml-fast-real/**.mlb ../lib/github.com/shwestrick/sml-fast-real/**.sml
SML_FAST_REAL_DIR=../lib/github.com/shwestrick/sml-fast-real
C_SOURCES=$(SML_FAST_REAL_DIR)/digit_parse_neon.c
C_FLAGS=-O3 -c -target arm64-apple-macos -fPIC
CPP_FLAGS=-O3 -c -target arm64-apple-macos -fPIC -std=c++17


default: test

lib:
	smlpkg sync

$(SML_FAST_REAL_DIR)/digit_parse_neon.o: $(C_SOURCES)
	clang $(C_FLAGS) $(C_SOURCES) -o $@

$(SML_FAST_REAL_DIR)/fast_float_wrapper.o: $(SML_FAST_REAL_DIR)/fast_float_wrapper.c
	clang++ $(CPP_FLAGS) $(SML_FAST_REAL_DIR)/fast_float_wrapper.c -o $@

test: $(SML_FAST_REAL_SOURCES) lib test.sml test.mlb
	mlton -default-type int64 -default-type word64 test.mlb 

test.fast: $(SML_FAST_REAL_SOURCES) lib test.sml test.mlb $(SML_FAST_REAL_DIR)/fast_float_wrapper.o
	mlton -default-type int64 -default-type word64 \
	-default-ann 'allowFFI true' \
	-link-opt $(SML_FAST_REAL_DIR)/fast_float_wrapper.o \
	-link-opt -lstdc++ \
	test.mlb


test.mpl: $(SML_FAST_REAL_SOURCES) lib test.sml test.mlb
	mpl -default-type int64 -default-type word64 test.mpl.mlb

test.ffi.mpl: $(SML_FAST_REAL_SOURCES) lib test.sml test.mlb $(SML_FAST_REAL_DIR)/digit_parse_neon.o
	mpl -default-type int64 -default-type word64 \
	-default-ann 'allowFFI true' \
	-link-opt $(SML_FAST_REAL_DIR)/digit_parse_neon.o \
	test.mpl.mlb

test.ffi: $(SML_FAST_REAL_SOURCES) lib test.sml test.mlb $(SML_FAST_REAL_DIR)/digit_parse_neon.o
	mlton -default-type int64 -default-type word64 \
	-default-ann 'allowFFI true' \
	-link-opt $(SML_FAST_REAL_DIR)/digit_parse_neon.o \
	test.mlb
clean:
	rm -f $(SML_FAST_REAL_DIR)/digit_parse_neon.o
	rm -f $(SML_FAST_REAL_DIR)/fast_float_wrapper.o
	rm test