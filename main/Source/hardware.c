/** @file hardware.c
 * 
 * @brief Configurações e inicializações das interfaces de hardware.
 *
 * Autor Pietro Di Bernardo Neto
 */ 

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "hardware.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////

spi_device_handle_t spi;

spi_bus_config_t buscfg={
    .mosi_io_num=PIN_NUM_MOSI,
    .miso_io_num=PIN_NUM_MISO,
    .sclk_io_num=PIN_NUM_CLK,
    .quadwp_io_num=-1,
    .quadhd_io_num=-1,
    .max_transfer_sz=QUEUE_SIZE
};

spi_device_interface_config_t devcfg={
    .mode=0,                                //SPI mode 0
    .clock_speed_hz=10*1000*1000,           //Clock out at 10 MHz
    .spics_io_num=PIN_NUM_CS,               //CS pin
    .queue_size=QUEUE_SIZE,                 //Queue transactions at a time
};

// Cria a porta de comunicação para o Master.
i2c_port_t i2c_master_port = I2C_MASTER_NUM;

// Monta a Struct de configuração do barramento.
i2c_config_t conf = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = I2C_MASTER_SDA_IO,
    .scl_io_num = I2C_MASTER_SCL_IO,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master = {
        .clk_speed = I2C_MASTER_FREQ_HZ,
    }
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * @brief Cria o driver do barramento de comunicação SPI.
 *
 * @param void
 * @return void
 */
void init_spi(void)
{
    esp_err_t ret;

    // Inicializa o SPI.
    ret=spi_bus_initialize(CAN_HOST, &buscfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    // Atribui o CAN ao barramento SPÌ
    ret=spi_bus_add_device(CAN_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG_SPI, "SPI CAN initialized successfully");
    
}

/*!
 * @brief Cria o driver do barramento de comunicação I2C.
 *
 * @param void
 * @return void
 */
void i2c_master_init(void)
{
    // Configura periférico.
    i2c_param_config(i2c_master_port, &conf);

    // Cria o driver e salva o código de retorno.
    i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}