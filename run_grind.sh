#!/bin/sh
EXDIR="./externals/nts-examples"
valgrind --track-origins=yes opt -load ./pass/libpass_llvm2nts.so -instnamer -llvm2nts "$EXDIR/02_gcd.ll" \
	-llvm2nts_of="output.nts" > /dev/null
