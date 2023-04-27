#!/bin/bash

echo "Installing Phoenix-Pro"

sudo curl -s --compressed -o /usr/share/keyrings/ctr-pubkey.gpg "https://deb.ctr-electronics.com/ctr-pubkey.gpg"
sudo curl -s --compressed -o /etc/apt/sources.list.d/ctr2023.list "https://deb.ctr-electronics.com/ctr2023.list"

sudo apt update
sudo apt install phoenix-pro

echo "Installed Phoenix-Pro"
