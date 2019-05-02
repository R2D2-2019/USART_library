#pragma once

#include <hwlib.hpp>

namespace r2d2::usart {
    enum class uart_ports_c {
        uart0,
        uart1,
        uart2,
        uart3,
        // only for determining how many uart ports there are
        UART_SIZE
    };
} // namespace r2d2::usart
