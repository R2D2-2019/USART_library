#pragma once
/**
 * @file hardware_usart.hpp
 * @author Patrick Dekker
 * @brief Hardware implementation for the usart
 * @version 0.1
 * @date 2019-05-24
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <hwlib.hpp>
#include <ringbuffer.hpp>
#include <type_traits>

#include <usart_connection.hpp>

namespace r2d2::usart {
    namespace detail::pio {
        // helper structs for the peripherals
        struct periph_a {};
        struct periph_b {};

        // helper structs for the pio's
        struct pioa {};
        struct piod {};
        struct piob {};
    }; // namespace detail::pio

    /**
     * @brief Struct with the pin definitions for usart0.
     *
     * Usart0 tx is pin: D18.
     * Usart0 rx is pin: D19.
     *
     * With labels TX1 and RX1 on the arduino due.
     *
     */
    struct usart0 {
        constexpr static uint32_t rx = PIO_PA10A_RXD0;
        constexpr static uint32_t tx = PIO_PA11A_TXD0;

        using periph = detail::pio::periph_a;
        using pio = detail::pio::pioa;
    };

    /**
     * @brief Struct with the pin definitions for usart1.
     *
     * Usart1 tx is pin: D16.
     * Usart1 rx is pin: D17.
     *
     * With labels TX2 and RX2 on the arduino due.
     *
     */
    struct usart1 {
        constexpr static uint32_t rx = PIO_PA12A_RXD1;
        constexpr static uint32_t tx = PIO_PA13A_TXD1;

        using periph = detail::pio::periph_a;
        using pio = detail::pio::pioa;
    };

    /**
     * @brief Struct with the pin definitions for usart2.
     *
     * Usart2 tx is pin: A11.
     * Usart2 rx is pin: D52.
     *
     * This usart port doesn't have TX/RX labels for the pins.
     * Just A11 and 52.
     *
     */
    struct usart2 {
        constexpr static uint32_t rx = PIO_PB21A_RXD2;
        constexpr static uint32_t tx = PIO_PB20A_TXD2;

        using periph = detail::pio::periph_a;
        using pio = detail::pio::piob;
    };

    /**
     * @brief Struct with the pin definitions for usart3.
     *
     * Usart3 tx is pin: D14.
     * Usart3 rx is pin: D15.
     *
     * With labels TX3 and RX3 on the arduino due.
     *
     */
    struct usart3 {
        constexpr static uint32_t rx = PIO_PD5B_RXD3;
        constexpr static uint32_t tx = PIO_PD4B_TXD3;

        using periph = detail::pio::periph_b;
        using pio = detail::pio::piod;
    };

    namespace detail {
        namespace pio {
            // helper structs for getting the pointer to the pio
            template <typename PIO>
            Pio *const port = nullptr;

            template <>
            Pio *const port<pioa> = PIOA;

            template <>
            Pio *const port<piob> = PIOB;

            template <>
            Pio *const port<piod> = PIOD;
        } // namespace pio

        namespace usart {
            // helper structs for getting the pointer to the usart
            template <typename USART>
            Usart *const port = nullptr;

            template <>
            Usart *const port<usart0> = USART0;

            template <>
            Usart *const port<usart1> = USART1;

            template <>
            Usart *const port<usart2> = USART2;

            template <>
            Usart *const port<usart3> = USART3;
        } // namespace usart

        namespace peripheral_id {
            // helper structs for getting the usart id
            template <typename USART>
            constexpr uint32_t id = 0;

            template <>
            constexpr uint32_t id<usart0> = ID_USART0;

            template <>
            constexpr uint32_t id<usart1> = ID_USART1;

            template <>
            constexpr uint32_t id<usart2> = ID_USART2;

            template <>
            constexpr uint32_t id<usart3> = ID_USART3;
        } // namespace peripheral_id

    } // namespace detail

    // struct for storing divider information
    struct usart_divider {
        // Main divider for the baudrate
        uint16_t divider;

        // extra fraction to make the baud rate more precice
        uint8_t fraction;
    };

    template <typename Bus>
    class hardware_usart_c : public usart_connection_c {
    private:
        static inline ringbuffer_c<uint8_t, 256> input_buffer;

        /// @brief check if the transmitter is ready to send
        /// @return true if ready to send, false if not ready to send
        constexpr static bool transmit_ready() {
            return (detail::usart::port<Bus>->US_CSR & 2);
        }

        /// @brief receive a byte by reading the US_RHR register
        /// @return byte received
        constexpr static uint8_t receive_byte() {
            return detail::usart::port<Bus>->US_RHR;
        }

        template <uint32_t mask>
        constexpr void set_peripheral() {
            uint32_t curr_absr_mask =
                detail::pio::port<typename Bus::pio>->PIO_ABSR;

            if constexpr (std::is_same_v<typename Bus::periph,
                                         detail::pio::periph_a>) {
                detail::pio::port<typename Bus::pio>->PIO_ABSR =
                    (~mask & curr_absr_mask);
            } else {
                detail::pio::port<typename Bus::pio>->PIO_ABSR =
                    (mask | curr_absr_mask);
            }

            // remove pin from pio controller
            detail::pio::port<typename Bus::pio>->PIO_PDR = mask;
        };

        constexpr void enable() {
            detail::usart::port<Bus>->US_CR = UART_CR_RXEN | UART_CR_TXEN;
        }

        constexpr void disable() {
            detail::usart::port<Bus>->US_CR =
                UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
        }

        /**
         * @brief This function calculates the divider for the baudrate
         *
         * @warning this function will generate a error when used on runtime
         * as this uses floats as this function should be removed through
         * optimisation.
         *
         */
        constexpr usart_divider calculate_divider(unsigned int baudrate) const
            __attribute__((error("Runtime usage of divider_calculator"))) {

            // calculate the base divider and fp
            float divider = CHIP_FREQ_CPU_MAX / (16.f * baudrate);

            // calculate the cd
            uint16_t cd = static_cast<uint16_t>(divider);

            // get the data after the comma
            uint8_t fp = static_cast<uint8_t>(((divider - cd) + (1.f / 16.f)) * 8);

            return {cd, fp};
        }

    public:
        /**
         * @brief Construct a hardware usart c object using a divider and a
         * fraction.
         *
         * @detail To calculate these divider you can use the following
         * formula:
         *
         * divider baud rate only calculation:
         * baudrate = CHIP_FREQ_CPU_MAX / CD * 16
         *
         * converted for CD:
         * CD = CHIP_FREQ_CPU_MAX / 16 * baudrate
         *
         * Using a fractional baud rate calculation:
         * baudrate = CHIP_FREQ_CPU_MAX / (16 * (CD + FP/8)
         *
         * converted for CD, FP:
         * CD, FP = CHIP_FREQ_CPU_MAX / 16 * baudrate
         * (Before the comma is the CD and after the comma is a number that
         * needs to be converted to a step fom 0..7.)
         *
         * Where CHIP_FREQ_CPU_MAX is 84'000'000 (84Mhz)
         * CD = is the base divider. And FP is a number between 0..7.
         * Where 0 = 0/8, 1 = 1/8, 2 = 2/8, enz
         *
         * @param divider
         */
        hardware_usart_c(usart_divider divider) {
            // set the peripheral for the usart pins
            set_peripheral<Bus::rx>();
            set_peripheral<Bus::tx>();

            // enable the peripheral clock for the usart
            PMC->PMC_PCER0 = (0x01 << detail::peripheral_id::id<Bus>);

            // disable all interrupts for the usart
            detail::usart::port<Bus>->US_IDR = 0xFFFFFFFF;

            // clear the status register for old interrupts
            (void)detail::usart::port<Bus>->US_CSR;

            // use the peripheral_id since that is the same id as the IRQn_type
            constexpr auto iqrn =
                static_cast<IRQn_Type>(detail::peripheral_id::id<Bus>);

            // enable the nvic interrupt for the current usart
            NVIC_SetPriority(iqrn, 9);
            NVIC_EnableIRQ(iqrn);

            disable();

            // write the baud rate in the BRGR register
            detail::usart::port<Bus>->US_BRGR =
                (divider.divider | (divider.fraction & 0x7) << 16);

            // set the usart mode
            detail::usart::port<Bus>->US_MR =
                UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL | US_MR_CHRL_8_BIT;

            enable();

            // enable the interrupt for handling the receiving of data
            detail::usart::port<Bus>->US_IER = US_IER_RXRDY;
        }

        /**
         * @brief Construct a hardware usart c object. Calculates a divider
         * based on the the baud rate. Calculates the divider on compile time.
         * This will give an error when the calculate_divider is not optimized
         * away. That means it wont compile with -O0
         *
         * @param baudrate
         */
        hardware_usart_c(unsigned int baudrate)
            : hardware_usart_c(calculate_divider(baudrate)) {
        }

        /**
         * @brief Write a uint8_t with the usart
         *
         * @param c data to send
         *
         * @warning currenltly this will loop until the data is put in the
         * holding register.
         */
        void send(const uint8_t c) override {
            while (!transmit_ready()) {
                // wait until transmit is ready
            }

            // write the byte in the Transmit holding register
            detail::usart::port<Bus>->US_THR = c;
        }

        /**
         * @brief Get a data from the usart
         *
         * @return uint8_t
         *
         * @warning if you call this function without any data in the ringbuffer
         * then this will cause undefined behaviour.
         */
        uint8_t receive() override {
            return input_buffer.copy_and_pop_front();
        }

        /**
         * @brief returns how much data is available
         *
         * @return unsigned int amount of bytes
         */
        const unsigned int available() override {
            return input_buffer.size();
        }

        /**
         * @brief Interrupt handler for receiving data when the cpu is doing
         * other stuff. The user shouln't call this function as it will be
         * handled through the interrupt controller.
         *
         * @warning This adds the data into the ringbuffer and will overwrite
         * old data if not read once in a while.
         *
         */
        constexpr static void _isr_handler() {
            // read the status register
            uint32_t status = detail::usart::port<Bus>->US_CSR &
                              detail::usart::port<Bus>->US_IMR;

            // check if the rx ready bit is set
            if ((status & US_CSR_RXRDY)) {
                input_buffer.push(receive_byte());
            }
        }
    };
}; // namespace r2d2::usart

extern "C" {
void __USART0_Handler() {
    r2d2::usart::hardware_usart_c<r2d2::usart::usart0>::_isr_handler();
}

void __USART1_Handler() {
    r2d2::usart::hardware_usart_c<r2d2::usart::usart1>::_isr_handler();
}

void __USART2_Handler() {
    r2d2::usart::hardware_usart_c<r2d2::usart::usart2>::_isr_handler();
}

void __USART3_Handler() {
    r2d2::usart::hardware_usart_c<r2d2::usart::usart3>::_isr_handler();
}
}