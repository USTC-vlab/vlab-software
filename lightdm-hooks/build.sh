#!/bin/sh -xe

gcc -fPIC -shared -o /workspace/xsessions-error-to-journald.so /workspace/xsessions-error-to-journald.c -ldl
gcc -fPIC -shared -o /workspace/no-create-seat0.so /workspace/no-create-seat0.c -ldl
