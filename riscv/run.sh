#!/bin/sh

IMAGE=vlab-riscv
CONTAINER="$IMAGE"

cd "$(dirname "$0")"
docker build -t "$IMAGE" .
docker run --rm --name="$CONTAINER" -v "$PWD/output":/output "$IMAGE"
