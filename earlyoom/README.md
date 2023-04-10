# Earlyoom build

This script helps building /opt/vlab/bin/earlyoom under Ubuntu 18.04 (the lowest distro glibc we need to support).

```
docker build -t local/earlyoom-build .
docker run --rm -v $(pwd):/output local/earlyoom-build
```
