FROM buildpack-deps:20.04-scm

ENV DEBIAN_FRONTEND=noninteractive
RUN sed -Ei 's/(archive|security)\.ubuntu\.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list && \
    apt-get update && \
    apt-get install -y \
        build-essential \
        automake autotools-dev \
        python3 \
        bison flex gawk bc \
        libtool patchutils \
        libmpc-dev libmpfr-dev libgmp-dev \
        zlib1g-dev libexpat-dev \
        texinfo gperf \
        && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY build.sh /build.sh
CMD ["/build.sh"]
