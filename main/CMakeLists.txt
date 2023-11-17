set(srcs
    "Source/main.c"
    "Source/usbhid.c"
)

idf_component_register( 
    SRCS "${srcs}"
    INCLUDE_DIRS "Headers/"
    REQUIRES driver freertos
)