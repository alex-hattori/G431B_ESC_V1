#ifndef HW_CONFIG_H
#define HW_CONFIG_H


/* Timer and PWM */
#define TIM_PWM			htim1				// PWM/ISR timer handle
#define TIM_CH_U		TIM_CHANNEL_1		// Terminal U timer channel
#define TIM_CH_V		TIM_CHANNEL_2		// Terminal V timer channel
#define TIM_CH_W		TIM_CHANNEL_3		// Terminal W timer channel
#define INVERT_DTC		0					// PWM inverting (1) or non-inverting (0)

/* ISRs */
#define PWM_ISR			TIM1_UP_TIM16_IRQn	// PWM Timer ISR
#define CAN_ISR			FDCAN1_IT0_IRQn		// CAN Receive ISR

/* ADC */

#define ADC_CH_MAIN		hadc1				// ADC channel handle which drives simultaneous mode
#define ADC_CH_IA		hadc1					// Phase A current sense ADC channel handle.  0 = unused
#define ADC_CH_IB		hadc2				// Phase B current sense ADC channel handle.  0 = unused
#define ADC_CH_IC		0				// Phase C current sense ADC channel handle.  0 = unused
#define ADC_CH_VBUS		hadc1				// Bus voltage ADC channel handle.  0 = unused

#define ENC_I2C			hi2c1
#define ENC_CPR			4096
#define INV_CPR			1.0f/ENC_CPR
#define ENC_ADDRESS		0x6C

/* Misc. GPIO */
#define LED         	GPIOC, GPIO_PIN_6	// LED Pin
#define CAN_SHDWN       GPIOC, GPIO_PIN_11  //CAN Shutdown, Set = Shutdown
#define CAN_TERM		GPIOC, GPIO_PIN_14  //Termination Resistor, Set = On
#define PWM_PIN			GPIOA, GPIO_PIN_15 //Useful GPIO output pad

#define DRV_HA			GPIOA, GPIO_PIN_8
#define DRV_HB			GPIOA, GPIO_PIN_9
#define DRV_HC			GPIOA, GPIO_PIN_10
#define DRV_LA			GPIOC, GPIO_PIN_13
#define DRV_LB			GPIOA, GPIO_PIN_12
#define DRV_LC			GPIOB, GPIO_PIN_15

/* CAN */
#define CAN_H			hfdcan1				// CAN handle

/* Other hardware-related constants */
#define I_SCALE 			-0.02937818963f  // Amps per A/D Count - Not 100% sure. Assumes 64/7 gain, 0.003ohm shunt
#define V_SCALE 			0.00837138508f    // Bus volts per A/D Count
#define DTC_MAX 			0.94f          	// Max duty cycle
#define DTC_MIN 			0.0f          	// Min duty cycle
#define DTC_COMP 			0.000f          // deadtime compensation (100 ns / 25 us)
#define DT					0.00005f		// Loop period
#define EN_ENC_LINEARIZATION 1				// Enable/disable encoder linearization

/* Current controller */
#define L_D .000003f				// D axis inductance
#define L_Q .000003f				// Q axis inductance
#define K_D .05f                    // Loop gain,  Volts/Amp
#define K_Q .05f                    // Loop gain,  Volts/Amp
#define K_SCALE 0.0001f             // K_loop/Loop BW (Hz) 0.0042
#define KI_D 0.045f                // PI zero, in radians per sample
#define KI_Q 0.045f                // PI zero, in radians per sample
#define OVERMODULATION 1.15f        // 1.0 = no overmodulation
#define CURRENT_FILT_ALPHA	.01f	// 1st order d/q current filter (not used in control)
#define VBUS_FILT_ALPHA		.1f		// 1st order bus voltage filter

#define D_INT_LIM V_BUS/(K_D*KI_D)  // Amps*samples
#define Q_INT_LIM V_BUS/(K_Q*KI_Q)  // Amps*samples


#endif
