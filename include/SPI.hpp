//
// Created by prometheus on 5/12/23.
//

#pragma once

#include <iostream>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "utils.hpp"

#define SPI_DEVICE "/dev/spidev.0.0"
#define SPI_MODE SPI_MODE_0
#define SPI_BITS_PER_WORD 8
#define SPI_SPEED_HZ 1000000 // 1Mhz

class SPI {
public:
    SPI();
    ~SPI();

    uint8_t transfer(uint8_t* tx_buffer, uint8_t* rx_buffer, size_t length);

private:
    int spi_fd_;
};