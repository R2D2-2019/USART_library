#pragma once
/**
 * @file usart_connection.hpp
 * @author Patrick Dekker
 * @brief base class for a usart implementation.
 * @version 0.1
 * @date 2019-05-24
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <cstdint>

namespace r2d2::usart {
    /**
     * @brief Base class for the usart implementation
     *
     */
    class usart_connection_c {
    public:
        /**
         * @brief operator << writes a byte with the usart
         *
         * @param byte to send
         * @return usart_connection_c&
         */
        usart_connection_c &operator<<(uint8_t byte);

        /**
         * @brief operator << writes a string with the usart
         *
         * @param c c-style string to send
         * @return usart_connection_c&
         */
        usart_connection_c &operator<<(const char *c);

        /**
         * @brief Write a uint8_t with the usart
         *
         * @param c data to send
         * @return true if the data is sended correctly
         * @return false if the data has not been sended correctly
         */
        virtual bool send(const uint8_t c) = 0;

        /**
         * @brief Get a data from the usart
         *
         * @return uint8_t
         */
        virtual uint8_t receive() = 0;

        /**
         * @brief returns how much data is available
         *
         * @return unsigned int amount of bytes
         */
        virtual unsigned int available() = 0;

        /**
         * @brief Write a char with the usart
         *
         * @param c
         */
        virtual void putc(char c) {
            send(static_cast<uint8_t>(c));
        }

        /**
         * @brief Check if data is available
         *
         * @return true if data is available
         * @return false if no data is available
         */
        virtual bool char_available() {
            return static_cast<bool>(available());
        }

        /**
         * @brief Get a char if available
         *
         * @return char
         */
        virtual char getc() {
            return static_cast<char>(receive());
        }
    };

} // namespace r2d2::usart
