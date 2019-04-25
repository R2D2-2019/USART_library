## Hardware USART-library

## This library currently only supports the arduino DUE hardware USART interface and a basic test interface for use with catch2

### Including this library

When including this library, add these lines to your module's Makefile.link:

```Makefile
#add USART_library
USART               ?= ../../../libraries/USART_library
include              $(USART)/Makefile.inc
```

Then you'll be able to include the library like so in your project files:
**Note:** always use the base class `usart_connection_c` as variable type and parameters for your classes,
 this way you can either use the test_usart_c for native applications or use the hardware_usart_c for embedded aplications

```c++
// include in class
#include <usart_connection.hpp>
// include in embedded main
#include <hardware_usart.hpp>
//include in catch/native main
#include <test_usart.hpp>
```
## Using this library
When using this library it is first required you create an uartport like so
```c++
r2d2::uart_ports_c uart_port_one = r2d2::uart_ports_c::uart1;
```
uart1 corresponds to tx1 and rx1, uart2 to rx2 and tx2 etc.
uart0 is also used for programming and communicating with the PC. It is reccomended you DO NOT use it. 

To create and initialize a usart bus:
```c++
r2d2::hardware_usart_c usart_bus(9600, uart_port_one);
```
the first parameter is the baudrate, 9600 is used here as an example. Check which baudrate is applicable for you. This aplication does not work if you use uart port zero.

The bus can retreive and send chars with getc and putc
