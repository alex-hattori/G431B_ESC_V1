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
	eeprom_store(__float_reg, 64, __int_reg, 256);
}

void load_eeprom_regs()
{
	eeprom_restore(__float_reg, 64, __int_reg, 256);
}
