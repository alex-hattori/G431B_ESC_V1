/*
 * user_config.c
 *
 *  Created on: Feb 7, 2023
 *      Author: Alex
 */

#include "user_config.h"
#include "eeprom.h"

void store_eeprom_regs()
{
	eeprom_store(__float_reg, FLOAT_REG_LEN, __int_reg, INT_REG_LEN);
}

void load_eeprom_regs()
{
	eeprom_restore(__float_reg, FLOAT_REG_LEN, __int_reg, INT_REG_LEN);
}
