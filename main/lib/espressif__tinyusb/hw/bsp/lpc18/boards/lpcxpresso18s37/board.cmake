set(MCU_VARIANT LPC18S37)

set(JLINK_DEVICE LPC18S37)
set(PYOCD_TARGET LPC18S37)
set(NXPLINK_DEVICE LPC18S37:LPCXPRESSO18S37)

set(LD_FILE_GNU ${CMAKE_CURRENT_LIST_DIR}/lpc1837.ld)

function(update_board TARGET)
  # nothing to do
endfunction()
