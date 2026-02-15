# -------- base --------
FROM ubuntu:24.04 AS base

ENV DEBIAN_FRONTEND=noninteractive
ENV PATH="/root/.cargo/bin:${PATH}"

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    gpg \
    wget \
    && rm -rf /var/lib/apt/lists/* \
    && test -f /usr/share/doc/kitware-archive-keyring/copyright || wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null \
    && echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ noble main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null \
    && apt-get update \
    && test -f /usr/share/doc/kitware-archive-keyring/copyright || rm /usr/share/keyrings/kitware-archive-keyring.gpg \
    && apt-get install kitware-archive-keyring \
    && apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    gdb \
    git \
    libreadline-dev \
    libmariadb-dev \
    libsqlite3-dev \
    python3-pyparsing \
    curl \
    && rm -rf /var/lib/apt/lists/* \
    && curl https://sh.rustup.rs -sSf | sh -s -- -y --profile minimal

RUN rustup default stable

WORKDIR /app

# -------- builder --------
FROM base AS builder

ARG BUILD_TYPE=Release

COPY . .

RUN cmake -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE}

RUN cmake --build build

# -------- runtime --------
FROM ubuntu:24.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 \
    libmariadb3 \
    libsqlite3-0 \
    libreadline8 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app/bin
COPY --from=builder /app/build/bin /app/bin
CMD [""]

FROM runtime AS login
CMD ["/app/bin/LoginServer"]

FROM runtime AS character
WORKDIR /app/bin
CMD ["/app/bin/CharServer"]

FROM runtime AS map
WORKDIR /app/bin
CMD ["/app/bin/MapServer"]

FROM runtime AS node
WORKDIR /app/bin
CMD ["/app/bin/NodeServer"]

# -------- dev container --------
FROM base AS dev

ARG BUILD_TYPE=Debug

RUN apt-get update && apt-get install -y --no-install-recommends \
    clang \
    lldb \
    && rm -rf /var/lib/apt/lists/*

CMD ["bash"]