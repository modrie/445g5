################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include CMSIS/EFM32PG1B/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SPI.axf

# Tool invocations
SPI.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "SPI.ld" -Xlinker --gc-sections -Xlinker -Map="SPI.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o SPI.axf "./CMSIS/EFM32PG1B/startup_efm32pg1b.o" "./CMSIS/EFM32PG1B/system_efm32pg1b.o" "./emlib/em_cmu.o" "./emlib/em_gpio.o" "./emlib/em_system.o" "./emlib/em_usart.o" "./src/main.o" "./src/retargetio.o" "./src/retargetswo.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: SPI.hex'
	arm-none-eabi-objcopy -O ihex "SPI.axf" "SPI.hex"
	@echo ' '

	@echo 'Building bin file: SPI.bin'
	arm-none-eabi-objcopy -O binary "SPI.axf" "SPI.bin"
	@echo ' '

	@echo 'Building s37 file: SPI.s37'
	arm-none-eabi-objcopy -O srec "SPI.axf" "SPI.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "SPI.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) SPI.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
