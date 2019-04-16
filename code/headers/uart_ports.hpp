#pragma once
#include <hwlib.hpp>

namespace r2d2 {
    
    
    enum class uart_ports_c {
        uart0, uart1, uart2, uart3, 
        // only for determining how many uart ports there are
        UART_SIZE

    };


    //this code below is for another day, for a possible future software usart/uar implementation
/*
    /// @brief info about usart ports
    /// only for a possible future software usart/uart implementation
    struct uart_port_info_type_s {
        hwlib::target::pin_in rx;
        hwlib::target::pin_out tx;
    };

    const uart_port_info_type_s & uart_port_info_function(uart_ports_c name) {

        static const uart_port_info_type_s uart_port_info_array[(int)uart_ports_c::UART_SIZE ] = {
                {hwlib::target::pin_in(hwlib::target::pins::rx),
                 hwlib::target::pin_out(hwlib::target::pins::tx)},

                 {hwlib::target::pin_in(hwlib::target::pins::d19),
                 hwlib::target::pin_out(hwlib::target::pins::d18)},

                 {hwlib::target::pin_in(hwlib::target::pins::d17),
                 hwlib::target::pin_out(hwlib::target::pins::d16)},

                 {hwlib::target::pin_in(hwlib::target::pins::d15),
                  hwlib::target::pin_out(hwlib::target::pins::d14)},

        };

        uint_fast8_t n = static_cast<uint_fast8_t>(name);
        if (n >= static_cast<uint_fast8_t>(uart_ports_c::UART_SIZE)){
            return uart_port_info_array[3];
        }
        return uart_port_info_array[n];

    }*/

};

