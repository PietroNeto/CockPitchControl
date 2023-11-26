set(srcs
    "Source/analogin.c"
    "Source/canBUS.cpp"
    "Source/digitalin.c"
    "Source/digitalout.c"
    "Source/hardware.c"
    "Source/init.c"
    "Source/main.cpp"
    "Source/usbhid.c"
)

idf_component_register( 
    SRCS "${srcs}"
    INCLUDE_DIRS "Headers/"
    INCLUDE_DIRS "../components/mcp2515/"
    INCLUDE_DIRS "../components/pcf8574/"
    REQUIRES driver freertos
)