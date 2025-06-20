FROM ubuntu:24.04 AS build-env

RUN apt-get update && apt-get install --no-install-recommends -y \
    clang-format \
    clang-tidy \
    cmake \
    g++ \
    gcc \
    gcovr \
    make \
    pipx \
    python3-pip \
 && rm -rf /var/lib/apt/lists/*

RUN pipx install conan
RUN pipx ensurepath
RUN /root/.local/bin/conan profile detect

WORKDIR /app
