set(srcs
    "Source/analogin.c"
    "Source/canBUS.cpp"
    "Source/digitalin.c"
    "Source/digitalout.c"
    "Source/hardware.c"
    "Source/main.cpp"
    "Source/usbhid.c"
)

idf_component_register( 
    SRCS "${srcs}"
    INCLUDE_DIRS "Headers/"
    INCLUDE_DIRS "../components/mcp2515/"
    INCLUDE_DIRS "../components/pcf8574/"
    REQUIRES driver freertos nvs_flash
)

if("$ENV{SLAVE_BUILD}" STREQUAL "1")
    message(STATUS "Building for SLAVE")
    add_compile_definitions(CONFIG_SLAVE=1)
else()
    message(STATUS "Building for MASTER")
    add_compile_definitions(CONFIG_MASTER=1)
endif()