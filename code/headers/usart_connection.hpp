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
#include <hwlib.hpp>

namespace r2d2::usart {
    /**
     * @brief Base class for the usart implementation
     *
     */
    class usart_connection_c : public hwlib::ostream, public hwlib::istream {
    public:
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
        virtual const unsigned int available() = 0;

        /**
         * @brief Write a char with the usart for hwlib ostream
         *
         * @param c
         */
        void putc(char c) override {
            send(static_cast<uint8_t>(c));
        }

        /**
         * @brief Check if data is available
         *
         * @return true if data is available
         * @return false if no data is available
         */
        virtual bool char_available() override {
            return static_cast<bool>(available());
        }

        /**
         * @brief Read and return a single character or '\0'
         * This function reads and returns a single character.
         * When no character is available it waits for one.
         *
         * @return char
         */
        virtual char getc() override {
            while (!char_available()) {
                // wait until a char is available
            }
            return static_cast<char>(receive());
        }

        /**
         * @brief flush override for hwlib::ostream
         *
         */
        void flush() override {
        }
    };

} // namespace r2d2::usart
