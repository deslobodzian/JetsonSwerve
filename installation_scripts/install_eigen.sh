#!/bin/bash

echo "Download Eigen from stable release... "

wget https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
echo "Extracting Eigen Linux"

tar -xzvf eigen-3.4.0.tar.gz

echo "Changing Directory to eigen"
cd eigen-3.4.0

echo "Building and configuring CMake"

mkdir build_eigen
cd build_eigen

cmake .. 
sudo make install -j16

echo "Installation complete removing temp files"

cd ..
cd ..
sudo rm -r eigen-3.4.0
rm eigen-3.4.0.tar.gz
