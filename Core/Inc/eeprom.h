/*
 * eeprom.h
 *
 *  Created on: 13 déc. 2020
 *      Author: Patrick, Alex
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include <stdbool.h>
#include "stm32g4xx_hal.h"

bool eeprom_empty();
HAL_StatusTypeDef eeprom_restore(float * float_regs, uint32_t float_size, int * int_regs, uint32_t int_size);
HAL_StatusTypeDef eeprom_store(float const * float_regs, uint32_t float_size, int const * int_regs, uint32_t int_size);

uint8_t const * eeprom_base_address();

#endif /* INC_EEPROM_H_ */
