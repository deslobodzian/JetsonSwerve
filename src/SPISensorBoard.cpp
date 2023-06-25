//
// Created by prometheus on 5/12/23.
//

#include <SPISensorBoard.hpp>

SPISensorBoard::SPISensorBoard() {
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

SPISensorBoard::~SPISensorBoard() {
    close(spi_fd_);
}

void SPISensorBoard::decode_message(spi_data_t& data, const spi_buffer& rx_buf) {
    std::array<float, 2>* encoders[] = {
        &data.encoder_one,
        &data.encoder_two,
        &data.encoder_three,
        &data.encoder_four
    };

    auto it = rx_buf.cbegin();

    for (auto& encoder : encoders) {
        std::memcpy(encoder->data(), &(*it), encoder->size() * sizeof(float));
        std::advance(it, encoder->size() * sizeof(float));
    }
}

uint8_t SPISensorBoard::transfer_receive(spi_buffer* tx_buf, spi_buffer* rx_buf) {
    struct spi_ioc_transfer transfer_data = {
        .tx_buf = reinterpret_cast<__u64>(tx_buf->data()),
        .rx_buf = reinterpret_cast<__u64>(rx_buf->data()),
        .len = static_cast<__u32>(rx_buf->size()),
        .speed_hz = SPI_SPEED_HZ,
        .delay_usecs = 0,
        .bits_per_word = SPI_BITS_PER_WORD,
        .cs_change = 0,
        .tx_nbits = 0,
        .rx_nbits = 0,
        .word_delay_usecs = 0,
        .pad = 0,
    };

    if (ioctl(spi_fd_, SPI_IOC_MESSAGE(1), &transfer_data) < 0) {
        error("SPI transfer failed");
        return -1;
    }
    return 0;
}

uint8_t SPISensorBoard::receive(spi_data_t& data) {
    spi_buffer tx_buf{};
    spi_buffer rx_buf{};

    std::fill(tx_buf.begin(), tx_buf.end(), 0);
    std::fill(rx_buf.begin(), rx_buf.end(), 0);

    if(transfer_receive(&tx_buf, &rx_buf))  {
        error("Failed to receive data");
        return -1;
    }

    decode_message(data, rx_buf);
    return 0;
}

void SPISensorBoard::update_spi() {
    // std::lock_guard<std::mutex> lock(spi_data_mutex_);
    receive(spi_data_);
}

const spi_data_t& SPISensorBoard::get_spi_data() const {
    std::lock_guard<std::mutex> lock(spi_data_mutex_);
    return spi_data_;
}
void SPISensorBoard::set_spi_data(spi_data_t& data) {
    std::lock_guard<std::mutex> lock(spi_data_mutex_);
    spi_data_ = data;
}