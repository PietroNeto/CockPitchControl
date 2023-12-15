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
uint32_t gCANDigitalButtons;
uint32_t gAnalogEndDigitalButton;
uint32_t gIOXDigitalButtons;

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

    uint16_t mIOXRead = ((pcfDigitalRead1&IO_EXP_01_MASK)<<8) | (pcfDigitalRead2&IO_EXP_02_MASK);

    // Faz a leitura do Botão de emergencia.
    gudtUsbHidReport.Digital |= gpio_get_level(GPIO_NUM_36);
    
    // Mapeia os botões.
    if(mIOXRead&1)
    {
        gIOXDigitalButtons |= JSB_IO_1_DIR;
    }
    else
    {
        gIOXDigitalButtons &= ~JSB_IO_1_DIR;
    }
    if(mIOXRead&1<<1)
    { 
        gIOXDigitalButtons |= JSB_IO_2_DIR;
    }
    else
    {
        gIOXDigitalButtons &= ~JSB_IO_2_DIR;
    }
    if(mIOXRead&1<<2)
    { 
        gIOXDigitalButtons |= JSB_IO_3_DIR;
    }
    else
    {
        gIOXDigitalButtons &= ~JSB_IO_3_DIR;
    }
    if(mIOXRead&1<<3)
    { 
        gIOXDigitalButtons |= JSB_IO_4_DIR;
    }
    else
    {
        gIOXDigitalButtons &= ~JSB_IO_4_DIR;
    }
    if(mIOXRead&1<<4)
    { 
        gIOXDigitalButtons |= JSB_IO_5_DIR;
    }
    else
    {
        gIOXDigitalButtons &= ~JSB_IO_5_DIR;
    }
    if(mIOXRead&1<<8)
    { 
        gIOXDigitalButtons |= JSB_IO_1_ESQ;
    }
    else
    {
        gIOXDigitalButtons &= ~JSB_IO_1_ESQ;
    }
    if(mIOXRead&1<<9)
    { 
        gIOXDigitalButtons |= JSB_IO_2_ESQ;
    }
    else
    {
        gIOXDigitalButtons &= ~JSB_IO_2_ESQ;
    }
    if(mIOXRead&1<<10)
    { 
        gIOXDigitalButtons |= JSB_IO_3_ESQ;
    }
    else
    {
        gIOXDigitalButtons &= ~JSB_IO_3_ESQ;
    }
    if(mIOXRead&1<<12)
    { 
        gIOXDigitalButtons |= JSB_IO_4_ESQ;
    }
    else
    {
        gIOXDigitalButtons &= ~JSB_IO_4_ESQ;
    }
    if(mIOXRead&1<<13)
    { 
        gIOXDigitalButtons |= JSB_IO_5_ESQ;
    }
    else
    {
        gIOXDigitalButtons &= ~JSB_IO_5_ESQ;
    }
    
    // Extras.
    if(mIOXRead&1<<5)
    { 
        gIOXDigitalButtons |= BTN_EXT1;
    }
    else
    {
        gIOXDigitalButtons &= ~BTN_EXT1;
    }
    if(mIOXRead&1<<15)
    { 
        gIOXDigitalButtons |= BTN_EXT2;
    }
    else
    {
        gIOXDigitalButtons &= ~BTN_EXT2;
    }

    for(uint8_t i=0; i<32; i++)
    {
        if((gIOXDigitalButtons & 1<<i) || (gCANDigitalButtons & 1<<i) || (gAnalogEndDigitalButton & 1<<i))
        {
            gudtUsbHidReport.Digital |= 1<<i;
        }
        else
        {
            gudtUsbHidReport.Digital &= ~(1<<i);
        }
    }
}
