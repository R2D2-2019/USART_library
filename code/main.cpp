#include <hwlib.hpp>

#include <hardware_usart.hpp>

int main(void) {
  // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    hwlib::cout << "this works on arduino";
    // nothing of use here

    r2d2::hardware_usart_c usart(115200, r2d2::uart_ports_c::uart1);

    uint16_t count = 0;

    for(;;){
        usart << "Test: " << count++ << '\n';
        hwlib::wait_ms(100);
    }
}
