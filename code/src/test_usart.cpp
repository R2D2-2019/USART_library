#include <test_usart.hpp>

namespace r2d2::usart {

    template <size_t buffer_length>
    void test_usart_c<buffer_length>::enable(){
        //Not needed in test implementation
    }

    template <size_t buffer_length>
    void test_usart_c<buffer_length>::disable(){
        //Not needed in test implementation
    }

    template <size_t buffer_length>
    bool test_usart_c<buffer_length>::send(const uint8_t c) {
        return true;
        //Not needed in test implementation
    }

    template <size_t buffer_length>
    void test_usart_c<buffer_length>::putc(char c) {
        send(c);
    }

    template <size_t buffer_length>
    uint8_t test_usart_c<buffer_length>::receive() {
        return receive_buffer.copy_and_pop();
    }

    template <size_t buffer_length>
    bool test_usart_c<buffer_length>::char_available() {
        return !receive_buffer.empty();
    }

    template <size_t buffer_length>
    char test_usart_c<buffer_length>::getc() {
        return receive();
    }

    template <size_t buffer_length>
    unsigned int test_usart_c<buffer_length>::available() {
        return receive_buffer.size();
    }

    template <size_t buffer_length>
    void test_usart_c<buffer_length>::set_receive_string(const std::string &str) {
        receive_buffer.clear();
        for(char c : str){
            add_receive_byte(c);
        }
    }

    template <size_t buffer_length>
    void test_usart_c<buffer_length>::set_receive_bytes(const std::vector<uint8_t> &bytes){
        receive_buffer.clear();
        for(auto &byte : bytes){
            add_receive_byte(byte);
        }
    }

    template <size_t buffer_length>
    void test_usart_c<buffer_length>::add_receive_byte(const uint8_t byte){
        receive_buffer.push(byte);
    }    
}; // namespace r2d2::usart