/*
 * eeprom.c
 *
 *  Created on: 13 déc. 2020
 *      Author: Patrick
 */

#include "eeprom.h"
#include <string.h>

static uint32_t const page_nb = 63U; //(uint32_t)FLASH_PAGE_NB-1;
static uint32_t const start_address = 0x08000000UL+63*0x800U; //(FLASH_BASE)+page_nb*(FLASH_PAGE_SIZE);

uint8_t const * eeprom_base_address()
{
	return (uint8_t*)start_address;
}

bool eeprom_empty()
{
	return *(uint32_t*)(start_address)==0xFFFFFFFF;
}

HAL_StatusTypeDef eeprom_restore(float * float_regs, uint32_t float_size, int * int_regs, uint32_t int_size)
{
	uint64_t data = 0xDEADBEEF;
	for(uint32_t index=0; index<float_size;index+=2) // 64 bits
	{

		memcpy(&data, start_address+32*index, sizeof(uint64_t));
		uint32_t temp = data&0xFFFFFFFF;
		uint32_t temp2 = data>>32;
		float value;
		float value2;
		memcpy(&value,&temp,sizeof(uint32_t));
		memcpy(&value2,&temp2,sizeof(uint32_t));
		printf("Load %i %f\r\n",index, value);
		float_regs[index] = value;
		printf("Load %i %f\r\n",index+1, value2);
		float_regs[index+1] = value2;
	}

	return HAL_OK;
}

HAL_StatusTypeDef eeprom_store(float const * float_regs, uint32_t float_size, int const * int_regs, uint32_t int_size)
{
	HAL_FLASH_Unlock();
	// erase the last page of bank1 (STM32G43x : 1 bank, 64 pages, 2kB per page, 64-bit data)
	{
		FLASH_EraseInitTypeDef erase =
			{
				FLASH_TYPEERASE_PAGES,
				FLASH_BANK_1,
				page_nb,
				1
			};
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
		for(uint32_t index=0; index<float_size;index+=2) // 64 bits
		{
			printf("Write %d %f\r\n",(int)index, float_regs[index]);
			printf("Write %d %f\r\n",(int)index+1, float_regs[index+1]);
			uint32_t temp;
			uint32_t temp2;
			memcpy(&temp,&float_regs[index],sizeof(uint32_t));
			memcpy(&temp2,&float_regs[index+1],sizeof(uint32_t));
			uint64_t temp3 = temp2;
			data = (temp3<<32)|temp;
			HAL_StatusTypeDef result = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,start_address+index*32,data);
			if(result!=HAL_OK)
			{
				printf("Error\r\n");
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
		printf("Done Writing\r\n");
	}
	HAL_FLASH_Lock();
	return HAL_OK;
}
