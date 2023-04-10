#!/bin/sh -xe

git clone https://github.com/rfjakob/earlyoom.git
cd earlyoom
git checkout fabac1f3b1e2af71771f16f1e7ea81d59fbc5e98
make
cp earlyoom /output/