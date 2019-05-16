#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

SEARCH  += $(USART)/code/headers
SEARCH  += $(USART)/code/src


SOURCES += $(USART)/code/src/usart_connection.cpp
HEADERS += $(USART)/code/headers/usart_connection.hpp

ifeq ($(TARGET), native)
	# header files in this library
	HEADERS += $(USART)/code/headers/test_usart.hpp
	
else
	# header files in this library
	HEADERS += $(USART)/code/headers/hardware_usart.hpp

endif

# search for queue.hpp
SEARCH  += $(USART)/../datastructures/code/headers
HEADERS += $(wildcard $(USART)/../datastructures/code/headers/queue.hpp)
