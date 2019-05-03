## Hardware USART-library

## This library currently only supports the arduino DUE hardware USART interface

### Including this library

When including this library, add these lines to your module's Makefile.due:

```Makefile
#add USART_library
USART               ?= ../../../libraries/USART_library
include              $(USART)/Makefile.inc
```

Then you'll be able to include the library like so in your project files:

```c++
#include <hardware_usart.hpp>
```
## Using this library
When using this library it is first required you create an uartport like so
```c++
r2d2::usart_ports_c uart_port_one = r2d2::usart_ports_c::uart1;
```
uart1 corresponds to tx1 and rx1, uart2 to rx2 and tx2 etc.
uart0 is also used for programming and communicating with the PC that's why it is not possible to use uart0 with this library. 

To create and initialize a usart bus:
```c++
r2d2::hardware_usart_c usart_bus(9600, uart_port_one);
```
the first parameter is the baudrate, 9600 is used here as an example. Check which baudrate is applicable for you.

The bus can retreive and send chars with getc and putc
