# -------- base --------
# 26.04 LTS ships gcc 15.2 (sqlpp23 needs >= 14.2 for C++23) and cmake 4.2
# (we need >= 3.31), so no third-party apt repo is required for either.
FROM ubuntu:26.04 AS base

ENV DEBIAN_FRONTEND=noninteractive
ENV PATH="/root/.cargo/bin:${PATH}"

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
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
FROM ubuntu:26.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 \
    libmariadb3 \
    libsqlite3-0 \
    libreadline8t64 \
    ca-certificates \
    curl \
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