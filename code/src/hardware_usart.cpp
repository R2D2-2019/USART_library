#include <hardware_usart.hpp>
#include <uart_ports.hpp>
namespace r2d2 {
    enum class peripheral {
        peripheral_a,
        peripheral_b
    };

    struct hw_usart_s {
        Usart *usart;
        uint32_t rx_mask;
        uint32_t tx_mask;
        Pio * pio;
        peripheral periph;
        uint16_t id;
    };

    constexpr hw_usart_s usart[uint8_t(uart_ports_c::UART_SIZE)] = {
        {USART0, PIO_PA10A_RXD0, PIO_PA11A_TXD0, PIOA, peripheral::peripheral_a, ID_USART0},
        {USART1, PIO_PA12A_RXD1, PIO_PA13A_TXD1, PIOA, peripheral::peripheral_a, ID_USART1},
        {USART3, PIO_PD5B_RXD3, PIO_PD4B_TXD3, PIOD, peripheral::peripheral_b, ID_USART3}
    };

    hardware_usart_c::hardware_usart_c(unsigned int baudrate,
                                       uart_ports_c usart_port)
        : baudrate(baudrate) {

        if (usart_port == uart_ports_c::UART_SIZE){
            HWLIB_PANIC_WITH_LOCATION;
        }

        auto set_peripheral = [](Pio * pio, uint32_t mask, peripheral p) {
            uint32_t t = pio->PIO_ABSR;

            if (p == peripheral::peripheral_a) {
                pio->PIO_ABSR &= (~mask & t);
            } else {
                pio->PIO_ABSR = (mask | t);
            }

            // remove pin from pio controller
            pio->PIO_PDR = mask;
        };

        const auto &curr = usart[uint8_t(usart_port)];

        hardware_usart = curr.usart;
        set_peripheral(curr.pio, curr.rx_mask, curr.periph);
        set_peripheral(curr.pio, curr.tx_mask, curr.periph);
        PMC->PMC_PCER0 = (0x01 << curr.id);

        disable();

        hardware_usart->US_BRGR = (5241600u / baudrate);

        hardware_usart->US_MR =
            UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL | US_MR_CHRL_8_BIT;
        hardware_usart->US_IDR = 0xFFFFFFFF;

        enable();
    }

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

    hardware_usart_c &hardware_usart_c::operator<<(uint8_t byte) {
        send_byte(byte);
        return *this;
    }

    hardware_usart_c &hardware_usart_c::operator<<(const char *c) {
        for (const char *p = c; *p != '\0'; p++) {
            send_byte(*p);
        }
        return *this;
    }

    void hardware_usart_c::enable() {
        hardware_usart->US_CR = UART_CR_RXEN | UART_CR_TXEN;
    }

    void hardware_usart_c::disable() {
        hardware_usart->US_CR =
                UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
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
