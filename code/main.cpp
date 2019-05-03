#include <hwlib.hpp>

#include <hardware_usart.hpp>

int main(void) {
  // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    hwlib::cout << "this works on arduino";

    auto usart = r2d2::usart::hardware_usart_c(9600, r2d2::usart::usart_ports_c::uart1);

    uint8_t count = 0;

    for(;;){
        usart << "Test: " << count++ << '\n';
        hwlib::wait_ms(100);
    }
}
