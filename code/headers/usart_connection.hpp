/// @file
/// @brief      base class for a usart implementation hardware, software and mock
/// @author     Patrick Dekker

#pragma once
#include <hwlib.hpp>

namespace r2d2 {

    /// @brief  base class for usart implementation
    class usart_connection_c {
    public:

        /// @brief write character
        /// @param c character to send
        virtual void uart_putc(char c) = 0;

        /// @brief operator << writes byte to usart
        /// @param byte byte to send
        usart_connection_c &operator<<(uint8_t byte);

        /// @brief operator << writes string to usart
        /// @param c c-style string to send
        usart_connection_c &operator<<(const char *c);

        /// @brief enables internal USART controler
        virtual void enable() = 0;

        /// @brief disables internal USART controler
        virtual void disable() = 0;

        /// @brief send byte using USART
        /// @param c byte to send
        virtual bool send(const uint8_t c) = 0;

        /// @brief send char using USART
        /// @param c char to send
        virtual void putc(char c) = 0;

        /// @brief receive byte from USART
        /// @return uint8_t byte received from USART
        virtual uint8_t receive() =0;

        /// @brief check if char is available
        /// @return true if byte is available false if not
        virtual bool char_available() = 0;

        /// @brief receive char if available
        /// @return char received
        virtual char getc() =0;

        /// @brief returns available data
        /// @return amount of bytes
        virtual unsigned int available() =0;
    };

};