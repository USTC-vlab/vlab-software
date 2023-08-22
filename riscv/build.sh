#!/bin/sh

WORKDIR="$PWD/riscv"
REPO=https://github.com/riscv/riscv-gnu-toolchain.git
PREFIX=/opt/vlab/riscv64

# Saves disk space by shallow cloning
git clone --depth=1 --single-branch "$REPO" "$WORKDIR"

cd "$WORKDIR"
./configure --prefix="$PREFIX" --enable-multilib --target=riscv64-multilib-elf
make -j $(nproc)
make install
tar zcf /output/riscv.tar.gz -C / "$PREFIX"
