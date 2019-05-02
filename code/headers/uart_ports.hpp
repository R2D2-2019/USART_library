#pragma once
#include <hwlib.hpp>

namespace r2d2 {

    enum class uart_ports_c : uint8_t {
        uart1 = 0,
        uart2 = 1,
        uart3 = 2,
        // only for determining how many uart ports there are
        UART_SIZE

    };
}; // namespace r2d2
