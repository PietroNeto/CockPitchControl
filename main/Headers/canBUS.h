/** @file canBUS.h
 * 
 * @brief Modulo de controle da can.
 *
 * Autor Pietro Di Bernardo Neto
 */ 

#ifndef CANBUS_H
#define CANBUS_H

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////////////////////////
#define J1939_CAN_ID_MASK 		0x1FFFFFFF
#define J1939_PGN_MASK 			0x03FFFF00
#define J1939_PF_MASK 			0x0000FF00
#define J1939_PS_MASK 			0x000000FF
#define AXIS_MASK 				0x0000FFC0
#define AXIS_CONTROL			0x0000003F
#define JSPGN 					64982
#define J1939_PDU1_RANGE(PF) 	(PF < 240)
#define TELEOP_DEFAULT_ADDRESS  0x44

////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS GLOBAIS DO MODULO
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES DE INTERFACE COM O MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////
void InitCAN(void);
void readCAN(void);
void writeCAN(void);

#endif /* CANBUS_H */

/*** end of file ***/