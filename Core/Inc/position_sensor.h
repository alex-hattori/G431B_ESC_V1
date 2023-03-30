/*
 * position_sensor.h
 *
 *  Created on: Jul 26, 2020
 *      Author: Alex
 */

#ifndef INC_POSITION_SENSOR_H_
#define INC_POSITION_SENSOR_H_


//#include "structs.h"
#include <stdint.h>

#define N_POS_SAMPLES 20		// Number of position samples to store.  should put this somewhere else...
#define N_LUT 64

typedef union
{
	      volatile uint16_t      raw;
	      struct
	      {
	          volatile uint16_t      SF            :  2;
	          volatile uint16_t      FTH           :  3;
	          volatile uint16_t      WD            :  1;
	          volatile uint16_t      notused       :  2;
	          volatile uint16_t      PM            :  2;
	          volatile uint16_t      HYST          :  2;
	          volatile uint16_t      OUTS          :  2;
	          volatile uint16_t      PWMF          :  2;

	      } bit;
} conf_reg_t;

typedef union
	{
	    volatile uint16_t      raw;
	    struct
	    {
	        volatile uint16_t      angle8_12     :   4;
	        volatile uint16_t      notused       :   4;
	        volatile uint16_t      angle0_7      :   8;
	    } bit;
	} angle_reg_t;
typedef struct{

	conf_reg_t config;

	angle_reg_t data;

	float angle_singleturn, old_angle, angle_multiturn[N_POS_SAMPLES], elec_angle, velocity, elec_velocity, ppairs, vel2;
	float output_angle_multiturn;
	int raw, count, old_count, turns;
	int count_buff[N_POS_SAMPLES];
	int e_zero;
	int offset_lut[N_LUT];
	uint8_t first_sample;
	float mech_zero;
} EncoderStruct;


void ps_warmup(EncoderStruct * encoder, int n);
void ps_sample(EncoderStruct * encoder, float dt);
void ps_print(EncoderStruct * encoder, int dt_ms);

#endif /* INC_POSITION_SENSOR_H_ */
