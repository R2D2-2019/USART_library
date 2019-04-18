#pragma once
#include <usart_connection.hpp>
#include <uart_ports.hpp>
#include <queue.hpp>
#include <hwlib.hpp>

namespace r2d2 {

    class test_usart_c : public usart_connection_c {
    private:
        unsigned int boudrate;
        uart_ports_c usart_port;
        queue_c<uint8_t, 250> input_buffer;


    public:

        test_usart_c(unsigned int baudrate, uart_ports_c usart_port);

        /// @brief does not actualy disable anything
        void enable() override;

        /// @brief does not actualy disable anyting
        void disable() override;

        /// @brief sends byte
        /// @param c byte to send.. does not actualy send anything
        bool send(const uint8_t c) override;

        ///@brief sends char c
        ///@param c char to send
        void putc(char c) override;

        ///@brief returns byte
        ///@return uint8_t always 0xAA
        uint8_t receive() override;

        ///@brief returns true if char is available
        ///@return bool always true
        bool char_available() override;

        ///@brief returns receive()
        ///@return char uint8_t from recieve
        char getc() override;

        ///@brief returns 1
        ///@return unsigned int always 1
        unsigned int available() override;

    };
};