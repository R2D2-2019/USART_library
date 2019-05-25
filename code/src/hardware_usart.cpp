#include <hardware_usart.hpp>

namespace r2d2::usart {

    void set_peripheral(Pio *pio, uint32_t mask, peripheral p) {
        uint32_t t = pio->PIO_ABSR;

        if (p == peripheral::peripheral_a) {
            pio->PIO_ABSR &= (~mask & t);
        } else {
            pio->PIO_ABSR = (mask | t);
        }

        // remove pin from pio controller
        pio->PIO_PDR = mask;
    };
}

extern "C"{
void __USART0_Handler(){

}

void __USART1_Handler(){

}

void __USART3_Handler(){

}
}