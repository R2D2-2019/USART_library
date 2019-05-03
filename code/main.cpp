#include <hwlib.hpp>

#include <hardware_usart.hpp>
#include <test_usart.hpp>

int main(void) {
  // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    hwlib::cout << "this works on arduino";
    auto hwusart = r2d2::hardware_usart_c(9600, r2d2::uart_ports_c::uart1);
    // nothing of use here
}
