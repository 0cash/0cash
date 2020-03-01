FROM debian:stretch-slim as build

# Install our build dependencies
RUN apt-get update \
  && apt-get install -y \
    curl \
    build-essential \
  	pkg-config \
  	libc6-dev \
  	m4 \
  	g++-multilib \
    autoconf \
  	libtool \
  	ncurses-dev \
  	unzip \
  	git \
  	python \
    zlib1g-dev \
  	wget \
  	bsdmainutils \
  	automake \
  	p7zip-full \
  	pwgen \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/local/src/
COPY . /usr/local/src/

RUN ./zcutil/build.sh -j$(nproc)
RUN ./zcutil/fetch-params.sh

FROM debian:stretch-slim

# Add our user and group first to ensure consistency
RUN groupadd -r 0cash && useradd -r -d /0cash -g 0cash 0cash

# Install our run dependencies
RUN apt-get update \
  && apt-get install -y \
    curl \
    libc6-dev \
    g++-multilib \
  && rm -rf /var/lib/apt/lists/*

# Setup application directory
RUN mkdir -p /0cash/data

# Copy binaries from build container
COPY --from=build /usr/local/src/src/0cashd /usr/local/bin
COPY --from=build /usr/local/src/src/0cash-cli /usr/local/bin
COPY --from=build /usr/local/src/src/zcash-gtest /usr/local/bin
COPY --from=build /usr/local/src/src/0cash-tx /usr/local/bin
COPY contrib/docker/cli /usr/local/bin

RUN chmod +x /usr/local/bin/cli

# Copy zcash params
COPY --from=build /root/.zcash-params /0cash/.zcash-params

RUN chown -R 0cash: /0cash
USER 0cash
WORKDIR /0cash
CMD ["0cashd", "-datadir=/0cash/data"]
