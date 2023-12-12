/** @file hardware.h
 * 
 * @brief Definições de hardware.
 *
 * Autor Pietro Di Bernardo Neto
 */ 

#ifndef HARDWARE_H
#define HARDWARE_H

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "driver/i2c.h"
#include "driver/spi_master.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////////////////////////

// Configurações do módulo SPI usado na comunicação com a CAN.
#define CAN_HOST                            SPI2_HOST
#define QUEUE_SIZE                          1
#define PIN_NUM_MISO                        13
#define PIN_NUM_MOSI                        11
#define PIN_NUM_CLK                         12
#define PIN_NUM_CS                          10

// Configurações do módulo I2C usdado na comunicação com o IOExpander.
#define I2C_MASTER_SCL_IO                   9
#define I2C_MASTER_SDA_IO                   8
#define I2C_MASTER_NUM                      (i2c_port_t)0
#define I2C_MASTER_FREQ_HZ                  40000
#define I2C_MASTER_TX_BUF_DISABLE           0
#define I2C_MASTER_RX_BUF_DISABLE           0
#define I2C_MASTER_TIMEOUT()                (1000/portTICK_PERIOD_MS)

// Endereços dos chips
#define IO_EXPANDER_01                      0x20
#define IO_EXPANDER_02                      0x21

// Endereçamento dos botões do HID
#define JSB1_DIR				            (1<<5)
#define JSB2_DIR				            (1<<6)
#define JSB3_DIR				            (1<<7)
#define JSB4_DIR				            (1<<8)
#define JSB5_DIR				            (1<<9)
#define JSB1_ESQ				            (1<<15)
#define JSB2_ESQ				            (1<<16)
#define JSB3_ESQ				            (1<<17)
#define JSB4_ESQ				            (1<<18)
#define JSB5_ESQ				            (1<<19)
#define BTN_EXT1				            (1<<30)
#define BTN_EXT2				            (1<<31)


////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS GLOBAIS DO MODULO
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES DE INTERFACE COM O MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////

#endif /* HARDWARE_H */