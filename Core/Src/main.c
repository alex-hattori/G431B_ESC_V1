/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fdcan.h"
#include "i2c.h"
#include "opamp.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "user_config.h"
#include "hw_config.h"
#include "structs.h"
#include "position_sensor.h"
#include "fsm.h"
#include "gatedrive.h"
#include "foc.h"
#include "math_ops.h"
#include "calibration.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define VERSION_NUM 1.0f
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Structs for control, etc */

ControllerStruct controller;
ObserverStruct observer;
COMStruct com;
FSMStruct state;
EncoderStruct comm_encoder;
CalStruct comm_encoder_cal;
CANTxMessage can_tx;
CANRxMessage can_rx;

ControllerStruct dummy;

/* init but don't allocate calibration arrays */
int *error_array = NULL;
int *lut_array = NULL;

uint8_t Serial2RxBuffer[1];
float __float_reg[FLOAT_REG_LEN];
int __int_reg[INT_REG_LEN];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	HAL_Delay(100);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_I2C1_Init();
  MX_OPAMP1_Init();
  MX_OPAMP2_Init();
  MX_OPAMP3_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_FDCAN1_Init();
  /* USER CODE BEGIN 2 */
	load_eeprom_regs();
	/* Sanitize configs in case flash is empty*/
	if(E_ZERO==-1){E_ZERO = 0;}
	if(isnan(I_BW) || I_BW==-1){I_BW = 1000;}
	if(isnan(I_MAX) || I_MAX ==-1){I_MAX=40;}
	if(isnan(I_FW_MAX) || I_FW_MAX ==-1){I_FW_MAX=0;}
	if(CAN_ID==-1){CAN_ID = 1;}
	if(CAN_MASTER==-1){CAN_MASTER = 0;}
	if(CAN_TIMEOUT==-1){CAN_TIMEOUT = 0;}
	if(isnan(R_NOMINAL) || R_NOMINAL==-1){R_NOMINAL = 0.0f;}
	if(isnan(TEMP_MAX) || TEMP_MAX==-1){TEMP_MAX = 125.0f;}
	if(isnan(I_MAX_CONT) || I_MAX_CONT==-1){I_MAX_CONT = 14.0f;}
	if(isnan(I_CAL)||I_CAL==-1){I_CAL = 5.0f;}
	if(isnan(PPAIRS) || PPAIRS==-1){PPAIRS = 21.0f;}
	if(isnan(GR) || GR==-1){GR = 1.0f;}
	if(isnan(KT) || KT==-1){KT = 1.0f;}
	if(isnan(KP_MAX) || KP_MAX==-1){KP_MAX = 500.0f;}
	if(isnan(KD_MAX) || KD_MAX==-1){KD_MAX = 5.0f;}
	if(isnan(P_MAX)){P_MAX = 12.5f;}
	if(isnan(P_MIN)){P_MIN = -12.5f;}
	if(isnan(V_MAX)){V_MAX = 200.0f;}
	if(isnan(V_MIN)){V_MIN = -200.0f;}
	if(isnan(MECH_ZERO)){MECH_ZERO = 0.0f;}
	if(isnan(T_MAX)){T_MAX = 10.0f;}

	printf("\r\nFirmware Version Number: %.2f\r\n", VERSION_NUM);
	/* Controller Setup */
	init_controller_params(&controller);

	/* calibration "encoder" zeroing */
	memset(&comm_encoder_cal.cal_position, 0, sizeof(EncoderStruct));

	/* commutation encoder setup */
	comm_encoder.e_zero = E_ZERO;
	comm_encoder.ppairs = PPAIRS;
	comm_encoder.mech_zero = MECH_ZERO;
	ps_warmup(&comm_encoder, 100);			// clear the noisy data when the encoder first turns on
	ps_sample(&comm_encoder, DT);
	ps_sample(&comm_encoder, DT);
	ps_sample(&comm_encoder, DT);
	if(comm_encoder.angle_multiturn[0]>PI_F){
	comm_encoder.angle_multiturn[0]-=TWO_PI_F;
	comm_encoder.turns--;
	printf("Added\r\n");
	}
	else if(comm_encoder.angle_multiturn[0]<-PI_F){
	comm_encoder.angle_multiturn[0] += TWO_PI_F;
	comm_encoder.turns++;
	printf("Subtracted\r\n");
	}


	if(EN_ENC_LINEARIZATION){memcpy(&comm_encoder.offset_lut, &ENCODER_LUT, sizeof(comm_encoder.offset_lut));}	// Copy the linearization lookup table
	else{memset(&comm_encoder.offset_lut, 0, sizeof(comm_encoder.offset_lut));}

	/* Turn on ADCs */
	HAL_OPAMP_Start(&hopamp1);
	HAL_OPAMP_Start(&hopamp2);
	HAL_OPAMP_Start(&hopamp3);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)controller.ADC1_Val, 2);
	HAL_ADC_Start_DMA(&hadc2, (uint32_t *)controller.ADC2_Val, 2);

	disable_gd(&controller);
	HAL_Delay(10);

	zero_current(&controller);
	HAL_Delay(100);
	printf("ADC A OFFSET: %d     ADC B OFFSET: %d     ADC C OFFSET: %d\r\n", controller.adc_a_offset, controller.adc_b_offset, controller.adc_c_offset);
	printf("Vbus: %f\r\n",controller.v_bus);
	/* Turn on PWM */
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

	disable_gd(&controller);

	/* CAN setup */
	HAL_GPIO_WritePin(CAN_SHDWN, GPIO_PIN_RESET); //Enable CAN
	HAL_GPIO_WritePin(CAN_TERM, GPIO_PIN_RESET ); //Disable CAN termination resistor


	can_rx_init(&can_rx);
	can_tx_init(&can_tx);

	HAL_FDCAN_Start(&CAN_H); //start CAN

	/* Set Interrupt Priorities */
	NVIC_SetPriority(PWM_ISR, 1);

	/* Start the FSM */
	state.state = MENU_MODE;
	state.next_state = MENU_MODE;
	state.ready = 1;

	HAL_UART_Receive_IT(&huart2, (uint8_t *)Serial2RxBuffer, 1);
	HAL_TIM_Base_Start_IT(&htim1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	printf("A:%f B:%f C:%f V:%f \r\n",controller.i_a, controller.i_b, controller.i_c, controller.v_bus);
//	  printf("%f %f\r\n",controller.i_q, controller.i_q_des);
		static float const R60 = 4700.0f; // ohm
		static float const eps = 0.1f; // epsilon (avoid divide by zero)
		float const R_NTC = R60*(4096.0f/(controller.ADC1_Val[2]+eps)-1.0f); // 10kohm NTC at 25°C
		static float const Beta = 3455.0f; // for a 10k NTC
		static float const Kelvin = 273.15f; //°C
		static float const T0 = 273.15f + 25.0f;
		static float const R0 = 10000.0f; // 10kohm at 25° for 10k NTC
		float const present_temperature_K = Beta * T0 / ( Beta - T0*logf(R0/R_NTC) );
		controller.fet_temp_C = present_temperature_K-Kelvin;
		printf("%f\r\n",controller.fet_temp_C);
		HAL_Delay(100);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_ADC12|RCC_PERIPHCLK_FDCAN;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
