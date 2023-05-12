#!/bin/bash

echo "Download WPILib from github release... "

wget https://github.com/wpilibsuite/allwpilib/archive/refs/tags/v2023.4.3.tar.gz
echo "Extracting WPILib Linux"

tar -xzvf v2023.4.3.tar.gz

echo "Changing Directory to WPILib"
cd allwpilib-2023.4.3

echo "Building and configuring CMake"

mkdir build_wpi
cd build_wpi

cmake .. -DWITH_JAVA=OFF -DWITH_SHARED_LIBS=OFF -DWITH_CSCORE=OFF -DWITH_NTCORE=ON -DWITH_WPIMATH=ON -DWITH_WPILIB=ON -DWITH_EXAMPLES=OFF -DWITH_TESTS=OFF -DWITH_GUI=OFF -DWITH_SIMULATION_MODULES=OFF

sudo make install -j16

echo "Installation complete removing temp files"

cd ..
cd ..
sudo rm -r allwpilib-2023.4.3
rm v2023.4.3.tar.gz
