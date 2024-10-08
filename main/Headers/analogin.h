/** @file analogin.h
 * 
 * @brief Modulo de leitura das entradas analógicas. 
 *
 * Autor Pietro Di Bernardo Neto
 */ 

#ifndef ANALOGIN_H
#define ANALOGIN_H

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS GLOBAIS DO MODULO
////////////////////////////////////////////////////////////////////////////////////////////////////
extern int8_t  analog1;
extern int8_t  analog2;
extern int8_t  analog3;
extern int8_t  analog4;
extern int8_t  analog5;
extern int8_t  analog6;
extern int8_t  analog7;
extern int8_t  analog8;

// Armazena ranges dos analógicos.
extern uint8_t analog_1_2;
extern uint8_t analog_3_4;
extern uint8_t analog_5_6;
extern uint8_t analog_7_8;

////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES DE INTERFACE COM O MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////
void InitAnalogIn(void);
void readAnalog(void);

#endif /* ANALOGIN_H */
