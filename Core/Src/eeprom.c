/*
 * eeprom.c
 *
 *  Created on: 13 déc. 2020
 *      Author: Patrick, Alex
 */

#include "eeprom.h"
#include <string.h>

static uint32_t const page_nb = 58U; //(uint32_t)FLASH_PAGE_NB-1;
static uint32_t const start_address = 0x08000000UL+58*0x800U; //(FLASH_BASE)+page_nb*(FLASH_PAGE_SIZE);

HAL_StatusTypeDef eeprom_restore(float * float_regs, uint32_t float_size, int * int_regs, uint32_t int_size)
{
	uint64_t data = 0xDEADBEEF;
	for(uint32_t index=0; index<float_size;index+=2) // 64 bits
	{
		memcpy(&data, (uint32_t *)(start_address+32*index), sizeof(uint64_t));
		uint32_t temp = data&0xFFFFFFFF;
		uint32_t temp2 = data>>32;
		float value;
		float value2;
		memcpy(&value,&temp,sizeof(uint32_t));
		memcpy(&value2,&temp2,sizeof(uint32_t));
		float_regs[index] = value;
		float_regs[index+1] = value2;
	}
	for(uint32_t index=0; index<int_size;index+=2) // 64 bits
	{

		memcpy(&data, (uint32_t *)(start_address+32*(index+float_size)), sizeof(uint64_t));
		uint32_t temp = data&0xFFFFFFFF;
		uint32_t temp2 = data>>32;
		int value;
		int value2;
		memcpy(&value,&temp,sizeof(uint32_t));
		memcpy(&value2,&temp2,sizeof(uint32_t));
		int_regs[index] = value;
		int_regs[index+1] = value2;
	}

	return HAL_OK;
}

HAL_StatusTypeDef eeprom_store(float const * float_regs, uint32_t float_size, int const * int_regs, uint32_t int_size)
{
	HAL_FLASH_Unlock();
	// erase the last page of bank1 (STM32G43x : 1 bank, 64 pages, 2kB per page, 64-bit data)
	{
		FLASH_EraseInitTypeDef erase ={FLASH_TYPEERASE_PAGES,FLASH_BANK_1,page_nb,5};
		uint32_t page_error = 0;
		HAL_StatusTypeDef result = HAL_FLASHEx_Erase(&erase, &page_error);
		if(result!=HAL_OK)
		{
			HAL_FLASH_Lock();
			return result;
		}
	}
	// write the last page
	{
		uint64_t data = 0xDEADBEEF;
		for(uint32_t index=0; index<float_size;index+=2)
		{
			union UN {float a; uint32_t b;};
			union UN un1;
			union UN un2;
			un1.a = float_regs[index];
			un2.a = float_regs[index+1];
			uint64_t temp = un2.b;
			data = (temp<<32)|un1.b;
			HAL_StatusTypeDef result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,start_address+index*32,data);
			if(result!=HAL_OK)
			{
				uint32_t error = HAL_FLASH_GetError();
				if(error)
				{
					HAL_FLASH_Lock();
					return error;
				}
				HAL_FLASH_Lock();
				return result;
			}
		}
		for(uint32_t index=0; index<int_size;index+=2)
		{
			union UN {int a; uint32_t b;};
			union UN un1;
			union UN un2;
			un1.a = int_regs[index];
			un2.a = int_regs[index+1];
			uint64_t temp = un2.b;
			data = (temp<<32)|un1.b;
			HAL_StatusTypeDef result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,start_address+(float_size+index)*32,data);
			if(result!=HAL_OK)
			{
				uint32_t error = HAL_FLASH_GetError();
				if(error)
				{
					HAL_FLASH_Lock();
					return error;
				}
				HAL_FLASH_Lock();
				return result;
			}
		}
	}
	HAL_FLASH_Lock();
	return HAL_OK;
}
