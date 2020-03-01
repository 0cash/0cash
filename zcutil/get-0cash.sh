#!/bin/bash

sudo apt -y update
sudo apt-get install -y libc6-dev g++-multilib python p7zip-full pwgen jq curl
cd ~

if [ -f 0cash.zip ]
then
    rm 0cash.zip
fi
wget -O 0cash.zip `curl -s 'https://api.github.com/repos/0cash/0cash/releases/latest' | jq -r '.assets[].browser_download_url' | egrep "0cash.+x64.zip"`
7z x -y 0cash.zip
chmod -R a+x ~/0cash-pkg
rm 0cash.zip

cd ~/0cash-pkg
./fetch-params.sh

if ! [[ -d ~/.0cash ]]
then
    mkdir -p ~/.0cash
fi

if ! [[ -f ~/.0cash/0cash.conf ]]
then
    echo "rpcuser=rpc`pwgen 15 1`" > ~/.0cash/0cash.conf
    echo "rpcpassword=rpc`pwgen 15 1`" >> ~/.0cash/0cash.conf
fi

./0cashd
