#pragma once
/**
 * @file test_usart.hpp
 * @author Patrick Dekker
 * @brief Catch2 Test implementation for the usart
 * @version 0.1
 * @date 2019-05-24
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <string>
#include <vector>

#include <usart_connection.hpp>

namespace r2d2::usart {
    /**
     * @brief Usart implementation to test the usart with Catch2
     *
     */
    class test_usart_c : public usart_connection_c {
    private:
        std::vector<uint8_t> receive_buffer;
        std::vector<uint8_t> send_buffer;

    public:
        /**
         * @brief Write a uint8_t with the usart
         *
         * @param c data to send
         *
         * @warning doesnt send anything at the moment
         */
        void send(const uint8_t c) override {
            send_buffer.push_back(c);
        }

        /**
         * @brief Get a data from the usart
         *
         * @return uint8_t
         */
        uint8_t receive() override {
            auto r = receive_buffer.front();
            receive_buffer.erase(receive_buffer.begin());

            return r;
        }

        /**
         * @brief returns how much data is available
         *
         * @return unsigned int amount of bytes
         */
        const unsigned int available() override {
            return receive_buffer.size();
        }

        /**
         * @brief Set a string the test usart will return
         *
         * @param str
         */
        void set_receive_string(const std::string &str) {
            receive_buffer.clear();

            for (char c : str) {
                add_receive_byte(c);
            }
        }
        /**
         * @brief Set a string the test usart will return
         *
         * @param str
         */

        void set_send_string(const std::string &str) {
            send_buffer.clear();

            for (char c : str) {
                send(c);
            }
        }

        /**
         * @brief Set the bytes the test usart will return
         *
         * @param bytes
         */
        void set_receive_bytes(const std::vector<uint8_t> &bytes) {
            receive_buffer.clear();

            for (auto &byte : bytes) {
                add_receive_byte(byte);
            }
        }

        /**
         * @brief Add a byte to the data that the test usart will return
         *
         * @param byte
         */
        void add_receive_byte(const uint8_t byte) {
            receive_buffer.push_back(byte);
        }

        /**
         * @brief Add a byte to the send buffer that the test usart will return
         *
         * @param byte
         */
        void add_send_byte(const uint8_t byte) {
            send_buffer.push_back(byte);
        }

        /**
         * @brief returns the first byte that was added into the send buffer
         *
         */
        uint8_t get_send_byte() {
            auto byte = send_buffer.front();
            send_buffer.erase(send_buffer.begin());

            return byte;
        }
    };
}; // namespace r2d2::usart
