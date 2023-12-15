/** @file analogin.c
 * 
 * @brief Modulo de leitura das entradas analógicas. 
 *
 * Autor Pietro Di Bernardo Neto
 */ 

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "sdkconfig.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#include "soc/soc_caps.h"
#include "driver/adc.h"
#include "driver/gpio.h"
#include "analogin.h"
#include "usbhid.h"
#include "hardware.h"
#include "digitalin.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS GLOBAIS DO MODULO
////////////////////////////////////////////////////////////////////////////////////////////////////
int8_t  analog1;
int8_t  analog2;
int8_t  analog3;
int8_t  analog4;
int8_t  analog5;
int8_t  analog6;
int8_t  analog7;
int8_t  analog8;

// Armazena ranges dos analógicos.
uint8_t analog_1_2;
uint8_t analog_3_4;
uint8_t analog_5_6;
uint8_t analog_7_8;

////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////

void InitAnalogIn(void)
{
    ESP_ERROR_CHECK(adc1_config_width((adc_bits_width_t)ADC_WIDTH_BIT_DEFAULT));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_2, ADC_ATTEN_DB_11));
	ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11));
	ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11));
	ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN_DB_11));
	ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11));
	ESP_ERROR_CHECK(adc2_config_channel_atten(ADC2_CHANNEL_3, ADC_ATTEN_DB_11));
	ESP_ERROR_CHECK(adc2_config_channel_atten(ADC2_CHANNEL_6, ADC_ATTEN_DB_11));
	ESP_ERROR_CHECK(adc2_config_channel_atten(ADC2_CHANNEL_7, ADC_ATTEN_DB_11));
}

void readAnalog(void)
{
    int adc_raw[8];
    float range;
    uint32_t ranges = (analog_1_2 | analog_3_4 << 8 | analog_5_6 << 16 | analog_7_8 << 24);
    uint32_t ranges_pos = ranges;
    int8_t analog_values[] = {analog1, analog2, analog3, analog4, analog5, analog6, analog7, analog8};
    uint8_t analogDigitalAddress[] = {JSB_ANALOG_1, JSB_ANALOG_2, JSB_ANALOG_3, JSB_ANALOG_4, JSB_ANALOG_5, JSB_ANALOG_6, JSB_ANALOG_7, JSB_ANALOG_8};


    if(ranges&0xF)
    {
        range = 245*15/(ranges&0xF);
        adc_raw[0] = adc1_get_raw(ADC1_CHANNEL_2);
        adc_raw[0] = ((adc_raw[0]*range)/3176)-127;
    }
    else
    {
        adc_raw[0] = adc1_get_raw(ADC1_CHANNEL_2);
    }
    ranges = ranges >> 4;

    if(ranges&0xF)
    {
        range = 245*15/(ranges&0xF);
        adc_raw[1] = adc1_get_raw(ADC1_CHANNEL_3);
        adc_raw[1] = ((adc_raw[1]*range)/3176)-127;
    }
    else
    {
        adc_raw[1] = adc1_get_raw(ADC1_CHANNEL_3);
    }
    ranges = ranges >> 4;

    if(ranges&0xF)
    {
        range = 245*15/(ranges&0xF);
        adc_raw[2] = adc1_get_raw(ADC1_CHANNEL_4);
        adc_raw[2] = ((adc_raw[2]*range)/3176)-127;
    }
    else
    {
        adc_raw[2] = adc1_get_raw(ADC1_CHANNEL_4);
    }
    ranges = ranges >> 4;

    if(ranges&0xF)
    {
        range = 245*15/(ranges&0xF);
        adc_raw[3] = adc1_get_raw(ADC1_CHANNEL_5);
        adc_raw[3] = ((adc_raw[3]*range)/3176)-127;
    }
    else
    {
        adc_raw[3] = adc1_get_raw(ADC1_CHANNEL_5);
    }
    ranges = ranges >> 4;

    if(ranges&0xF)
    {
        range = 245*15/(ranges&0xF);
        adc_raw[4] = adc1_get_raw(ADC1_CHANNEL_6);
        adc_raw[4] = ((adc_raw[4]*range)/3176)-127;
    }
    else
    {
        adc_raw[4] = adc1_get_raw(ADC1_CHANNEL_6);
    }
    ranges = ranges >> 4;

    if(ranges&0xF)
    {
        range = 245*15/(ranges&0xF);
        adc2_get_raw(ADC2_CHANNEL_3, (adc_bits_width_t)ADC_WIDTH_BIT_12, &adc_raw[5]);
        adc_raw[5] = ((adc_raw[5]*range)/3176)-127;
    }
    else
    {
        adc2_get_raw(ADC2_CHANNEL_3, (adc_bits_width_t)ADC_WIDTH_BIT_12, &adc_raw[5]);
    }
    ranges = ranges >> 4;

    if(ranges&0xF)
    {
        range = 245*15/(ranges&0xF);
        adc2_get_raw(ADC2_CHANNEL_6, (adc_bits_width_t)ADC_WIDTH_BIT_12, &adc_raw[6]);
        adc_raw[6] = ((adc_raw[6]*range)/3176)-127;
    }
    else
    {
        adc2_get_raw(ADC2_CHANNEL_6, (adc_bits_width_t)ADC_WIDTH_BIT_12, &adc_raw[6]);
    }
    ranges = ranges >> 4;

    if(ranges&0xF)
    {
        range = 245*15/(ranges&0xF);
        adc2_get_raw(ADC2_CHANNEL_7, (adc_bits_width_t)ADC_WIDTH_BIT_12, &adc_raw[7]);
        adc_raw[7] = ((adc_raw[7]*range)/3176)-127;
    }
    else
    {
        adc2_get_raw(ADC2_CHANNEL_7, (adc_bits_width_t)ADC_WIDTH_BIT_12, &adc_raw[7]);
    }

    for(uint8_t i=0; i<8; i++)
    {
        if( (ranges_pos >> 4*i) & 0xF)
        {
            if(analog_values[i] <= adc_raw[i])
            {
                *((int8_t*)&gudtUsbHidReport + i) = (int8_t)adc_raw[i];
            }
            else
            {
                *((int8_t*)&gudtUsbHidReport + i) = (int8_t)analog_values[i];
            }
        }
        else
        {
            if(adc_raw[i]>15)
            {
                gAnalogEndDigitalButton |= (1<<analogDigitalAddress[i]);
            }
            else
            {
                gAnalogEndDigitalButton &= ~(1<<analogDigitalAddress[i]);
            }
        }
        
    }

}