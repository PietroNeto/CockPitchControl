/** @file digitalin.c
 * 
 * @brief Modulo de leitura das entradas de digital.
 *
 * Autor Pietro Di Bernardo Neto
 */ 

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "digitalin.h"
#include "usbhid.h"
#include "pcf8574.h"
#include "hardware.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////

void InitDigitalIn(void)
{
    // Inicia módulo de leitura dos IOexps.
    InitDigitalRead();

    // Configura porta de leitura do botão de emergencia.
	gpio_set_direction(GPIO_NUM_36, GPIO_MODE_INPUT);
}

void readDigital(void)
{
    // Faz a leitura do IOexp 1.
    uint8_t pcfDigitalRead1 = ReadIOs(IO_EXPANDER_01);

    // Faz a leitura do IOexp 2.
    uint8_t pcfDigitalRead2 = ReadIOs(IO_EXPANDER_02);

    uint16_t buttons = ((pcfDigitalRead1&IO_EXP_01_MASK)<<8) | (pcfDigitalRead2&IO_EXP_02_MASK);

    // Faz a leitura do Botão de emergencia.
    gudtUsbHidReport.Digital |= gpio_get_level(GPIO_NUM_36);
    
    // Mapeia os botões.
    if(buttons&1    ) gudtUsbHidReport.Digital |= JSB1_DIR;
    if(buttons&1<<1 ) gudtUsbHidReport.Digital |= JSB2_DIR;
    if(buttons&1<<2 ) gudtUsbHidReport.Digital |= JSB3_DIR;
    if(buttons&1<<3 ) gudtUsbHidReport.Digital |= JSB4_DIR;
    if(buttons&1<<4 ) gudtUsbHidReport.Digital |= JSB5_DIR;
    if(buttons&1<<8 ) gudtUsbHidReport.Digital |= JSB1_ESQ;
    if(buttons&1<<9 ) gudtUsbHidReport.Digital |= JSB2_ESQ;
    if(buttons&1<<10) gudtUsbHidReport.Digital |= JSB3_ESQ;
    if(buttons&1<<12) gudtUsbHidReport.Digital |= JSB4_ESQ;
    if(buttons&1<<13) gudtUsbHidReport.Digital |= JSB5_ESQ;
    
    // Extras.
    if(buttons&1<<5 ) gudtUsbHidReport.Digital |= BTN_EXT1;
    if(buttons&1<<15) gudtUsbHidReport.Digital |= BTN_EXT2;
}
