#!/bin/bash

LINARO_TOOLCHAIN=/home/wasd/seminars/caos/sem4/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu

$LINARO_TOOLCHAIN/bin/aarch64-linux-gnu-gcc $1.c -o $1.out
qemu-aarch64 -L $LINARO_TOOLCHAIN/aarch64-linux-gnu/libc ./$1.out

