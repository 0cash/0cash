# 0cash
**Keep running wallet to strengthen the 0cash network. Backup your wallet in many locations & keep your coins wallet offline.**

### Ports:
- RPC port: 1837
- P2P port: 1847

Install
-----------------
### Linux

### [Quick guide for beginners](https://github.com/0cash/0cash/wiki/Quick-guide-for-beginners)

Install required dependencies:
```{r, engine='bash'}
sudo apt-get install \
      build-essential pkg-config libc6-dev m4 g++-multilib \
      autoconf libtool ncurses-dev unzip git python \
      zlib1g-dev wget bsdmainutils automake
```

Execute the build command:
```{r, engine='bash'}
# Clone Bitcoinz Repository
git clone https://github.com/0cash/0cash
# Build
cd 0cash/
./zcutil/build.sh -j$(nproc)
# fetch key
./zcutil/fetch-params.sh
```

Usage:
```{r, engine='bash'}
# Run
./src/0cashd
# Test getting information about the network
cd src/
./0cash-cli getmininginfo
# Test creating new transparent address
./0cash-cli getnewaddress
# Test creating new private address
./0cash-cli z_getnewaddress
# Test checking transparent balance
./0cash-cli getbalance
# Test checking total balance 
./0cash-cli z_gettotalbalance
# Check all available wallet commands
./0cash-cli help
# Get more info about a single wallet command
./0cash-cli help "The-command-you-want-to-learn-more-about"
./0cash-cli help "getbalance"
```

### Windows
The 0cash Windows Command Line Wallet can only be built from ubuntu for now.

Install required dependencies:
```
apt-get update \
&& apt-get install -y \
    curl build-essential pkg-config libc6-dev m4 g++-multilib autoconf \
    libtool ncurses-dev unzip git python zlib1g-dev wget bsdmainutils \
    automake p7zip-full pwgen mingw-w64 cmake
```

Execute the build command:
```
./zcutil/build-win.sh -j$(nproc)
```

### Docker

Build
```
$ docker build -t 0cash/0cash .
```

Create a data directory on your local drive and create a 0cash.conf config file
```
$ mkdir -p /ops/volumes/0cash/data
$ touch /ops/volumes/0cash/data/0cash.conf
$ chown -R 999:999 /ops/volumes/0cash/data
```

Create 0cash.conf config file and run the application
```
$ docker run -d --name 0cash-node \
  -v 0cash.conf:/0cash/data/0cash.conf \
  -p 1847:1847 -p 127.0.0.1:1837:1837 \
  0cash/0cash
```

Verify 0cash-node is running
```
$ docker ps
CONTAINER ID        IMAGE                  COMMAND                     CREATED             STATUS              PORTS                                              NAMES
31868a91456d        0cash/0cash            "0cashd --datadir=..."   2 hours ago         Up 2 hours          127.0.0.1:1837->1837/tcp, 0.0.0.0:1847->1847/tcp   0cash-node
```

Follow the logs
```
docker logs -f 0cash-node
```

The cli command is a wrapper to 0cash-cli that works with an already running Docker container
```
docker exec -it 0cash-node cli help
```

## Using a Dockerfile
If you'd like to have a production btc/0cash image with a pre-baked configuration
file, use of a Dockerfile is recommended:

```
FROM 0cash/0cash
COPY 0cash.conf /0cash/data/0cash.conf
```

Then, build with `docker build -t my-0cash .` and run.

### Windows
Windows build is maintained in [0cash-win project](https://github.com/0cash/0cash-win).

Security Warnings
-----------------

**0cash is experimental and a work-in-progress.** Use at your own risk.
