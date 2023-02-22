################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adc.c \
../Core/Src/calibration.c \
../Core/Src/dma.c \
../Core/Src/eeprom.c \
../Core/Src/fdcan.c \
../Core/Src/foc.c \
../Core/Src/fsm.c \
../Core/Src/gatedrive.c \
../Core/Src/gpio.c \
../Core/Src/main.c \
../Core/Src/math_ops.c \
../Core/Src/opamp.c \
../Core/Src/position_sensor.c \
../Core/Src/stm32g4xx_hal_msp.c \
../Core/Src/stm32g4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g4xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c \
../Core/Src/user_config.c 

OBJS += \
./Core/Src/adc.o \
./Core/Src/calibration.o \
./Core/Src/dma.o \
./Core/Src/eeprom.o \
./Core/Src/fdcan.o \
./Core/Src/foc.o \
./Core/Src/fsm.o \
./Core/Src/gatedrive.o \
./Core/Src/gpio.o \
./Core/Src/main.o \
./Core/Src/math_ops.o \
./Core/Src/opamp.o \
./Core/Src/position_sensor.o \
./Core/Src/stm32g4xx_hal_msp.o \
./Core/Src/stm32g4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g4xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o \
./Core/Src/user_config.o 

C_DEPS += \
./Core/Src/adc.d \
./Core/Src/calibration.d \
./Core/Src/dma.d \
./Core/Src/eeprom.d \
./Core/Src/fdcan.d \
./Core/Src/foc.d \
./Core/Src/fsm.d \
./Core/Src/gatedrive.d \
./Core/Src/gpio.d \
./Core/Src/main.d \
./Core/Src/math_ops.d \
./Core/Src/opamp.d \
./Core/Src/position_sensor.d \
./Core/Src/stm32g4xx_hal_msp.d \
./Core/Src/stm32g4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g4xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d \
./Core/Src/user_config.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/calibration.d ./Core/Src/calibration.o ./Core/Src/calibration.su ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/eeprom.d ./Core/Src/eeprom.o ./Core/Src/eeprom.su ./Core/Src/fdcan.d ./Core/Src/fdcan.o ./Core/Src/fdcan.su ./Core/Src/foc.d ./Core/Src/foc.o ./Core/Src/foc.su ./Core/Src/fsm.d ./Core/Src/fsm.o ./Core/Src/fsm.su ./Core/Src/gatedrive.d ./Core/Src/gatedrive.o ./Core/Src/gatedrive.su ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/math_ops.d ./Core/Src/math_ops.o ./Core/Src/math_ops.su ./Core/Src/opamp.d ./Core/Src/opamp.o ./Core/Src/opamp.su ./Core/Src/position_sensor.d ./Core/Src/position_sensor.o ./Core/Src/position_sensor.su ./Core/Src/stm32g4xx_hal_msp.d ./Core/Src/stm32g4xx_hal_msp.o ./Core/Src/stm32g4xx_hal_msp.su ./Core/Src/stm32g4xx_it.d ./Core/Src/stm32g4xx_it.o ./Core/Src/stm32g4xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g4xx.d ./Core/Src/system_stm32g4xx.o ./Core/Src/system_stm32g4xx.su ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/user_config.d ./Core/Src/user_config.o ./Core/Src/user_config.su

.PHONY: clean-Core-2f-Src

