#pragma once

#include <hwlib.hpp>
#include <ringbuffer.hpp>
#include <usart_connection.hpp>

namespace r2d2::usart {

    enum class usart_ports {
        uart1 = 0,
        uart2,
        uart3,
        // only for determining how many uart ports there are
        UART_SIZE
    };

    enum class peripheral {
        peripheral_a,
        peripheral_b
    };

    struct hw_usart_s {
        Usart *usart;
        uint32_t rx_mask;
        uint32_t tx_mask;
        Pio *pio;
        peripheral periph;
        uint16_t id;
    };

    const static hw_usart_s usart[uint8_t(usart_ports::UART_SIZE)] = {
            {USART0, PIO_PA10A_RXD0, PIO_PA11A_TXD0, PIOA, peripheral::peripheral_a, ID_USART0},
            {USART1, PIO_PA12A_RXD1, PIO_PA13A_TXD1, PIOA, peripheral::peripheral_a, ID_USART1},
            {USART3, PIO_PD5B_RXD3, PIO_PD4B_TXD3, PIOD, peripheral::peripheral_b, ID_USART3}
    };

    template <size_t BufferLength = 256>
    class hardware_usart_c : public usart_connection_c {
    private:
        Usart *hardware_usart = nullptr;
        unsigned int baudrate;
        ringbuffer_c<uint8_t, BufferLength> input_buffer;

        /// @brief check if the transmitter is ready to send
        /// @return true if ready to send, false if not ready to send
        bool transmit_ready() {
            return (hardware_usart->US_CSR & 2);
        }

        /// @brief send a byte via usart
        /// @param b: byte to send
        void send_byte(const uint8_t &b) {
            while (!transmit_ready()) {
            }
            hardware_usart->US_THR = b;
        }

        /// @brief receive a byte by reading the US_RHR register
        /// @return byte received
        uint8_t receive_byte() {
            return hardware_usart->US_RHR;
        }

        void set_peripheral(Pio *pio, uint32_t mask, peripheral p) {
            uint32_t t = pio->PIO_ABSR;

            if (p == peripheral::peripheral_a) {
                pio->PIO_ABSR &= (~mask & t);
            } else {
                pio->PIO_ABSR = (mask | t);
            }

            // remove pin from pio controller
            pio->PIO_PDR = mask;            
        }

    public:
        hardware_usart_c(unsigned int baudrate, usart_ports usart_port)
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
            for (const char *p = c; *p != '\0'; p++) {
                send_byte(*p);
            }
            return *this;
        }

        /// @brief enables the internal USART controller
        void enable() override {
            hardware_usart->US_CR = UART_CR_RXEN | UART_CR_TXEN;
        }

        /// @brief disables the internal USART controller
        void disable() override {
            hardware_usart->US_CR =
                    UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
        }

        /// @brief send single byte
        /// @param c: byte to send
        /// @return true if byte send, false if not send
        bool send(const uint8_t c) override {
           send_byte(c);
           return true;
        }

        /// @brief sends a char via usart
        /// @param c: char to send
        void putc(char c) override {
            send_byte(c);
        }

        /// @brief recieve byte bia usart
        /// @return received byte
        uint8_t receive() override {
            if (!input_buffer.size()) {
                return 0;
            }

            return input_buffer.copy_and_pop_front();
        }

        /// @brief check if char is available
        /// @return true if char is available false if not
        bool char_available() override {
            return (available() > 0);
        }

        /// @brief receive char via usart
        /// @return char received
        char getc() override {
            if (char_available()) {
                return receive();
            }
            return 0;
        }

        /// @brief returns amount of available data in buffer
        /// @return amount of uint8_t's in buffer
        unsigned int available() override {
           if ((hardware_usart->US_CSR & 1) != 0) {
                input_buffer.push(receive_byte());
            }

            return input_buffer.size();
        }

    };
}; // namespace r2d2::usart

