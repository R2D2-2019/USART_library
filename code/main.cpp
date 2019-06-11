#include <hwlib.hpp>
#include <hardware_usart.hpp>

int main() {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000);
    hwlib::cout << "this works on arduino\n";

    auto usart = r2d2::usart::hardware_usart_c<r2d2::usart::usart0>(9600);

    char t = 'a';

    for (;;) {
        while (usart.available()) {
            hwlib::cout << static_cast<char>(usart.receive());
        }

        usart << "Test: " << t++ << '\n';

        if (t > 'z') {
            t = 'a';
        }

        hwlib::wait_ms(100);
    }
}
