#pragma once

#include <vector>
#include <string>
#include <queue.hpp>
#include <usart_connection.hpp>

namespace r2d2::usart {

    template <size_t buffer_length = 250>
    class test_usart_c : public usart_connection_c {
    private:
        queue_c<uint8_t, 250> receive_buffer;

    public:
        test_usart_c(){}

        /// @brief does not actualy enable anything
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
        ///@return bool false if queue is empty, true if not
        bool char_available() override;

        ///@brief returns receive()
        ///@return char uint8_t from recieve
        char getc() override;

        ///@brief returns 1
        ///@return unsigned int the amount of bytes in queue
        unsigned int available() override;

        /// @brief sets a string the test usart will return
        void set_receive_string(const std::string &str);

        /// @brief sets bytes the test usart will return
        /// @param bytes vector of bytes
        void set_receive_bytes(const std::vector<uint8_t> &bytes);

        /// @brief sets one byte the test usart will return
        /// @param byte to be returned in receive
        void add_receive_byte(const uint8_t byte);

    };
};
