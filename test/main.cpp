#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <ostream>
#include <string>

#include <test_usart.hpp>

TEST_CASE("test_usart_c sends", "[test_usart_c]") {
    auto test_usart = r2d2::usart::test_usart_c();

    // Todo: implement the sending side of test_usart and add a test to test the
    // actual sending of the data and not for a true return
    test_usart.send(0xAA);
}

TEST_CASE("test_usart_c chars availiable ", "[test_usart_c]") {
    auto test_usart = r2d2::usart::test_usart_c();

    REQUIRE(!test_usart.char_available());

    test_usart.add_receive_byte(0x24);

    REQUIRE(test_usart.char_available() == true);
}

TEST_CASE("test_usart_c receives", "[test_usart_c]") {
    auto test_usart = r2d2::usart::test_usart_c();
    test_usart.add_receive_byte(0x25);

    REQUIRE(test_usart.receive() == 0x25);
}

TEST_CASE("test_usart_c is empty after receiving", "[test_usart_c]") {
    auto test_usart = r2d2::usart::test_usart_c();
    test_usart.add_receive_byte(0x25);
    test_usart.receive();
    REQUIRE(!test_usart.available());
}

TEST_CASE("set_receive_string()", "[test_usart_c]") {
    auto test_usart = r2d2::usart::test_usart_c();
    std::string str1 = "this is a test string";
    std::string str2;
    test_usart.set_receive_string(str1);
    while (test_usart.char_available()) {
        str2 += test_usart.getc();
    }
    REQUIRE(str1 == str2);
}

TEST_CASE("set_receive_bytes()", "[test_usart_c]") {
    auto test_usart = r2d2::usart::test_usart_c();
    std::vector<uint8_t> vec1 = {0xAA, 0x25, 0x01, 0x56};
    std::vector<uint8_t> vec2;
    test_usart.set_receive_bytes(vec1);
    while (test_usart.char_available()) {
        vec2.push_back(test_usart.receive());
    }
    REQUIRE(vec1 == vec2);
}

TEST_CASE("usart FIFO test", "[test_usart_c]") {
    auto test_usart = r2d2::usart::test_usart_c();

    test_usart.set_receive_bytes({0xAA, 0x25, 0x01, 0x56});

    REQUIRE(test_usart.receive() == 0xAA);
    REQUIRE(test_usart.receive() == 0x25);
    REQUIRE(test_usart.receive() == 0x01);
    REQUIRE(test_usart.receive() == 0x56);
}
