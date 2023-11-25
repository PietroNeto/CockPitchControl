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

////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////////////////////////

// Configurações do módulo SPI usado na comunicação com a CAN.
#define CAN_HOST     SPI2_HOST
#define QUEUE_SIZE   1
#define PIN_NUM_MISO 13
#define PIN_NUM_MOSI 11
#define PIN_NUM_CLK  12
#define PIN_NUM_CS   10

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

////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS GLOBAIS DO MODULO
////////////////////////////////////////////////////////////////////////////////////////////////////

// Variáveis de configuração do barramento SPI.
extern spi_device_handle_t spi;
extern spi_bus_config_t buscfg;
extern spi_device_interface_config_t devcfg;

// Variáveis de configuração do barramento I2C.
extern i2c_port_t i2c_master_port;
extern i2c_config_t conf;

////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES DE INTERFACE COM O MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////
void init_spi(void);
void i2c_master_init(void);

#endif /* HARDWARE_H */