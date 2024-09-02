FROM php:8.3-zts-bullseye as builder

WORKDIR /app

# Install meson
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    python3-setuptools \
    python3-wheel \
    ninja-build
RUN pip3 install meson

COPY . .
RUN meson setup builddir
RUN meson compile -C builddir
RUN meson test -C builddir

FROM php:8.3-zts-bullseye
WORKDIR /usr/bin

COPY --from=builder /app/builddir/limos /usr/bin/limos
ENTRYPOINT ["/usr/bin/limos"]