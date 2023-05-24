#!/bin/bash

echo "Downloading Toolchain from NVidia Website"
mkdir ~/toolchain/
cd ~/toolchain/
wget https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-a/10.3-2021.07/binrel/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu.tar.xz
tar -xf gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu.tar.xz
cd gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu

# Get the absolute path to the toolchain installation directory
TOOLCHAIN_DIR=$(pwd)

# Set up the environment variable for the toolchain path
echo "export JETSON_TOOLCHAIN=${TOOLCHAIN_DIR}" >> ~/.bashrc

# Create a CMake toolchain file for Jetson
echo "set(CMAKE_SYSTEM_NAME Linux)" > ${TOOLCHAIN_DIR}/JetsonToolchain.cmake
echo "set(CMAKE_SYSTEM_PROCESSOR aarch64)" >> ${TOOLCHAIN_DIR}/JetsonToolchain.cmake
echo "set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/aarch64-none-linux-gnu-gcc)" >> ${TOOLCHAIN_DIR}/JetsonToolchain.cmake
echo "set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/aarch64-none-linux-gnu-g++)" >> ${TOOLCHAIN_DIR}/JetsonToolchain.cmake

# Reload the .bashrc file to apply the environment variable
source ~/.bashrc
