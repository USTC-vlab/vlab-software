#!/bin/sh

WORKDIR="$PWD/verilator"
REPO=https://github.com/verilator/verilator.git
PREFIX=/opt/vlab

git clone --depth=1 --single-branch "$REPO" "$WORKDIR"

cd "$WORKDIR"
autoconf
./configure --prefix="$PREFIX"
make -j $(nproc)
make install
tar zcf /output/verilator.tar.gz -C / "$PREFIX"
