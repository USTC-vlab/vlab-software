FROM node:18

RUN apt-get update && apt-get -y install git && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY build.sh /build.sh
CMD ["/build.sh"]
