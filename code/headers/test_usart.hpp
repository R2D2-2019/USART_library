#pragma once

#include <vector>
#include <string>
#include <queue.hpp>
#include <usart_connection.hpp>

namespace r2d2::usart {

    template <size_t BufferLength = 250>
    class test_usart_c : public usart_connection_c {
    private:
        queue_c<uint8_t, BufferLength> receive_buffer;

    public:
        test_usart_c(){}

        /// @brief does not actualy enable anything
        void enable() override {
            //Not needed in test implementation
        }

        /// @brief does not actualy disable anyting
        void disable() override {
            //Not needed in test implementation
        }

        /// @brief sends byte
        /// @param c byte to send.. does not actualy send anything
        bool send(const uint8_t c) override {
            return true;
            //Not needed in test implementation
        }

        ///@brief sends char c
        ///@param c char to send
        void putc(char c) override {
            send(c);
        }


        ///@brief returns byte
        ///@return uint8_t the next item in the buffer
        uint8_t receive() override {
            return receive_buffer.copy_and_pop();
        }

        ///@brief returns true if char is available
        ///@return bool false if queue is empty, true if not
        bool char_available() override {
            return !receive_buffer.empty();
        }

        ///@brief returns receive()
        ///@return char uint8_t from recieve
        char getc() override {
            return receive();
        }

        ///@brief returns 1
        ///@return unsigned int the amount of bytes in queue
        unsigned int available() override {
            return receive_buffer.size();
        }

        /// @brief sets a string the test usart will return
        void set_receive_string(const std::string &str) {
            receive_buffer.clear();
            for(char c : str){
                add_receive_byte(c);
            }
        }

        /// @brief sets bytes the test usart will return
        /// @param bytes vector of bytes
        void set_receive_bytes(const std::vector<uint8_t> &bytes) {
            receive_buffer.clear();
            for(auto &byte : bytes){
                add_receive_byte(byte);
            }
        }

        /// @brief sets one byte the test usart will return
        /// @param byte to be returned in receive
        void add_receive_byte(const uint8_t byte) {
            receive_buffer.push(byte);
        }

    };
};
