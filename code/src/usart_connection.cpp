#include "../headers/usart_connection.hpp"

namespace r2d2 {
    usart_connection_c &usart_connection_c::operator<<(uint8_t byte) {
        send(byte);
        return *this;
    }

    usart_connection_c &usart_connection_c::operator<<(const char *c) {
        for (const char *p = c; *p != '\0'; p++) {
            send(*p);
        }
        return *this;
    }
}