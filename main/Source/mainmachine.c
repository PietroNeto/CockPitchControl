/** @file pcf8574.c
 * 
 * @brief Modulo de criação e gerenciamento das tarefas.
 *
 * Autor Pietro Di Bernardo Neto
 */ 

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "usbhid.h"
#include "digitalin.h"
#include "analogin.h"
#include "can.h"
////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////
// Tag da mensagem de log do módulo.
static const char *TAG_MM = "MAIN MACHINE";

////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES PRIVADAS DO MÓDULO
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
        readDigitalIn();

        // Faz a leitura das entradas Analógicas.
        readAnalogIn();

        // Faz a leitura da rede CAN.
        readCAN();

        // Pausa tarefa e passa a prioridade.
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}