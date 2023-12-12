/** @file pcf8574.h
 * 
 * @brief Modulo de comunicação com IO expander PCF8574, configura a interface de comunicação com o 
 * chip usando interface I2C. 
 *
 * Autor Pietro Di Bernardo Neto
 */ 

#ifndef PCF8574_H
#define PCF8574_H

////////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////////////////////////

#define WRITE_BIT I2C_MASTER_WRITE                              /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ                                /*!< I2C master read */
#define ACK_CHECK_EN 0x1                                        /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0                                       /*!< I2C master will not check ack from slave */
#define ACK_VAL (i2c_ack_type_t)0x0                             /*!< I2C ack value */
#define NACK_VAL (i2c_ack_type_t)0x1                            /*!< I2C nack value */
#define DATA_LENGTH 8                                           /*!< Data buffer length of test buffer */
#define IO_EXP_01_MASK 0xB7
#define IO_EXP_02_MASK 0x3F


////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIAVEIS GLOBAIS DO MODULO
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNÇÕES DE INTERFACE COM O MÓDULO
////////////////////////////////////////////////////////////////////////////////////////////////////
void InitDigitalRead(void);
uint8_t ReadIOs(uint8_t slave_addr);

#endif /* PCF8574_H */

/*** end of file ***/