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
all: i2c_display.axf

# Tool invocations
i2c_display.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "i2c_display.ld" -Xlinker --gc-sections -Xlinker -Map="i2c_display.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o i2c_display.axf "./CMSIS/EFM32PG1B/startup_efm32pg1b.o" "./CMSIS/EFM32PG1B/system_efm32pg1b.o" "./emlib/em_cmu.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_i2c.o" "./emlib/em_system.o" "./src/display.o" "./src/i2cspm.o" "./src/main.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: i2c_display.hex'
	arm-none-eabi-objcopy -O ihex "i2c_display.axf" "i2c_display.hex"
	@echo ' '

	@echo 'Building bin file: i2c_display.bin'
	arm-none-eabi-objcopy -O binary "i2c_display.axf" "i2c_display.bin"
	@echo ' '

	@echo 'Building s37 file: i2c_display.s37'
	arm-none-eabi-objcopy -O srec "i2c_display.axf" "i2c_display.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "i2c_display.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) i2c_display.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
