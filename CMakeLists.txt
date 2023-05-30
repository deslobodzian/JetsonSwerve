# Disable in-source builds to prevent source tree corruption
if ("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()
cmake_minimum_required(VERSION 3.6)
project(JetsonSwerve)

# Set the compiler options for the device
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -pthread -Wno-psabi")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Og")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(THREADS_PREFER_PTHREAD_FLAG ON)

# Find required packages for development
#find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(wpimath REQUIRED)
find_package(phoenix-pro REQUIRED)

# Set the source files for the development build
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Set the header files for the development build
file(GLOB_RECURSE HDR_FILES include/*.hpp)

add_executable(${PROJECT_NAME} ${HDR_FILES} ${SRC_FILES})
# Include the toolchain's sysroot for library search paths
link_directories(
        /home/prometheus/toolchain/aarch64--glibc--stable-2021.11-1/aarch64-buildroot-linux-gnu/sysroot/usr/lib/phoenix-pro
)

# Include the toolchain's sysroot for header search paths
target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        /home/prometheus/toolchain/aarch64--glibc--stable-2021.11-1/aarch64-buildroot-linux-gnu/sysroot/usr/include/phoenix-pro
)
# Link required libraries for the deployment build
target_link_libraries(
        ${PROJECT_NAME}
        phoenix-pro
        ntcore
        wpimath
        wpiutil
)

# Print the location of the linked phoenix-pro library
message("phoenix-pro library path: ${phoenix-pro_LIBRARIES}")

