#include <test_usart.hpp>

namespace r2d2::usart {

    test_usart_c::test_usart_c(){}

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
        return receive_buffer.copy_and_pop();
    }

    bool test_usart_c::char_available() {
        return !receive_buffer.empty();
    }

    char test_usart_c::getc() {
        return receive();
    }

    unsigned int test_usart_c::available() {
        return receive_buffer.size();
    }

    void test_usart_c::set_receive_string(const std::string &str) {
        receive_buffer.clear();
        for(char c : str){
            add_receive_byte(c);
        }
    }

    void test_usart_c::set_receive_bytes(const std::vector<uint8_t> &bytes){
        receive_buffer.clear();
        for(auto &byte : bytes){
            add_receive_byte(byte);
        }
    }

    void test_usart_c::add_receive_byte(const uint8_t byte){
        receive_buffer.push(byte);
    }    
}; // namespace r2d2::usart