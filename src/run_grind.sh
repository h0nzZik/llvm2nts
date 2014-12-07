#!/bin/sh
valgrind opt -load ./libllvm2nts.so -hello < ../../examples/02_gcd.ll > /dev/null
