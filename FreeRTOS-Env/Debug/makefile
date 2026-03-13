################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include rtos/src/portable/MemMang/subdir.mk
-include rtos/src/portable/GCC/ARM_CM3/subdir.mk
-include rtos/src/portable/Common/subdir.mk
-include rtos/src/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := FreeRTOS-Env
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: FreeRTOS-Env.axf

# Tool invocations
FreeRTOS-Env.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/home/guack/ISEL/6º semestre/SEIoT/CMSIS_CORE_LPC17xx/Debug" -L"/home/guack/ISEL/6º semestre/SEIoT/DRIVERS/Release" -Xlinker -Map="FreeRTOS-Env.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T FreeRTOS-Env_Debug.ld -u _printf_float -o "FreeRTOS-Env.axf" $(OBJS) $(USER_OBJS) $(LIBS) -lDRIVERS -lCMSIS_CORE_LPC17xx
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) FreeRTOS-Env.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "FreeRTOS-Env.axf"; # arm-none-eabi-objcopy -v -O binary "FreeRTOS-Env.axf" "FreeRTOS-Env.bin" ; # checksum -p LPC1769 -d "FreeRTOS-Env.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
