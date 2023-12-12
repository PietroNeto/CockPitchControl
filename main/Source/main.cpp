/** @file main.c
 * 
 * @brief módulo princial (MAIN)
 *
 * Autor Pietro Di Bernardo Neto
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "sdkconfig.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#include "canBUS.h"

extern "C" {
    #include "usbhid.h"
    #include "digitalin.h"
    #include "analogin.h"
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * @brief Faz a escrita no HID.
 *
 * @param void
 * @return void
 */
void vTaskHID(void *pvParameter)
{
    while (true)
    {
        // Faz a escrita no HID.
        writeToHID();

        // Pausa tarefa e passa a prioridade.
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/*!
 * @brief Faz a leitura dos periféricos.
 *
 * @param void
 * @return void
 */
void vTaskRead(void *pvParameter)
{   
    while (true)
    {
        // Faz a leitura das entradas Digitais.
        readDigital();

        // Faz a leitura das entradas Analógicas.
        // readAnalogIn();

        // Faz a leitura da rede CAN.
        readCAN();

        // Faz a escrita na rede CAN (somente slave).
        writeCAN();

        // Pausa tarefa e passa a prioridade.
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/*!
 * @brief MAIN.
 *
 * @param void
 * @return void
 */
extern "C" void app_main(void)
{
    // Inicialização dos módulos.
    //////////////////////////////////////////////////////////
    
    // Inicialização do HID.
    initHID();

    // Inicialização da CAN.
    InitCAN();

    // Inicialização do módulos de leitura Digital.
    InitDigitalIn();

    // Criação das tarefas.
    //////////////////////////////////////////////////////////
    
    // Cria e inicia a tarefa de escrita do HID
    xTaskCreate(&vTaskHID, "vTaskHID", 2048, NULL, 5, NULL);
    
    // Cria e inicia a tarefa das leituras
    xTaskCreate(&vTaskRead, "vTaskRead", 2048, NULL, 5, NULL);
}