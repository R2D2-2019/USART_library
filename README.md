## Hardware USART-library
### Including this library

When including this library, add these lines to your module's `Makefile.link`:

```Makefile
#add USART_library
USART               ?= ../../../libraries/USART_library
include              $(USART)/Makefile.inc
```

Though be sure you add those lines above these lines:
```Makefile
# Defer to the bmptk/Makefile.inc to do the work
include $(BMPTK)/Makefile.inc
```

Then you'll be able to include the library like so in your project files:
**Note:** always use the base class `usart_connection_c` as variable type and parameters for your classes, this way you can either use the test_usart_c for native applications or use the hardware_usart_c for embedded aplications

## Using this library

### Using the usart connection

The usart connection class is a child of ostream and istream. Thanks to this we can use the `<<` and the `>>` operators to send or receive bytes. If you want more control what to do with the data, you can use `available()` to check how much data is available, receive data with `receive()` and send data with `send()`. When using `receive()` always make sure there is data available! Not checking will cause Undefined Behaviour when no data is available.

### Using the hardware usart

First we need to construct the bus we can do this as follows:
```cpp
#include <hardware_usart.hpp>

auto usart = r2d2::usart::hardware_usart_c<r2d2::usart::usart0>(9600);
```
The template parameter in the class is what usart port you want to use. **Warning** this is not the same port as the labels on the Arduino Due. The sam3x8e chip has 4 hardware usart ports, 3 of these ports have labels. The other port doesn't have a label. See the Doxygen or hardware_usart.hpp for the pin definitions.

The first parameter is the baudrate, 9600 is used here as an example. Check which baudrate is applicable for you. If there are multiple objects using the same usart the object that was constructed as last will set the baud rate, as this is set every time the constructor is called.


## Some examples

Example of using the usart connection with a class that needs a usart:
```cpp
// include in class
#include <usart_connection.hpp>

class hello_world_c {
    private:
        using usart_c = r2d2::usart::usart_connection_c;

        usart_c &usart;

        char data;

    public:
        hello_world_c(usart_c &usart) :
            usart(usart), data(' ')
        {}

        void print() {
            usart << "Hello world\n" << data << "\n";
        }

        void receive() {
            // the operator >> will wait until data is received. 
            // Currently there is no timeout. If this is not the 
            // behaviour you want you can use char_available() or
            // available() to check there is data in the receive 
            // buffer.
            usart >> data;
        }

        char get_char() {
            return data;
        }
};
```

Example of using the hardware usart with a class that needs usart:
```cpp
// include in embedded main
#include <hardware_usart.hpp>

int main() {
    // Setup main clock
    hwlib::wait_ms(1);

    // create a usart object 
    auto usart = r2d2::usart::hardware_usart_c<r2d2::usart::usart0>(9600);

    // give the object to the hello_world_c class
    auto hello_world = hello_world_c(usart);

    // print something on the usart bus
    hello_world.print();
}

```

Example of using the test usart to test a class that needs to receive data:
```cpp
//include in catch/native main
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <test_usart.hpp>

TEST_CASE("hello world receives", "[hello_world]") {
    // create a test usart object
    auto usart = r2d2::usart::test_usart_c();

    // add a byte to the recieve buffer
    usart.add_receive_byte('A');

    // give the object to the hello_world_c class
    auto hello_world = hello_world_c(usart);

    // receive the data in the hello_world_c class
    hello_world.receive();

    // check if the data is gone in the receive buffer 
    // and in the data of the hello_world_c class
    REQUIRE(usart.availabe() == 0);
    REQUIRE(usart.get_char() == 'A');
}
```