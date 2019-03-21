#include "hwlib.hpp"

#include <hardware_usart.hpp>


int main(void) {
  // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    hwlib::cout << "this works on arduino";
    r2d2::uart_ports_c port = r2d2::uart_ports_c::uart1;
    auto c = r2d2::hardware_usart_c(9600, port);
    char cc;
    char gps_buffer[100];
    bool complete_string = false;
    int n = 0;
    for (;;) {
        if (c.char_available()){
            cc = c.getc();
            if (cc == '$'){
                complete_string = true;
                n=0;
                gps_buffer[n] = cc;
            } else if (cc == '\n' || cc == '\r') {
                if (complete_string) {
                    hwlib::cout << gps_buffer << "\n";
                }
                complete_string = false;
            } else {
                n++;
                gps_buffer[n]  =cc;
            }
        }

    }

}
