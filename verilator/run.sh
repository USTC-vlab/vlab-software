#!/bin/sh

cd "$(dirname "$0")"
docker build -t vlab-verilator .
docker run --rm --name=vlab-verilator -v "$PWD/output":/output vlab-verilator
