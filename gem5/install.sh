#!/bin/bash

. /etc/os-release

case "$VERSION_ID" in
  22.04) sudo apt install -y libhdf5-103 libhdf5-cpp-103 libprotobuf23 libtcmalloc-minimal4 ;;
  24.04) sudo apt install -y libhdf5-103-1t64 libhdf5-cpp-103-1t64 libprotobuf32t64 libtcmalloc-minimal4t64 ;;
  *) echo "目前 Vlab GEM5 仅支持 Ubuntu 22.04/24.04，如果有其他需要，请联系我们。" >&2 ;;
esac
