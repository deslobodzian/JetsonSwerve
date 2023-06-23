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
#include <pthread.h>
#include <algorithm>
#include <array>
#include <utils.hpp>
#include <cstring>
#include <mutex>

#define SPI_DEVICE                      "/dev/spidev.0.0"
#define SPI_MODE                        SPI_MODE_0
#define SPI_BITS_PER_WORD               8
#define SPI_SPEED_HZ                    5000000 // 5Mhz
#define SPI_WORDS_PER_MESSAGE           32

using spi_buffer = std::array<uint8_t, SPI_WORDS_PER_MESSAGE>;

typedef struct {
    std::array<float, 2> encoder_one;
    std::array<float, 2> encoder_two;
    std::array<float, 2> encoder_three;
    std::array<float, 2> encoder_four;
} spi_data_t;

class SPISensorBoard {
public:
    SPISensorBoard();
    ~SPISensorBoard();
    void update_spi();
    const spi_data_t& get_spi_data() const;
    void set_spi_data(spi_data_t& data);
private:
    uint8_t receive(spi_data_t& data);
    void decode_message(spi_data_t& data, const spi_buffer& rx_buf);
    uint8_t transfer_receive(spi_buffer* tx_buf, spi_buffer* rx_buf);
    int spi_fd_;
    spi_data_t spi_data_;
    mutable std::mutex spi_data_mutex_;
};