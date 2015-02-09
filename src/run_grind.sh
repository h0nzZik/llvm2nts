#!/bin/sh
valgrind opt -load ./libllvm2nts.so -llvm2nts ../../examples/02_gcd.ll \
	-llvm2nts_of="output.nts" > /dev/null
