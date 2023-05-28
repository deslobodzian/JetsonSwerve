#!/bin/bash

# Set the destination folder for the built libraries
LIB_DESTINATION="$(dirname "$(pwd)")/lib"

# Check if destination folder exists and if the .so files are present
if [ -d "$LIB_DESTINATION" ] && [ -f "$LIB_DESTINATION/libwpilibc.so" ] && [ -f "$LIB_DESTINATION/libntcore.so" ] && [ -f "$LIB_DESTINATION/libwpimath.so" ] && [ -f "$LIB_DESTINATION/libwpiutil.so" ]; then
    echo "WPILib libraries already exist in $LIB_DESTINATION. Skipping installation."
    exit 0
fi

USE_TOOLCHAIN=false

# Check if --use-toolchain flag is present
if [[ $* == *--use-toolchain* ]]; then
    USE_TOOLCHAIN=true
    echo "Using the toolchain for installation."
fi

wget https://github.com/wpilibsuite/allwpilib/archive/refs/tags/v2023.4.3.tar.gz
echo "Extracting WPILib Linux"

tar -xzvf v2023.4.3.tar.gz
rm v2023.4.3.tar.gz

echo "Changing Directory to WPILib"
cd allwpilib-2023.4.3

mkdir build_wpi
cd build_wpi

# Configure CMake with the toolchain and necessary options if --use-toolchain flag is present
if [ "$USE_TOOLCHAIN" = true ]; then
    cmake .. -DCMAKE_TOOLCHAIN_FILE=$JETSON_TOOLCHAIN_FILE -DCMAKE_INSTALL_PREFIX=$JETSON_INSTALL_PREFIX -DWITH_JAVA=OFF -DWITH_CSCORE=OFF -DWITH_NTCORE=ON -DWITH_WPIMATH=ON -DWITH_WPILIB=ON -DWITH_EXAMPLES=OFF -DWITH_TESTS=OFF -DWITH_GUI=OFF -DWITH_SIMULATION_MODULES=OFF
else
    cmake .. -DWITH_JAVA=OFF -DWITH_CSCORE=OFF -DWITH_NTCORE=ON -DWITH_WPIMATH=ON -DWITH_WPILIB=ON -DWITH_EXAMPLES=OFF -DWITH_TESTS=OFF -DWITH_GUI=OFF -DWITH_SIMULATION_MODULES=OFF
fi

# Build WPILib
make -j16
make install

# Create the destination folder for the libraries if it doesn't exist
# mkdir -p $LIB_DESTINATION

# Copy the shared object files to the lib folder
# cp -L ./lib/libwpilibc.so $LIB_DESTINATION
# cp -L ./lib/libntcore.so $LIB_DESTINATION
# cp -L ./lib/libwpimath.so $LIB_DESTINATION
# cp -L ./lib/libwpiutil.so $LIB_DESTINATION

# Cleanup
cd ../..
rm -rf allwpilib-2023.4.3

echo "WPILib build completed. Shared object files copied to $LIB_DESTINATION"

