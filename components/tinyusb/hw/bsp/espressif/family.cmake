cmake_minimum_required(VERSION 3.5)

# Apply board specific content i.e IDF_TARGET must be set before project.cmake is included
include("${CMAKE_CURRENT_LIST_DIR}/boards/${BOARD}/board.cmake")

if(IDF_TARGET STREQUAL "esp32s2")
  set(FAMILY_MCUS ESP32S2)
elseif(IDF_TARGET STREQUAL "esp32s3")
  set(FAMILY_MCUS ESP32S3)
endif()

# Add example src and bsp directories
set(EXTRA_COMPONENT_DIRS "src" "${CMAKE_CURRENT_LIST_DIR}/boards" "${CMAKE_CURRENT_LIST_DIR}/components")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
