#include <hardware_usart.hpp>

namespace r2d2::usart {

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

    const static hw_usart_s usart[uint8_t(usart_ports::UART_SIZE)] = {
        {USART0, PIO_PA10A_RXD0, PIO_PA11A_TXD0, PIOA, peripheral::peripheral_a, ID_USART0},
        {USART1, PIO_PA12A_RXD1, PIO_PA13A_TXD1, PIOA, peripheral::peripheral_a, ID_USART1},
        {USART3, PIO_PD5B_RXD3, PIO_PD4B_TXD3, PIOD, peripheral::peripheral_b, ID_USART3}
    };

    void set_peripheral(Pio * pio, uint32_t mask, peripheral p) {
        uint32_t t = pio->PIO_ABSR;

        if (p == peripheral::peripheral_a) {
            pio->PIO_ABSR &= (~mask & t);
        } else {
            pio->PIO_ABSR = (mask | t);
        }

        // remove pin from pio controller
        pio->PIO_PDR = mask;
    };

    template <size_t buffer_length>
    hardware_usart_c<buffer_length>::hardware_usart_c(unsigned int baudrate,
                                       usart_ports usart_port)
        : baudrate(baudrate) {

        if (usart_port == usart_ports::UART_SIZE){
            HWLIB_PANIC_WITH_LOCATION;
        }

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

    template <size_t buffer_length>
    hardware_usart_c<buffer_length> &hardware_usart_c<buffer_length>::operator<<(uint8_t byte) {
        send_byte(byte);
        return *this;
    }

    template <size_t buffer_length>
    hardware_usart_c<buffer_length> &hardware_usart_c<buffer_length>::operator<<(const char *c) {
        for (const char *p = c; *p != '\0'; p++) {
            send_byte(*p);
        }
        return *this;
    }

    template <size_t buffer_length>
    bool hardware_usart_c<buffer_length>::transmit_ready() {
        return (hardware_usart->US_CSR & 2);
    }

    template <size_t buffer_length>
    void hardware_usart_c<buffer_length>::send_byte(const uint8_t &b) {
        while (!transmit_ready()) {
        }
        hardware_usart->US_THR = b;
    }

    template <size_t buffer_length>
    uint8_t hardware_usart_c<buffer_length>::receive_byte() {
        return hardware_usart->US_RHR;
    }

    template <size_t buffer_length>
    void hardware_usart_c<buffer_length>::enable() {
        hardware_usart->US_CR = UART_CR_RXEN | UART_CR_TXEN;
    }

    template <size_t buffer_length>
    void hardware_usart_c<buffer_length>::disable() {
        hardware_usart->US_CR =
                UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
    }

    template <size_t buffer_length>
    bool hardware_usart_c<buffer_length>::send(const uint8_t c) {
        send_byte(c);
        return true;
    }

    template <size_t buffer_length>
    void hardware_usart_c<buffer_length>::putc(char c) {
        send_byte(c);
    }

    template <size_t buffer_length>
    uint8_t hardware_usart_c<buffer_length>::receive() {
        if (!input_buffer.size()) {
            return 0;
        }

        return input_buffer.copy_and_pop();
    }

    template <size_t buffer_length>
    bool hardware_usart_c<buffer_length>::char_available() {
        return (available() > 0);
    }

    template <size_t buffer_length>
    char hardware_usart_c<buffer_length>::getc() {
        if (available() > 0) {
            return receive();
        }
        return 0;
    }

    template <size_t buffer_length>
    unsigned int hardware_usart_c<buffer_length>::available() {
        if ((hardware_usart->US_CSR & 1) != 0) {
            input_buffer.push(receive_byte());
        }

        return input_buffer.size();
    }
}; // namespace r2d2::usart
