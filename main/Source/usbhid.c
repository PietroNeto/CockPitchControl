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

////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////////////////////////

// VID
#define CONFIG_TINYUSB_DESC_CUSTOM_VID              0x1234

// PID
#define CONFIG_TINYUSB_DESC_CUSTOM_PID              0x5678

// ID do dispositivo HID.
#define HID_DEVICE_ID                       ( 0x0A )

// Definição da estrutura do report do protocolo HID.
#define TUD_HID_REPORT_DESC_TELEOP(...)     \
HID_USAGE_PAGE                              ( HID_USAGE_PAGE_DESKTOP     ),\
HID_USAGE                                   ( HID_USAGE_DESKTOP_GAMEPAD  ),\
HID_COLLECTION                              ( HID_COLLECTION_APPLICATION ),\
    /* Report ID if any */\
    __VA_ARGS__ \
    /* 8 bit Analogs (min -127, max 127 ) */ \
    HID_USAGE_PAGE                          ( HID_USAGE_PAGE_DESKTOP),\
    HID_USAGE                               ( 48 ) ,\
    HID_USAGE                               ( 49 ) ,\
    HID_USAGE                               ( 50 ) ,\
    HID_USAGE                               ( 51 ) ,\
    HID_USAGE                               ( 52 ) ,\
    HID_USAGE                               ( 53 ) ,\
    HID_USAGE                               ( 54 ) ,\
    HID_USAGE                               ( 55 ) ,\
    HID_LOGICAL_MIN                         ( 0x81 ) ,\
    HID_LOGICAL_MAX                         ( 0x7f ) ,\
    HID_REPORT_COUNT                        ( 8 ) ,\
    HID_REPORT_SIZE                         ( 8 ) ,\
    HID_INPUT                               ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
    /* 32 bit Button Map */ \
    HID_USAGE_PAGE                          ( HID_USAGE_PAGE_BUTTON) ,\
    HID_USAGE_MIN                           ( 1 ) ,\
    HID_USAGE_MAX                           ( 32 ) ,\
    HID_LOGICAL_MIN                         ( 0 ) ,\
    HID_LOGICAL_MAX                         ( 1 ) ,\
    HID_REPORT_COUNT                        ( 32 ) ,\
    HID_REPORT_SIZE                         ( 1 ) ,\
    HID_INPUT                               ( HID_DATA | HID_VARIABLE | HID_ABSOLUTE ) ,\
HID_COLLECTION_END

// Calculo do tamanho total do descriptor.
#define TUSB_DESC_TOTAL_LEN     (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN)

////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////

// Tag da mensagem de log do módulo.
static const char *TAG_UH = "USB HID";

// Armazena a descrição do report usado no protocolo HID.
const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_TELEOP(HID_REPORT_ID(HID_DEVICE_ID) )
};

// Armazena a descrição do dipositivo usado no protocolo HID.
// obs.: A descrição default deve ser desabilitada no sdk.
const tusb_desc_device_t device_descriptor = {
    .bLength            = sizeof(device_descriptor),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = 0,
    .bDeviceSubClass    = 0,
    .bDeviceProtocol    = 0,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = CONFIG_TINYUSB_DESC_CUSTOM_VID,
    .idProduct          = CONFIG_TINYUSB_DESC_CUSTOM_PID,
    .bcdDevice          = CONFIG_TINYUSB_DESC_BCD_DEVICE,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};

// Armazena a configuração de 1 HID com 1 dispositivo enviado na inicialização.
static const uint8_t hid_configuration_descriptor[] = {
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, TUSB_DESC_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_HID_DESCRIPTOR(0, 0, false, sizeof(hid_report_descriptor), 0x81, 16, 10),
};

// Variável que armazena os dados do HID.
USBHidReport gudtUsbHidReport;

////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * @brief  Invocado quando recebida a solicitação GET HID REPORT DESCRIPTOR
 *
 * @param uint8_t instance
 * @return Ponteiro de retorno do aplicativo para o descritor, cujo conteúdo deve existir por tempo 
 *         suficiente para que a transferência seja concluída
 */
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
    // We use only one interface and one HID report descriptor, so we can ignore parameter 'instance'
    return hid_report_descriptor;
}

/*!
 * @brief  Invocado quando recebe solicitação de controle GET_REPORT
 *
 * @param uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen
 * @return Retornar zero fará com que a pilha bloqueie a solicitação
 */ 
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

/*!
 * @brief  Invocado quando recebeu solicitação de controle SET_REPORT ou dados recebidos no endpoint OUT (Report ID = 0, Type = 0)
 *
 * @param uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize
 * @return void
 */ 
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
}

/*!
 * @brief Inicializa e instala o driver do HID.
 *
 * @param void
 * @return void
 */
void initHID(void)
{
    ESP_LOGI(TAG_UH, "USB initialization");
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = &device_descriptor,
        .string_descriptor = NULL,
        .external_phy = false,
        .configuration_descriptor = hid_configuration_descriptor,
    };

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));
    ESP_LOGI(TAG_UH, "USB initialization DONE");
}

/*!
 * @brief Faz a transmissão do HID report.
 *
 * @param void
 * @return void
 */
void writeToHID(void)
{
    tud_hid_n_report(0, HID_DEVICE_ID, &gudtUsbHidReport, sizeof(gudtUsbHidReport));
}
