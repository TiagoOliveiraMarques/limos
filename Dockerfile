FROM php:8.3-zts-bullseye as builder


WORKDIR /app

RUN apt-get update && apt-get install -y \
    cmake

COPY . .
RUN cmake .
RUN cmake --build .
RUN ctest

FROM php:8.3-zts-bullseye
WORKDIR /usr/bin

COPY --from=builder /app/limos /usr/bin/limos
ENTRYPOINT ["/usr/bin/limos"]