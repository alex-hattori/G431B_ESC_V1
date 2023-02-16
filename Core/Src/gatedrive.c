/*
 * gatedrive.c
 *
 *  Created on: Mar 3, 2022
 *      Author: Alex Hattori
 */
#include "gatedrive.h"
#include <stdio.h>


void enable_gd(ControllerStruct *controller){
	//Restart PWM
	controller->isEnabled = 1;
    HAL_TIM_PWM_Start(&TIM_PWM, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&TIM_PWM, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&TIM_PWM, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(&TIM_PWM, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&TIM_PWM, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(&TIM_PWM, TIM_CHANNEL_3);
}
void disable_gd(ControllerStruct *controller){
	//Drive all gates low
	controller->isEnabled = 0;
	TIM_PWM.Instance->CCR3 = ((TIM_PWM.Instance->ARR))*(0.0f);
	TIM_PWM.Instance->CCR1 = ((TIM_PWM.Instance->ARR))*(0.0f);
	TIM_PWM.Instance->CCR2 = ((TIM_PWM.Instance->ARR))*(0.0f);
	HAL_GPIO_WritePin(DRV_HA, GPIO_PIN_RESET );
	HAL_GPIO_WritePin(DRV_HB, GPIO_PIN_RESET );
	HAL_GPIO_WritePin(DRV_HC, GPIO_PIN_RESET );
	HAL_GPIO_WritePin(DRV_LA, GPIO_PIN_RESET );
	HAL_GPIO_WritePin(DRV_LB, GPIO_PIN_RESET );
	HAL_GPIO_WritePin(DRV_LC, GPIO_PIN_RESET );

}


