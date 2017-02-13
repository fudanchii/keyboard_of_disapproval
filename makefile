#
#             LUFA Library
#     Copyright (C) Dean Camera, 2016.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = atmega32u4
ARCH         = AVR8
BOARD        = ARDUINO_MICRO
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = keyboard_of_disapproval
SRC          = $(TARGET).c arduino_micro/setup.c arduino_micro/descriptors.c arduino_micro/keymap.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
LUFA_PATH    = ./vendor/lufa/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/ -DBOARD_$(BOARD) -I./
LD_FLAGS     =

AVRDUDE_PROGRAMMER = avr109
AVRDUDE_PORT       = COM12
AVRDUDE_FLAGS      = -C H:\apps\arduino-1.8.1\hardware\tools\avr\etc\avrdude.conf

# Default target
all:

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH      ?= $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/doxygen.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/hid.mk
include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk
