#pragma once
#include <uart_connection.hpp>
#include <uart_ports.hpp>
#include <queue.hpp>
#include <hwlib.hpp>

namespace r2d2 {

    class test_usart_c : public usart_connection_c {
    private:
        unsigned int boudrate;
        uart_ports_c &usart_port;
        queue_c<uint8_t, 250> input_buffer;


    public:

        test_usart_c(unsigned int baudrate, uart_ports_c controller)

        void enable() override;

        void disable() override;

        bool send(const uint8_t c) override;

        void putc(char c) override;

        uint8_t receive() override;

        bool char_available() override;

        char getc() override;

        unsigned int available() override;

    };
};