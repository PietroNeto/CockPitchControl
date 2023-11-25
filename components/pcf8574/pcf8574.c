/** @file pcf8574.c
 * 
 * @brief Modulo de comunicação com IO expander PCF8574, configura a interface de comunicação com o 
 * chip usando interface I2C. 
 *
 * Autor Pietro Di Bernardo Neto
 */ 

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pcf8574.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////

// Tag da mensagem de log do módulo.
static const char *TAG_I2C = "I2C PCF8574";

////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////
void i2c_master_read_slave(i2c_port_t i2c_num, uint8_t *data_rd, size_t size, uint8_t slave_addr);
void i2c_master_init(void);

/*!
 * @brief Faz a leitura das entradas digitais dos expanders.
 *
 * @param void
 * @return void
 */
uint8_t ReadIOs(uint8_t slave_addr)
{
    // Variável que armazena o valor da leitura do I2C
    uint8_t I2CbyteRead;

    // Faz a Leitura de 1 Byte - PCF8574 tem apenas 8 portas, não é necessário mais do que 1 byte.
    i2c_master_read_slave(I2C_MASTER_NUM, *I2CbyteRead, 1, uint8_t slave_addr);

    // Retorna a leitura.
    return I2CbyteRead;
}

/*!
 * @brief Faz a leitura do barramento I2C para do endereço indicado.
 *
 * @param i2c_num Porta do barramento
 * @param data_rd Ponteiro para o local de salvamento.
 * @param size total de bytes a ser lido.
 * @param slave_addr endereço do dispositivo.
 * @return void
 */
void i2c_master_read_slave(i2c_port_t i2c_num, uint8_t *data_rd, size_t size, uint8_t slave_addr)
{
    // Variável que armazena o código de retorno dado pelo driver.
    int intCodeReturn = 0;

    // Cria o buffer de comunicação.
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    // Faz 10 tentativas de comunicação com o módulo. Caso não obtenha retorno, marca a flag de 
    // falha no bit referente ao chip lido.
    for(uint8_t i=0; i<10; i++)
    {
        // Inicia mensagem.
        i2c_master_start(cmd);

        // Envia o endereço do módulo.
        i2c_master_write_byte(cmd, (slave_addr << 1) | READ_BIT, ACK_CHECK_EN);

        // Se A mensagem for maior que 1 byte, lê até o penultimo enviando ACK.
        if (size > 1) {
            i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
        }

        // Lê o ultimo byte enviando NACK para encerrar  a comunicação.
        i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);

        // Finaliza mensagem.
        i2c_master_stop(cmd);
        
        // Dispara o I2C e salva o código de retorno.
        intCodeReturn = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_PERIOD_MS);
        
        // Verifica se a o dispositivo respondeu.
        if (intCodeReturn == ESP_OK)
        {
            // Comunicação Bem sucedida.
            ////////////////////////////

            break;
        }
    }
    
    // Verifica se ouve falha e escreve o resultado na variável global de leitura digital.
    if (intCodeReturn |= ESP_OK)
    {
        // Não foi possível obter retorno do dispositivo.
        /////////////////////////////////////////////////

        // Salva o erro na flag do dispositivo.
        guintDigitalPorts & PCF_ERROR_FLAG(slave_addr);

        // Envia um log de erro.
        ESP_LOGI(TAG_I2C, "Falha ao obter retorno de 0x%02X", slave_addr);
    }

    // Libera o barramento I2C.
    i2c_cmd_link_delete(cmd);
}



/*!
 * @brief Configura e inicia o barramento de comunicação do Dispositivo.
 *
 * @param void
 * @return void
 */
void InitDigitalRead(void)
{
    // Tenta iniciar o driver.
    i2c_master_init();
}