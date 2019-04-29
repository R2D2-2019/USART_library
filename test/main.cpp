#include "ostream"
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <test_usart.hpp>
#include <uart_ports.hpp>

TEST_CASE("test_usart_c sends","[test_usart_c]"){
    auto test_usart = r2d2::test_usart_c();

    REQUIRE(test_usart.send(0xAA));    
}

TEST_CASE("test_usart_c chars availiable ", "[test_usart_c]"){
    auto test_usart = r2d2::test_usart_c();
   
    REQUIRE(!test_usart.char_available());

    test_usart.set_receive_byte(0x24);

    REQUIRE(test_usart.char_available() == true);
}

TEST_CASE("test_usart_c receives", "[test_usart_c]"){
    auto test_usart = r2d2::test_usart_c();
    test_usart.set_receive_byte(0x25);

    REQUIRE(test_usart.receive() == 0x25);

}
TEST_CASE("test_usart_c is empty after receiving", "[test_usart_c]"){
    auto test_usart = r2d2::test_usart_c();
    test_usart.set_receive_byte(0x25);
    test_usart.receive();
    REQUIRE(!test_usart.available());
}
