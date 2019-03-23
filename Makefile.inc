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


# source files in this library
SOURCES += $(wildcard $(USART)/code/src/*.cpp)

# header files in this library
HEADERS += $(wildcard $(USART)/code/headers/*.hpp)

