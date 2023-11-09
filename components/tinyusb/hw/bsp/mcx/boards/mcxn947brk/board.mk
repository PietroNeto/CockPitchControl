MCU_VARIANT = MCXN947
MCU_CORE = MCXN947_cm33_core0
PORT ?= 1

CFLAGS += -DCPU_MCXN947VDF_cm33_core0

JLINK_DEVICE = MCXN947_M33_0
PYOCD_TARGET = MCXN947

# flash using pyocd
flash: flash-jlink
