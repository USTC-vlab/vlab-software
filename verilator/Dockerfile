FROM buildpack-deps:22.04-scm

ENV DEBIAN_FRONTEND=noninteractive
RUN sed -Ei 's/(archive|security)\.ubuntu\.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list && \
    apt-get update && \
    apt-get install -y build-essential git autoconf flex bison \
        help2man libfl-dev zlib1g-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY build.sh /build.sh
CMD ["/build.sh"]
