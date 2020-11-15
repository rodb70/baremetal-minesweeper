# Makefile

CROSS_COMPILE := /opt/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-

COMPILER := gcc

#CPU := rpi1
CPU := host
BLD_TARGET := mine-sweeper
BLD_TYPE := debug

ifeq ($(CPU),rpi1)
PROJ_DIRS := rpi1
endif
ifeq ($(CPU),host)
PROJ_DIRS := host
EXTRA_LIBS := -lSDL2
SHORT_ENUMS := n

endif

PROJ_DIRS += src

include makefiles/main.mk

distclean:
	rm -rf build
