#include <test_usart.hpp>

namespace r2d2 {

    test_usart_c::test_usart_c(unsigned int baudrate, uart_ports_c usart_port):
        baudrate(baudrate),
        usart_port(usart_port)
    {}

    void test_usart_c::enable(){
        //Not needed in test implementation
    }

    void test_usart_c::disable(){
        //Not needed in test implementation
    }

    bool test_usart_c::send(const uint8_t c) {
        return true;
        //Not needed in test implementation
    }

    void test_usart_c::putc(char c) {
        send(c);
    }

    uint8_t test_usart_c::receive() {
        return 0xAA;
    }

    bool test_usart_c::char_available() {
        return true;
    }

    char test_usart_c::getc() {
        return receive();
    }

    unsigned int test_usart_c::available() {
        return 1;
    }
};
