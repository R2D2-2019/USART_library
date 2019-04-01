#pragma once

#include "hwlib.hpp"
#include <queue.hpp>
#include <uart_ports.hpp>

namespace r2d2 {

class hardware_usart_c {
private:
  Usart *hardware_usart = nullptr;
  unsigned int baudrate;
  bool usart_initialized = true;
  uart_ports_c &usart_port;
  queue_c<uint8_t, 250> input_buffer;

  /// @brief check if the transmitter is ready to send
  /// @return true if ready to send, false if not ready to send
  bool transmit_ready();

  /// @brief send a byte via usart
  /// @param b: byte to send
  void send_byte(const uint8_t &b);

  /// @brief receive a byte by reading the US_RHR register
  /// @return byte received
  uint8_t receive_byte();

public:
  hardware_usart_c(unsigned int baudrate, uart_ports_c &usart_port);

  /// console character output function
  ///
  /// This is the function used for console (ostream) output.
  /// The embedded targets provide an implementation that writes
  /// to the serial port.
  ///
  /// This definition is weak, which allows
  /// an application to provide its own definition.
  void uart_putc(char c);

  /// @brief char output operator
  ///
  /// Although calling send_byte should do the exact same thing.
  /// In practice useing this fuction is more stable
  /// Especially when the values are repeated.
  hardware_usart_c &operator<<(uint8_t byte) {
    send_byte(byte);
    return *this;
  }

  /// @brief string output operator
  ///
  /// used for char arrays to quickly send more than one byte
  /// In practice useing this fuction is more stable than repeated use of
  /// send_byte() Especially when the values are repeated.
  hardware_usart_c &operator<<(const char *c) {
    hwlib::cout << c;
    for (const char *p = c; *p != '\0'; p++) {
      send_byte(*p);
    }
    return *this;
  }

  // WIP? Getc works fine.
  // char output operator
  // template<>
  //    basic_istream<char>&
  //    operator>>(basic_istream<char>& __is, basic_string<char>& __str);
  // hardware_usart_c &operator>>(std::string &output) {
  //  char c = getc();
  //  output += c;
  //  return *this;
  //}

  /// @brief enables the internal USART controller
  void enable();

  /// @brief disables the internal USART controller
  void disable();

  /// @brief send single byte
  /// @param c: byte to send
  /// @return true if byte send, false if not send
  bool send(const uint8_t c);

  /// @brief sends a char via usart
  /// @param c: char to send
  void putc(char c);

  /// @brief recieve byte bia usart
  /// @return received byte
  uint8_t receive();

  /// @brief check if char is available
  /// @return true if char is available false if not
  bool char_available();

  /// @brief receive char via usart
  /// @return char received
  char getc();

  /// @brief returns available data in buffer
  /// @return amount of uint8_t's in buffer
  unsigned int available();
};
}; // namespace r2d2
