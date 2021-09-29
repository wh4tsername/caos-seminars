#!/bin/bash

LINARO_TOOLCHAIN=/home/wasd/seminars/caos/sem4/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu

$LINARO_TOOLCHAIN/bin/aarch64-linux-gnu-gdb --nx $1.out

