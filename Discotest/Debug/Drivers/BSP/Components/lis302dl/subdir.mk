################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/lis302dl/lis302dl.c 

OBJS += \
./Drivers/BSP/Components/lis302dl/lis302dl.o 

C_DEPS += \
./Drivers/BSP/Components/lis302dl/lis302dl.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/lis302dl/lis302dl.o: ../Drivers/BSP/Components/lis302dl/lis302dl.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/00_Daten/01_Elektronik/Discotest/Drivers/BSP/STM32F429I-Discovery" -I"C:/00_Daten/01_Elektronik/Discotest/Utilities" -I"C:/00_Daten/01_Elektronik/Discotest/Drivers/BSP/Components/ili9341" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/Components/lis302dl/lis302dl.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

