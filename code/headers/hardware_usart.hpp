#pragma once

#include <hwlib.hpp>
#include <queue.hpp>
#include <usart_connection.hpp>

namespace r2d2::usart {

    enum class usart_ports {
        uart1 = 0,
        uart2,
        uart3,
        // only for determining how many uart ports there are
        UART_SIZE
    };

    template <size_t buffer_length = 250>
    class hardware_usart_c : public usart_connection_c {
    private:
        Usart *hardware_usart = nullptr;
        unsigned int baudrate;
        queue_c<uint8_t, buffer_length> input_buffer;

        /// @brief check if the transmitter is ready to send
        /// @return true if ready to send, false if not ready to send
        bool transmit_ready();

        /// @brief send a byte via usart
        /// @param b: byte to send
        void send_byte(const uint8_t &b);

        /// @brief receive a byte by reading the US_RHR register
        /// @return byte received
        uint8_t receive_byte();

    public:
        hardware_usart_c(unsigned int baudrate, usart_ports usart_port);
        /// @brief char output operator
        ///
        /// Although calling send_byte should do the exact same thing.
        /// In practice useing this fuction is more stable
        /// Especially when the values are repeated.
        hardware_usart_c &operator<<(uint8_t byte);

        /// @brief string output operator
        ///
        /// used for char arrays to quickly send more than one byte
        /// In practice useing this fuction is more stable than repeated use of
        /// send_byte() Especially when the values are repeated.
        hardware_usart_c &operator<<(const char *c);

        /// @brief enables the internal USART controller
        void enable() override;

        /// @brief disables the internal USART controller
        void disable() override;

        /// @brief send single byte
        /// @param c: byte to send
        /// @return true if byte send, false if not send
        bool send(const uint8_t c) override;

        /// @brief sends a char via usart
        /// @param c: char to send
        void putc(char c) override;

        /// @brief recieve byte bia usart
        /// @return received byte
        uint8_t receive() override;

        /// @brief check if char is available
        /// @return true if char is available false if not
        bool char_available() override;

        /// @brief receive char via usart
        /// @return char received
        char getc() override;

        /// @brief returns available data in buffer
        /// @return amount of uint8_t's in buffer
        unsigned int available() override;

    };
}; // namespace r2d2::usart
