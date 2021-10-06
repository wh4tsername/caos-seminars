#!/bin/bash

LINARO_TOOLCHAIN=/home/wasd/seminars/caos/sem4/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu

$LINARO_TOOLCHAIN/bin/aarch64-linux-gnu-gcc -march=armv8.1-a task0.c asm_insertion_arm.c -o task0.out
qemu-aarch64 -L $LINARO_TOOLCHAIN/aarch64-linux-gnu/libc ./task0.out
