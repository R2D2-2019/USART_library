#pragma once
#include "hwlib.hpp"

namespace r2d2 {
    
    
    enum class uart_ports {
        uart0, uart1, uart2, uart3, 
        // only for determining how many uart ports there are
        UART_SIZE

    };


    /// @brief info about usart ports
    /// only for a possible future software usart/uart implementation
    struct uart_port_info_type_s {
        hwlib::target::pin_in rx;
        hwlib::target::pin_out tx;
    };

};
