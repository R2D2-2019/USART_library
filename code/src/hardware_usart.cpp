#include <hardware_usart.hpp>
#include <uart_ports.hpp>
namespace r2d2 {


    bool hardware_usart_c::transmit_ready() {
        return (hardware_usart->US_CSR & 2);
    }

    void hardware_usart_c::send_byte(const uint8_t &b) {
        while (!transmit_ready()) {
        }
        hardware_usart->US_THR = b;
    }

    uint8_t hardware_usart_c::receive_byte() {
        return hardware_usart->US_RHR;
    }

    hardware_usart_c::hardware_usart_c(unsigned int baudrate, uart_ports_c usart_port) :
        baudrate(baudrate),
        usart_port(usart_port)
    {
        if (usart_port == uart_ports_c::uart1) {

            hardware_usart = USART0;

            PIOA->PIO_PDR = PIO_PA10;
            PIOA->PIO_ABSR &= ~PIO_PA10;
            PIOA->PIO_PDR = PIO_PA11;
            PIOA->PIO_ABSR &= ~PIO_PA11;

            PMC->PMC_PCER0 = (0x01 << ID_USART0);
        } else if (usart_port == uart_ports_c::uart2) {
            hardware_usart = USART1;

            PIOA->PIO_PDR = PIO_PA12;
            PIOA->PIO_ABSR &= ~PIO_PA12;
            PIOA->PIO_PDR = PIO_PA13;
            PIOA->PIO_ABSR &= ~PIO_PA13;

            PMC->PMC_PCER0 = (0x01 << ID_USART1);
        } else if (usart_port == uart_ports_c::uart3) {

            hardware_usart = USART3;

            PIOD->PIO_PDR = PIO_PD4;
            PIOD->PIO_ABSR |= PIO_PD4;
            PIOD->PIO_PDR = PIO_PD5;
            PIOD->PIO_ABSR |= PIO_PD5;

            PMC->PMC_PCER0 = (0x01 << ID_USART3);
        } else if (usart_port == uart_ports_c::uart0) {

            // NO! its probably not a good idea to use this, it is already in
            // use by the hwlib::cout stuff hardware_usart = UART; PIOA->PIO_PDR
            // = PIO_PA8; PIOA->PIO_ABSR &= ~PIO_PA8; PIOA->PIO_PDR   = PIO_PA9;
            // PIOA->PIO_ABSR &= ~PIO_PA9;

            // PMC->PMC_PCER0 = ( 0x01 << ID_UART );*/
            usart_initialized = false;
        }

        disable();

        hardware_usart->US_BRGR = (5241600u / baudrate);

        hardware_usart->US_MR =
            UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL | US_MR_CHRL_8_BIT;
        hardware_usart->US_IDR = 0xFFFFFFFF;

        enable();
    }

    hardware_usart_c& hardware_usart_c::operator<<(uint8_t byte) {
        send_byte(byte);
        return *this;
    }

    hardware_usart_c& hardware_usart_c::operator<<(const char *c) {
        for (const char *p = c; *p != '\0'; p++) {
            send_byte(*p);
        }
        return *this;
    }

    void hardware_usart_c::enable() {
        if (hardware_usart) {
            hardware_usart->US_CR = UART_CR_RXEN | UART_CR_TXEN;
        }
    }

    void hardware_usart_c::disable() {
        if (hardware_usart) {
            hardware_usart->US_CR =
                UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
        }
    }

    bool hardware_usart_c::send(const uint8_t c) {
        send_byte(c);
        return true;
    }

    void hardware_usart_c::putc(char c) {
        send_byte(c);
    }

    uint8_t hardware_usart_c::receive() {
        if (!input_buffer.size()) {
            return 0;
        }

        return input_buffer.copy_and_pop();
    }

    bool hardware_usart_c::char_available() {
        return (available() > 0);
    }

    char hardware_usart_c::getc() {
        if (available() > 0) {
            return receive();
        }
        return 0;
    }

    unsigned int hardware_usart_c::available() {
        if ((hardware_usart->US_CSR & 1) != 0) {
            input_buffer.push(receive_byte());
        }

        return input_buffer.size();
    }
}; // namespace r2d2
