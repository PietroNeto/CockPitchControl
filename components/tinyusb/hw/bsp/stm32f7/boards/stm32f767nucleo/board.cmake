set(MCU_VARIANT stm32f767xx)
set(JLINK_DEVICE stm32f767zi)

set(LD_FILE_GNU ${CMAKE_CURRENT_LIST_DIR}/STM32F767ZITx_FLASH.ld)

function(update_board TARGET)
  target_compile_definitions(${TARGET} PUBLIC
    STM32F767xx
    HSE_VALUE=8000000
    BOARD_TUD_RHPORT=0
    BOARD_TUD_MAX_SPEED=OPT_MODE_FULL_SPEED
    )
endfunction()
