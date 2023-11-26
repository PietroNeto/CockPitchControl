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

// Armazena a mensagem CAN recebida.
struct can_frame canMsg;

// Cria o objeto de controle da CAN.
MCP2515 mcp2515;

////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////
bool FilterInJ1939Format(const struct can_frame j1939Msg, uint32_t pgn_filter);
void configDevice(enum DeviceType TypeOp);
void CanDataTreat(void);
void CheckReceivedCanData(void);

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

    mcp2515.readMessage(&canMsg);
}

void CanDataTreat(void)
{
    uint64_t jscan_data = 0;

    for(int i=0; i<4;i++){
        jscan_data += canMsg.data[i]<<(8*i);
    }

    //direito
    if((canMsg.can_id&0xFF)==0xA2){
       
    }

    //esquerdo
    if((canMsg.can_id&0xFF)==0xA1){
        
    }
}

void CheckReceivedCanData(void)
{
    if (FilterInJ1939Format(canMsg, JSPGN))
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
}
