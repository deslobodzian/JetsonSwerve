//
// Created by prometheus on 5/12/23.
//

#include <SPI.hpp>

SPI::SPI() {
    spi_fd_ = open(SPI_DEVICE, O_RDWR);
    if (spi_fd_ < 0) {
        error("Failed to open SPI device");
    }

    /* Configure SPI mode */
    uint8_t spi_mode = SPI_MODE;
    if (ioctl(spi_fd_, SPI_IOC_WR_MODE, &spi_mode) == -1) {
        error("Failed to set SPI mode");
    }

    /* Configure bits per word */
    uint8_t spi_bits = SPI_BITS_PER_WORD;
    if (ioctl(spi_fd_, SPI_IOC_WR_BITS_PER_WORD, &spi_bits) == -1 ) {
        error("Failed to set SPI bits per word");
    }

    uint32_t spi_speed = SPI_SPEED_HZ;
    if (ioctl(spi_fd_, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) == -1) {
        error("Failed to set SPI speed");
    }
}

SPI::~SPI() {
    close(spi_fd_);
}

uint8_t SPI::transfer(uint8_t *tx_buffer, uint8_t *rx_buffer, size_t length) {
    struct spi_ioc_transfer transfer_data = {
            .tx_buf = (unsigned long)tx_buffer,
            .rx_buf = (unsigned long)rx_buffer,
            .len = static_cast<__u32>(length),
            .speed_hz = SPI_SPEED_HZ,
            .delay_usecs = 0,
            .bits_per_word = SPI_BITS_PER_WORD,
    };

    if (ioctl(spi_fd_, SPI_IOC_MESSAGE(1), &transfer_data) < 0) {
        error("SPI transfer failed");
        return -1;
    }
    return 0;
}
