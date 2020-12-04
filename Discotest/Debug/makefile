################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/STM32F429I-Discovery/subdir.mk
-include Drivers/BSP/Components/wm8994/subdir.mk
-include Drivers/BSP/Components/ts3510/subdir.mk
-include Drivers/BSP/Components/stmpe811/subdir.mk
-include Drivers/BSP/Components/stmpe1600/subdir.mk
-include Drivers/BSP/Components/st7789h2/subdir.mk
-include Drivers/BSP/Components/st7735/subdir.mk
-include Drivers/BSP/Components/s5k5cag/subdir.mk
-include Drivers/BSP/Components/ov5640/subdir.mk
-include Drivers/BSP/Components/ov2640/subdir.mk
-include Drivers/BSP/Components/otm8009a/subdir.mk
-include Drivers/BSP/Components/mfxstm32l152/subdir.mk
-include Drivers/BSP/Components/lsm303dlhc/subdir.mk
-include Drivers/BSP/Components/ls016b8uy/subdir.mk
-include Drivers/BSP/Components/lis3dsh/subdir.mk
-include Drivers/BSP/Components/lis302dl/subdir.mk
-include Drivers/BSP/Components/l3gd20/subdir.mk
-include Drivers/BSP/Components/ili9341/subdir.mk
-include Drivers/BSP/Components/ili9325/subdir.mk
-include Drivers/BSP/Components/ft6x06/subdir.mk
-include Drivers/BSP/Components/exc7200/subdir.mk
-include Drivers/BSP/Components/cs43l22/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Discotest.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Discotest.list \

OBJCOPY_BIN += \
Discotest.bin \


# All Target
all: Discotest.elf secondary-outputs

# Tool invocations
Discotest.elf: $(OBJS) $(USER_OBJS) C:\00_Daten\01_Elektronik\Discotest\STM32F429ZITX_FLASH.ld
	arm-none-eabi-gcc -o "Discotest.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\00_Daten\01_Elektronik\Discotest\STM32F429ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Discotest.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Discotest.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Discotest.list"
	@echo 'Finished building: $@'
	@echo ' '

Discotest.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Discotest.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
