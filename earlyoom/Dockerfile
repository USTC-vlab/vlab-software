FROM ubuntu:18.04

RUN sed -i 'p; s/^deb /deb-src /' /etc/apt/sources.list && apt-get update && \
    apt-get build-dep -y earlyoom && apt-get install -y git
COPY build.sh /build.sh
CMD /build.sh
