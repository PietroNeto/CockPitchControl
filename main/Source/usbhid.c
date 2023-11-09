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

////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////

// Tag da mensagem de log do módulo.
static const char *TAG_UH = "USB HID";

// Armazena a descrição do report usado no protocolo HID.
const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_GAMEPAD_custom(HID_REPORT_ID(HID_DEVICE_ID) )
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
    .idVendor           = USB_ESPRESSIF_VID,
    .idProduct          = USB_PID,
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
