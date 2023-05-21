#!/bin/bash
sudo apt-get install curl -y
sudo apt-get install python3 -y 
sudo apt-get install python3-pip -y
sudo apt-get install cmake -y
sudo apt-get install libssh-dev -y 
sudo apt install build-essential -y
pip install jinja2
sudo apt install gcc-10 gcc-10-base gcc-10-doc g++-10 -y
sudo apt install libstdc++-10-dev libstdc++-10-doc -y
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 10
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 10
