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
