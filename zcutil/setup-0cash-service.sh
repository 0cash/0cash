#!/bin/bash

if [ $# -eq 0 ]
then
    echo "0cash systemd unit setup."
    echo -e "Run:\n$0 user\nor install for current user\n$0 $USER"
    exit 1
fi

if id "$1" >/dev/null 2>&1
then
    echo "Installing 0cash service for $1 user..."
else
    echo -e "User $1 does not exist.\nTo add user run the following command:\nsudo adduser --disabled-password --gecos '' $1"
    exit 1
fi

cat > /tmp/config_setup.sh << EOF
#!/bin/bash
if ! [[ -d ~/.0cash ]]
then
    mkdir -p ~/.0cash
fi

if ! [[ -f ~/.0cash/0cash.conf ]]
then
    echo "rpcuser=rpc`pwgen 15 1`" > ~/.0cash/0cash.conf
    echo "rpcpassword=rpc`pwgen 15 1`" >> ~/.0cash/0cash.conf
fi
EOF
chmod +x /tmp/config_setup.sh
sudo -H -u $1 /tmp/config_setup.sh
sudo -H -u $1 ~/0cash-pkg/fetch-params.sh


cat > /etc/systemd/system/0cash.service << EOF
[Unit]
Description=0cash

[Service]
ExecStart=`cd ~; pwd`/0cash-pkg/0cashd
User=$1
Restart=always


[Install]
WantedBy=multi-user.target
EOF

systemctl daemon-reload
systemctl enable 0cash
systemctl start 0cash

systemctl status 0cash
