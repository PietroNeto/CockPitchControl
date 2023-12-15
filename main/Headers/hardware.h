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
#include "sdkconfig.h"

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
//IO
#define JSB_IO_1_DIR				            (1<<5)
#define JSB_IO_2_DIR				            (1<<6)
#define JSB_IO_3_DIR				            (1<<7)
#define JSB_IO_4_DIR				            (1<<8)
#define JSB_IO_5_DIR				            (1<<9)
#define JSB_IO_1_ESQ				            (1<<15)
#define JSB_IO_2_ESQ				            (1<<16)
#define JSB_IO_3_ESQ				            (1<<17)
#define JSB_IO_4_ESQ				            (1<<18)
#define JSB_IO_5_ESQ				            (1<<19)
#define BTN_EXT1				                (1<<30)
#define BTN_EXT2				                (1<<31)
//CAN
#define JSB_CAN_1_DIR				            (1<<5)
#define JSB_CAN_2_DIR				            (1<<6)
#define JSB_CAN_3_DIR				            (1<<7)
#define JSB_CAN_4_DIR				            (1<<8)
#define JSB_CAN_5_DIR				            (1<<9)
#define JSB_CAN_1_ESQ				            (1<<15)
#define JSB_CAN_2_ESQ				            (1<<16)
#define JSB_CAN_3_ESQ				            (1<<17)
#define JSB_CAN_4_ESQ				            (1<<18)
#define JSB_CAN_5_ESQ				            (1<<19)
//ANALOG END DIGITAL BUTTON
#define JSB_ANALOG_1    				        (5)
#define JSB_ANALOG_2    				        (6)
#define JSB_ANALOG_3    				        (7)
#define JSB_ANALOG_4    				        (8)
#define JSB_ANALOG_5				            (15)
#define JSB_ANALOG_6				            (16)
#define JSB_ANALOG_7				            (17)
#define JSB_ANALOG_8				            (18)

#ifdef CONFIG_MASTER
#define DEFAULT_ANALOG_1                    0X0F
#define DEFAULT_ANALOG_2                    0X0F
#define DEFAULT_ANALOG_3                    0X0F
#define DEFAULT_ANALOG_4                    0X0F
#define DEFAULT_ANALOG_5                    0X0F
#define DEFAULT_ANALOG_6                    0X0F
#define DEFAULT_ANALOG_7                    0X0F
#define DEFAULT_ANALOG_8                    0X0F
#define IS_SLAVE                            false
#define TASK_CAN_TIME                       1
#elif CONFIG_SLAVE
#define DEFAULT_ANALOG_1                    0X00
#define DEFAULT_ANALOG_2                    0X00
#define DEFAULT_ANALOG_3                    0X00
#define DEFAULT_ANALOG_4                    0X00
#define DEFAULT_ANALOG_5                    0X00
#define DEFAULT_ANALOG_6                    0X00
#define DEFAULT_ANALOG_7                    0X00
#define DEFAULT_ANALOG_8                    0X00
#define IS_SLAVE                            true
#define TASK_CAN_TIME                       20
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS GLOBAIS DO MODULO
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES DE INTERFACE COM O MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////

#endif /* HARDWARE_H */