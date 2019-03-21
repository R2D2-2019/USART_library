
#include "hwlib.hpp"
#include <queue.hpp>
#include <uart_ports.hpp>
#pragma once

namespace r2d2 {

    class hardware_usart_c {
    private:
        Usart *hardware_usart = nullptr;
        unsigned int baudrate;
        bool usart_initialized = true;
        uart_ports_c &usart_port;
        queue_c<uint8_t, 250> input_buffer;

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
        hardware_usart_c(unsigned int baudrate, uart_ports_c &usart_port);

        /// @brief enables the internal USART controller
        void enable();

        /// @brief disables the internal USART controller
        void disable();

        /// @brief send single byte
        /// @param c: byte to send
        /// @return true if byte send, false if not send
        bool send(const uint8_t c);

        /// @brief sends a char via usart
        /// @param c: char to send
        void putc(char c);

        /// @brief recieve byte bia usart
        /// @return received byte
        uint8_t receive();

        /// @brief check if char is available
        /// @return true if char is available false if not
        bool char_available();

        /// @brief receive char via usart
        /// @return char received
        char getc();

        /// @brief returns available data in buffer
        /// @return amount of uint8_t's in buffer
        unsigned int available();




    };


};
