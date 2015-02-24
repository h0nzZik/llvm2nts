#!/bin/sh
EXDIR="./externals/nts-examples"
valgrind opt -load ./pass/libpass_llvm2nts.so -llvm2nts "$EXDIR/02_gcd.ll" \
	-llvm2nts_of="output.nts" > /dev/null
