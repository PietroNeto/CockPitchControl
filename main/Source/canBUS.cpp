/** @file canBUS.c
 * 
 * @brief Modulo de controle da can.
 *
 * Autor Pietro Di Bernardo Neto
 */ 

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "canBUS.h"
#include "mcp2515.h"
#include <stdbool.h>
#include <stdint.h>
#include "can.h"
#include "usbhid.h"
#include "hardware.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "digitalin.h"
#include "analogin.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES ENUNS e STRUCT
////////////////////////////////////////////////////////////////////////////////////////////////////

#define IS_JS_DEFAULT_ADDRESS(can_id)                                 ( (can_id&0xFF) == 0x33 )

enum DeviceType
{
    JoyStick_Rigth,
    JoyStick_Left,
    Slave_device,
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////

// armazena o endereço do módulo.
static uint8_t myaddress;

// Armazena a mensagem CAN recebida.
struct can_frame canMsg;

// Armazena tipo de operação.
static bool isSlave;

// Cria o objeto de controle da CAN.
MCP2515 mcp2515;

// Indice da ultima escrita na CAN.
static uint8_t IndexAnalog = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////
bool FilterInJ1939Format(const struct can_frame j1939Msg, uint32_t pgn_filter);
void configDevice(enum DeviceType TypeOp);
void CanDataTreat(void);
void CheckReceivedCanData(void);
void CanInicConfigs(void);
void CanSaveConfigs(void);
void CanInicNvs(void);

bool FilterInJ1939Format(const struct can_frame  j1939Msg, uint32_t pgn_filter)
{
	// Extract J1939 PGN, PF, and PS from CAN ID and print
	uint32_t j1939_29_bit_can_id = j1939Msg.can_id & J1939_CAN_ID_MASK;
	uint32_t j1939_18_bit_pgn = (j1939_29_bit_can_id & J1939_PGN_MASK) >> 8;
	uint8_t j1939_8_bit_pf = (j1939_18_bit_pgn & J1939_PF_MASK) >> 8;
	uint32_t pgn;

	if (J1939_PDU1_RANGE(j1939_8_bit_pf))
	{
		pgn = j1939_18_bit_pgn & 0x0003FF00;
	}
	else
	{
		pgn = j1939_18_bit_pgn;
	}

	if(pgn==pgn_filter){
		return true;
	}

	return false;
}

bool FilterMyAddres(const struct can_frame  j1939Msg)
{
    // Extract J1939 CAN ID
	uint32_t j1939_29_bit_can_id = j1939Msg.can_id & J1939_CAN_ID_MASK;
    uint8_t  j1939_8_bit_source_addres = (j1939_29_bit_can_id & J1939_PS_MASK);

    if(j1939_8_bit_source_addres == myaddress)
    {
        return true;
    }

    return false;
}

void InitCAN(void)
{
    // Inicia MCP2515.
    mcp2515.init();
	mcp2515.reset();
	mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ);
	mcp2515.setNormalMode();

    // Inicia NVS.
    CanInicNvs();

    // Carrega configurações.
    CanInicConfigs();
}

void readCAN(void)
{
    // Faz a leitura do barramento da CAN.
    uint8_t ret = mcp2515.readMessage(&canMsg);

    // Se recebeu alguma msg, verifica se os dados são úteis.
    if(ret)
    {
        CheckReceivedCanData();
    }
}

void writeCAN(void)
{
    if(isSlave)
    {
        int8_t analog_values[] = {analog1, analog2, analog3, analog4, analog5, analog6, analog7, analog8};

        // Formata frame de dados digitais
        canMsg.can_id = 0x8CFDD6A0;
        canMsg.can_dlc = 8;
        canMsg.data[0] = 0x10;
        canMsg.data[1] = IndexAnalog;
        canMsg.data[2] = analog_values[IndexAnalog];
        canMsg.data[3] = analog_values[IndexAnalog+1];
        canMsg.data[4] = (gudtUsbHidReport.Digital       & 0xFF);
        canMsg.data[5] = (gudtUsbHidReport.Digital >>  8 & 0xFF);
        canMsg.data[6] = (gudtUsbHidReport.Digital >> 16 & 0xFF);
        canMsg.data[7] = (gudtUsbHidReport.Digital >> 24 & 0xFF);
        

        printf("%#010lx ", canMsg.can_id);
        printf(" [%d]  ", sizeof(canMsg.data));
        for(int i=0; i<8;i++){
            printf("%#010x ", canMsg.data[i]);
        }
        printf("\n");


        // Envia Frame
        mcp2515.sendMessage(&canMsg);
        IndexAnalog += 2;
        if(IndexAnalog >= 8)
        {
            IndexAnalog = 0;
        }
    }
}

void CheckReceivedCanData(void)
{
    if (FilterInJ1939Format(canMsg, JSPGN) && !isSlave)
    {
        if(IS_JS_DEFAULT_ADDRESS(canMsg.can_id))
        {
            if((canMsg.data[5]&0x03)==0x01 && (canMsg.data[5]&0x30)==0x10)
            {
                configDevice(JoyStick_Left);
            }
            if((canMsg.data[5]&0xC0)==0x40 && (canMsg.data[5]&0x0C)==0x04)
            {
                configDevice(JoyStick_Rigth);
            }
        } 
        else 
        {
            CanDataTreat();
        }
    }
    else if(FilterMyAddres(canMsg))
    {
        // mensagem do tipo alterar endereço.
        if(canMsg.data[0] == 0x30)
        {
            myaddress = canMsg.data[1];
        }

        // mensagem do tipo alterar range analog.
        if(canMsg.data[0] == 0x40)
        {
            analog_1_2 = canMsg.data[1];
            analog_3_4 = canMsg.data[2];
            analog_5_6 = canMsg.data[3];
            analog_7_8 = canMsg.data[4];
        }

        // mensagem do tipo tornar slave.
        if(canMsg.data[0] == 0x50 && canMsg.data[7] == 0xFF)
        {
            isSlave = true;
        }

        // mensagem do tipo tornar master.
        if(canMsg.data[0] == 0x50 && canMsg.data[7] == 0x00)
        {
            isSlave = false;
        }

        CanSaveConfigs();
    }
}

void configDevice(enum DeviceType TypeOp)
{
    switch (TypeOp)
    {
        case JoyStick_Left:

            canMsg.can_id = 0x9CFF4633;
            canMsg.can_dlc = 8;
            canMsg.data[0] = 0x20;
            canMsg.data[1] = 0xFF;
            canMsg.data[2] = 0xA1;
            canMsg.data[3] = 0xFF;
            canMsg.data[4] = 0xFF;
            canMsg.data[5] = 0xFF;
            canMsg.data[6] = 0xFF;
            canMsg.data[7] = 0xFF;

        break;
        case JoyStick_Rigth:
            
            canMsg.can_id = 0x9CFF4633;
            canMsg.can_dlc = 8;
            canMsg.data[0] = 0x20;
            canMsg.data[1] = 0xFF;
            canMsg.data[2] = 0xA2;
            canMsg.data[3] = 0xFF;
            canMsg.data[4] = 0xFF;
            canMsg.data[5] = 0xFF;
            canMsg.data[6] = 0xFF;
            canMsg.data[7] = 0xFF;

        break;
        case Slave_device:
        break;
    }

    mcp2515.sendMessage(&canMsg);
}

void CanDataTreat(void)
{
    uint64_t jscan_data = 0;
    uint16_t x_axis = 0;
    uint8_t control = 0;
    uint16_t y_axis = 0;

    for(int i=0; i<4;i++){
        jscan_data += canMsg.data[i]<<(8*i);
    }

    //direito
    if((canMsg.can_id&0xFF)==0xA2)
    {
        x_axis = 0;
        control = 0;
        y_axis = 0;

        //eixo x
        x_axis = (jscan_data & AXIS_MASK)>>6;
        control = jscan_data & AXIS_CONTROL;
        switch (control)
        {
        case 4:
            analog1 = (int8_t)((x_axis)*(-120)/1024);
            break;
        case 16:
            analog1 = (int8_t)((x_axis)*( 120)/1024);
            break;
        default:
            analog1 = 0;
            break;
        }

        //eixo y
        y_axis = (jscan_data>>16 & AXIS_MASK)>>6;
        control = jscan_data>>16 & AXIS_CONTROL;
        switch (control)
        {
        case 4:
            analog2 = (int8_t)((y_axis)*(-120)/1024);
            break;
        case 16:
            analog2 = (int8_t)((y_axis)*( 120)/1024);
            break;
        default:
            analog2 = 0;
            break;
        }
        
        //botoes
        if((canMsg.data[5]&0x03)==0x01)
        {
            gCANDigitalButtons |= JSB_CAN_4_DIR;
        }
        else
        {
            gCANDigitalButtons &= ~JSB_CAN_4_DIR;
        }
        if((canMsg.data[5]&0x0C)==0x04)
        {
            gCANDigitalButtons |= JSB_CAN_3_DIR;
        }
        else
        {
            gCANDigitalButtons &= ~JSB_CAN_3_DIR;
        }
        if((canMsg.data[5]&0x30)==0x10)
        {
            gCANDigitalButtons |= JSB_CAN_2_DIR;
        }
        else
        {
            gCANDigitalButtons &= ~JSB_CAN_2_DIR;
        }
        if((canMsg.data[5]&0xC0)==0x40)
        {
            gCANDigitalButtons |= JSB_CAN_1_DIR;
        }
        else
        {
            gCANDigitalButtons &= ~JSB_CAN_1_DIR;
        }
        if((canMsg.data[6]&0xC0)==0x40)
        {
            gCANDigitalButtons |= JSB_CAN_5_DIR;
        }
        else
        {
            gCANDigitalButtons &= ~JSB_CAN_5_DIR;
        }
    }
    //esquerdo
    else if((canMsg.can_id&0xFF)==0xA1)
    {
        x_axis = 0;
        control = 0;
        y_axis = 0;

        //eixo x
        x_axis = (jscan_data & AXIS_MASK)>>6;
        control = jscan_data & AXIS_CONTROL;
        switch (control)
        {
        case 4:
            analog3 = (int8_t)((x_axis)*(-120)/1024);
            break;
        case 16:
            analog3 = (int8_t)((x_axis)*( 120)/1024);
            break;
        default:
            analog3 = 0;
            break;
        }

        //eixo y
        y_axis = (jscan_data>>16 & AXIS_MASK)>>6;
        control = jscan_data>>16 & AXIS_CONTROL;
        switch (control)
        {
        case 4:
            analog4 = (int8_t)((y_axis)*(-120)/1024);
            break;
        case 16:
            analog4 = (int8_t)((y_axis)*( 120)/1024);
            break;
        default:
            analog4 = 0;
            break;
        }
        
        //botoes
        if((canMsg.data[5]&0x03)==0x01)
        {
            gCANDigitalButtons |= JSB_CAN_4_ESQ;
        }
        else
        {
            gCANDigitalButtons &= ~JSB_CAN_4_ESQ;
        }
        if((canMsg.data[5]&0x0C)==0x04)
        {
            gCANDigitalButtons |= JSB_CAN_3_ESQ;
        }
        else
        {
            gCANDigitalButtons &= ~JSB_CAN_3_ESQ;
        }
        if((canMsg.data[5]&0x30)==0x10)
        {
            gCANDigitalButtons |= JSB_CAN_2_ESQ;
        }
        else
        {
            gCANDigitalButtons &= ~JSB_CAN_2_ESQ;
        }
        if((canMsg.data[5]&0xC0)==0x40)
        {
            gCANDigitalButtons |= JSB_CAN_1_ESQ;
        }
        else
        {
            gCANDigitalButtons &= ~JSB_CAN_1_ESQ;
        }
        if((canMsg.data[6]&0xC0)==0x40)
        {
            gCANDigitalButtons |= JSB_CAN_5_ESQ;
        }
        else
        {
            gCANDigitalButtons &= ~JSB_CAN_5_ESQ;
        }
    }
    //escravo
    else if((canMsg.can_id&0xFF)==0xA0)
    {
        if(canMsg.data[0] == 0x10)
        {
            uint32_t slaveDigitalRead = (canMsg.data[4] | canMsg.data[5] << 8 | canMsg.data[6] << 16 | canMsg.data[7] << 24);
            for(uint8_t i=0; i<32; i++)
            {
                if(slaveDigitalRead & 1<<i)
                {
                    gCANDigitalButtons |= (1<<i);
                }
                else
                {
                    gCANDigitalButtons &= ~(1<<i);
                }
            }
        }
    }

    printf("%#010lx ", canMsg.can_id);
    printf(" [%d]  ", sizeof(canMsg.data));
    for(int i=0; i<8;i++){
        printf("%#010x ", canMsg.data[i]);
    }
    printf("\n");
}

void CanInicConfigs(void)
{
    int32_t analog_range;
    int32_t configs;

    // Open
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);

    if (err == ESP_OK) 
    {
        // Carrega ranges do ADCs
        err = nvs_get_i32(my_handle, "analog_range", &analog_range);
        if (err == ESP_OK) 
        {
            analog_1_2 = (analog_range >> 0  & 0xFF);
            analog_3_4 = (analog_range >> 8  & 0xFF);
            analog_5_6 = (analog_range >> 16 & 0xFF);
            analog_7_8 = (analog_range >> 24 & 0xFF);
        }
        else
        {
            // Carrega valores Defaults.
            analog_1_2 = (DEFAULT_ANALOG_1 << 4 | DEFAULT_ANALOG_2);
            analog_3_4 = (DEFAULT_ANALOG_3 << 4 | DEFAULT_ANALOG_4);
            analog_5_6 = (DEFAULT_ANALOG_5 << 4 | DEFAULT_ANALOG_6);
            analog_7_8 = (DEFAULT_ANALOG_7 << 4 | DEFAULT_ANALOG_8);
        }

        // Carrega valores de configurações.
        err = nvs_get_i32(my_handle, "configs", &configs);
        if (err == ESP_OK) 
        {
            myaddress = (configs >> 0  & 0xFF);
            isSlave   = (configs >> 8  & 0x01);
        }
        else
        {
            // Carrega valores Defaults.
            myaddress = TELEOP_DEFAULT_ADDRESS;
            isSlave = IS_SLAVE;
        }
                
    }
}

void CanSaveConfigs(void)
{
    int32_t analog_range = (analog_7_8<<24 | analog_5_6<<16 | analog_3_4<<8 | analog_1_2);
    int32_t configs = (isSlave<<8 | myaddress);

    // Open
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err == ESP_OK) 
    {
        // Salva valores de range do ADC.
        err = nvs_set_i8(my_handle, "analog_range", analog_range);
        // Commit written value.
        err = nvs_commit(my_handle);

        // Salva valores de configuração.
        err = nvs_set_i8(my_handle, "configs", configs);
        // Commit written value.
        err = nvs_commit(my_handle);
    }
    // Close
    nvs_close(my_handle);
}

void CanInicNvs(void)
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
}