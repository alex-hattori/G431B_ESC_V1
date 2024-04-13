/*
 * position_sensor.c
 *
 *  Created on: Jul 26, 2020
 *      Author: Alex
 */
#include <stdio.h>
#include <string.h>
#include "position_sensor.h"
#include "math_ops.h"
#include "hw_config.h"
#include "user_config.h"
#include "gpio.h"

void ps_warmup(EncoderStruct * encoder, int n){
	/* Hall position sensors noisy on startup.  Take a bunch of samples to clear this data */
	for(int i = 0; i<n; i++){
		/* SPI read */
		HAL_GPIO_WritePin(ENC_SPI_CLK, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(ENC_SPI_CS, GPIO_PIN_RESET); //CS Low
		for(int i = 0; i<5; i++){
			//delay for CS falling edge -> clk rising edge requirement
		}
		uint16_t spi_val;
		int index = 15;
		//MSB first
		//Read on rising edge
		//Takes 50ns
		while(index>=0){
			HAL_GPIO_WritePin(ENC_SPI_CLK, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(ENC_SPI_MISO)){
				spi_val |= 1<<index;
			}
			index--;
			HAL_GPIO_WritePin(ENC_SPI_CLK, GPIO_PIN_RESET);
		}
		HAL_GPIO_WritePin(ENC_SPI_CS, GPIO_PIN_SET); //CS High
	}
}

void ps_sample(EncoderStruct * encoder, float dt){
	/* updates EncoderStruct encoder with the latest sample
	 * after elapsed time dt */

	/* Shift around previous samples */
	encoder->old_angle = encoder->angle_singleturn;
//	for(int i = N_POS_SAMPLES-1; i>0; i--){encoder->angle_multiturn[i] = encoder->angle_multiturn[i-1];}
	memmove(&encoder->angle_multiturn[1], &encoder->angle_multiturn[0], (N_POS_SAMPLES-1)*sizeof(float));

	/* SPI read */
	HAL_GPIO_WritePin(ENC_SPI_CLK, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ENC_SPI_CS, GPIO_PIN_RESET); //CS Low
	for(int i = 0; i<5; i++){
		//delay for CS falling edge -> clk rising edge requirement
	}
	uint16_t spi_val;
	int index = 15;
	//MSB first
	//Read on rising edge
	//Takes 50ns
	while(index>=0){
		HAL_GPIO_WritePin(ENC_SPI_CLK, GPIO_PIN_SET);
		if(HAL_GPIO_ReadPin(ENC_SPI_MISO)){
			spi_val |= 1<<index;
		}
		index--;
		HAL_GPIO_WritePin(ENC_SPI_CLK, GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(ENC_SPI_CS, GPIO_PIN_SET); //CS High

	encoder->raw = spi_val&0x3FFF; //angle;

	/* Linearization */
	int off_1 = encoder->offset_lut[(encoder->raw)>>8];				// lookup table lower entry
	int off_2 = encoder->offset_lut[((encoder->raw>>8)+1)%64];		// lookup table higher entry
	int off_interp = off_1 + ((off_2 - off_1)*(encoder->raw - ((encoder->raw>>8)<<8))>>8);     // Interpolate between lookup table entries
	encoder->count = encoder->raw - off_interp;


	/* Real angles in radians */
	encoder->angle_singleturn = ((float)(encoder->count))/((float)ENC_CPR);
	int int_angle = encoder->angle_singleturn;
	encoder->angle_singleturn = TWO_PI_F*(encoder->angle_singleturn - (float)int_angle);
	//encoder->angle_singleturn = TWO_PI_F*fmodf(((float)(encoder->count-M_ZERO))/((float)ENC_CPR), 1.0f);
	encoder->angle_singleturn = encoder->angle_singleturn<0 ? encoder->angle_singleturn + TWO_PI_F : encoder->angle_singleturn;

	encoder->elec_angle = (encoder->ppairs*(float)(encoder->count-E_ZERO))/((float)ENC_CPR);
	int_angle = (int)encoder->elec_angle;
	encoder->elec_angle = TWO_PI_F*(encoder->elec_angle - (float)int_angle);
	//encoder->elec_angle = TWO_PI_F*fmodf((encoder->ppairs*(float)(encoder->count-E_ZERO))/((float)ENC_CPR), 1.0f);
	encoder->elec_angle = encoder->elec_angle<0 ? encoder->elec_angle + TWO_PI_F : encoder->elec_angle;	// Add 2*pi to negative numbers
	/* Rollover */
	int rollover = 0;
	float angle_diff = encoder->angle_singleturn - encoder->old_angle;
	if(angle_diff > PI_F){rollover = -1;}
	else if(angle_diff < -PI_F){rollover = 1;}
	encoder->turns += rollover;
	if(!encoder->first_sample){
		encoder->turns = 0;
		encoder->first_sample = 1;
	}

	/* Multi-turn position */
	encoder->angle_multiturn[0] = encoder->angle_singleturn + TWO_PI_F*(float)encoder->turns-encoder->mech_zero;

	encoder->velocity = (encoder->angle_multiturn[0] - encoder->angle_multiturn[N_POS_SAMPLES-1])/(dt*(float)(N_POS_SAMPLES-1));
	encoder->elec_velocity = encoder->ppairs*encoder->velocity;

}

void ps_print(EncoderStruct * encoder, int dt_ms){
//	printf("Raw: %d\r\n", encoder->raw);
	printf("Raw: %d", encoder->raw);
	printf("   Linearized Count: %d", encoder->count);
	printf("   Single Turn: %f", encoder->angle_singleturn);
	printf("   Multiturn: %f", encoder->angle_multiturn[0]);
	printf("   Electrical: %f", encoder->elec_angle);
	printf("   Turns:  %d", encoder->turns);
	printf("   CAN:   %f\r\n", encoder->angle_multiturn[0]);
	//HAL_Delay(dt_ms);
}
