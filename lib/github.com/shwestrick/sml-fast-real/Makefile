FAST_FLOAT_DIR ?= fast_float

all: fast_float_wrapper.o
	mlton -default-ann 'allowFFI true' -export-header export.h \
		-link-opt "-L. -lstdc++" \
		DigitParse.sml fast_float_wrapper.o

fast_float_wrapper.o: fast_float_wrapper.c
	clang++ -std=c++17 -O3 -I$(FAST_FLOAT_DIR)/include -c fast_float_wrapper.c -o fast_float_wrapper.o

clean:
	rm -f DigitParse fast_float_wrapper.o export.h

.PHONY: all clean