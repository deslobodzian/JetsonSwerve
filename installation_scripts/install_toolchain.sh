#!/bin/bash

echo "Downloading "
mkdir ~/toolchain/
cd ~/toolchain/
wget https://toolchains.bootlin.com/downloads/releases/toolchains/aarch64/tarballs/aarch64--glibc--stable-2021.11-1.tar.bz2
tar -xf aarch64--glibc--stable-2021.11-1.tar.bz2
rm aarch64--glibc--stable-2021.11-1.tar.bz2
cd aarch64--glibc--stable-2021.11-1/

# Get the absolute path to the toolchain installation directory
TOOLCHAIN_DIR=$(pwd)

# Set up the environment variable for the toolchain path
echo "export JETSON_TOOLCHAIN=${TOOLCHAIN_DIR}" >> ~/.bashrc

# Create a CMake toolchain file for Jetson
TOOLCHAIN_FILE="${TOOLCHAIN_DIR}/JetsonToolchain.cmake"
INSTALL_PREFIX="${TOOLCHAIN_DIR}/aarch64-buildroot-linux-gnu/sysroot"

echo "export JETSON_TOOLCHAIN_FILE=\"${TOOLCHAIN_FILE}\"" >> ~/.bashrc
echo "export JETSON_INSTALL_PREFIX=\"${INSTALL_PREFIX}\"" >> ~/.bashrc

echo "Creating ${TOOLCHAIN_FILE}..."

cat << EOF > "${TOOLCHAIN_FILE}"
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR}/aarch64-buildroot-linux-gnu/sysroot)
set(CMAKE_SYSROOT ${TOOLCHAIN_DIR}/aarch64-buildroot-linux-gnu/sysroot)

set(CMAKE_TYPE_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER ${TOOLCHAIN_DIR}/bin/aarch64-buildroot-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_DIR}/bin/aarch64-buildroot-linux-gnu-g++)

if (NOT CMAKE_C_COMPILER OR NOT CMAKE_CXX_COMPILER)
    message(FATAL_ERROR "Can't find C/C++ cross compiler for Jetson")
endif()

set(CMAKE_AR ${TOOLCHAIN_DIR}/bin/aarch64-buildroot-linux-gnu-ar CACHE FILEPATH "" FORCE)
set(CMAKE_RANLIB ${TOOLCHAIN_DIR}/bin/aarch64-buildroot-linux-gnu-ranlib)
set(CMAKE_LINKER ${TOOLCHAIN_DIR}/bin/aarch64-buildroot-linux-gnu-ld)

set(CMAKE_EXE_LINKER_FLAGS_INIT -Wl,--allow-shlib-undefined)

set(CMAKE_CUDA_FLAGS "-ccbin \${CMAKE_CXX_COMPILER} -Xcompiler -fPIC" CACHE STRING "" FORCE)

EOF

source ~/.bashrc

echo "Installing libraries to toolchain..."
mkdir temp_libraries && cd temp_libraries
echo "Installing zlib..."
wget https://github.com/madler/zlib/releases/download/v1.2.13/zlib-1.2.13.tar.gz
tar -xf zlib-1.2.13.tar.gz
cd zlib-1.2.13
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_FILE}" -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" ..
make -j16
make install

echo "zlib installed, exiting folder"
cd ../..
echo "Installing OpenSSL..."

wget https://www.openssl.org/source/openssl-1.1.1l.tar.gz
tar -xf openssl-1.1.1l.tar.gz
cd openssl-1.1.1l

./Configure linux-aarch64 \
    --cross-compile-prefix=${TOOLCHAIN_DIR}/bin/aarch64-buildroot-linux-gnu- \
    --prefix=${INSTALL_PREFIX}

make -j16
make install

echo "Installed OpenSSL to toolchain, exiting folder"
cd ..
echo "Installing LIBSSH..."

wget https://git.libssh.org/projects/libssh.git/snapshot/libssh-0.9.7.tar.gz
tar -xf libssh-0.9.7.tar.gz
cd libssh-0.9.7/
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_FILE}" -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" ..
make -j16
make install
echo "Installed LIBSSH to toolchain, exiting folder"
cd ../..

echo "Installation complete, cleaning up files..."
cd ..
rm -r temp_libraries 
echo "Installation finished."

