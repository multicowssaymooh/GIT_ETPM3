################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/ov5640/ov5640.c 

OBJS += \
./Drivers/BSP/Components/ov5640/ov5640.o 

C_DEPS += \
./Drivers/BSP/Components/ov5640/ov5640.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/ov5640/ov5640.o: ../Drivers/BSP/Components/ov5640/ov5640.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/00_Daten/01_Elektronik/Discotest/Drivers/BSP/STM32F429I-Discovery" -I"C:/00_Daten/01_Elektronik/Discotest/Utilities" -I"C:/00_Daten/01_Elektronik/Discotest/Drivers/BSP/Components/ili9341" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/Components/ov5640/ov5640.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

