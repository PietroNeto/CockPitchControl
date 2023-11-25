/** @file can.c
 * 
 * @brief Modulo de controle da can.
 *
 * Autor Pietro Di Bernardo Neto
 */ 

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////
# include "can.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES ENUNS e STRUCT
////////////////////////////////////////////////////////////////////////////////////////////////////

enum tagDeviceType
{
    JoyStick_Rigth,
    JoyStick_Left,
    Slave_device,
} DeviceType;

////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////

// Armazena a mensagem CAN recebida.
struct can_frame canMsg_send;

////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES PRIVADAS DO MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////
bool FilterInJ1939Format(const struct can_frame &j1939Msg, uint32_t pgn_filter);
void configDevice(enum DeviceType TypeOp);
void CanDataTreat(void);
void CheckReceivedCanData(void);

bool FilterInJ1939Format(const struct can_frame &j1939Msg, uint32_t pgn_filter)
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
    int ret = mcp2515.readMessage(&canMsg);

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
        case JoyStick_Left

            canMsg_send.can_id = 0x9CFF4633;
            canMsg_send.can_dlc = 8;
            canMsg_send.data[0] = 0x20;
            canMsg_send.data[1] = 0xFF;
            canMsg_send.data[2] = 0xA1;
            canMsg_send.data[3] = 0xFF;
            canMsg_send.data[4] = 0xFF;
            canMsg_send.data[5] = 0xFF;
            canMsg_send.data[6] = 0xFF;
            canMsg_send.data[7] = 0xFF;

        break;
        case JoyStick_Rigth
            
            canMsg_send.can_id = 0x9CFF4633;
            canMsg_send.can_dlc = 8;
            canMsg_send.data[0] = 0x20;
            canMsg_send.data[1] = 0xFF;
            canMsg_send.data[2] = 0xA2;
            canMsg_send.data[3] = 0xFF;
            canMsg_send.data[4] = 0xFF;
            canMsg_send.data[5] = 0xFF;
            canMsg_send.data[6] = 0xFF;
            canMsg_send.data[7] = 0xFF;

        break;
    }

    int ret_sc = mcp2515.sendMessage(&canMsg_send);
}

void CanDataTreat(void)
{
    printf("%#010lx ", canMsg.can_id);
    printf(" [%d]  ", sizeof(canMsg.data));
    for(int i=0; i<4;i++){
        jscan_data += canMsg.data[i]<<(8*i);
        printf(" / "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(canMsg.data[i]));
    }
    printf("\n");

    //direito
    if((canMsg.can_id&0xFF)==0xA2){
       
    }

    //esquerdo
    if((canMsg.can_id&0xFF)==0xA1){
        
    }
}

void CheckReceivedCanData(void)
{
    if (ret == MCP2515::ERROR_OK && FilterInJ1939Format(canMsg, JSPGN))
    {
        if((canMsg.can_id&0xFF)==0x33){
            if((canMsg.data[5]&0x03)==0x01 && (canMsg.data[5]&0x30)==0x10){
                configDevice(JoyStick_Left)
            }
            if((canMsg.data[5]&0xC0)==0x40 && (canMsg.data[5]&0x0C)==0x04){
                configDevice(JoyStick_Rigth)
            }
        } else {
            CanDataTreat();
        }
    }
}
