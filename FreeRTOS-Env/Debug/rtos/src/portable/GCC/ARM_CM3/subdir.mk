################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rtos/src/portable/GCC/ARM_CM3/port.c 

C_DEPS += \
./rtos/src/portable/GCC/ARM_CM3/port.d 

OBJS += \
./rtos/src/portable/GCC/ARM_CM3/port.o 


# Each subdirectory must supply rules for building sources it contributes
rtos/src/portable/GCC/ARM_CM3/%.o: ../rtos/src/portable/GCC/ARM_CM3/%.c rtos/src/portable/GCC/ARM_CM3/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__NEWLIB__ -I"/home/guack/ISEL/6º semestre/SEIoT/CMSIS_CORE_LPC17xx/inc" -I"/home/guack/ISEL/6º semestre/SEIoT/FreeRTOS-Env/rtos/include" -I"/home/guack/ISEL/6º semestre/SEIoT/FreeRTOS-Env/rtos/src/portable/GCC/ARM_CM3" -I"/home/guack/ISEL/6º semestre/SEIoT/FreeRTOS-Env/inc" -I"/home/guack/ISEL/6º semestre/SEIoT/DRIVERS/inc" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-rtos-2f-src-2f-portable-2f-GCC-2f-ARM_CM3

clean-rtos-2f-src-2f-portable-2f-GCC-2f-ARM_CM3:
	-$(RM) ./rtos/src/portable/GCC/ARM_CM3/port.d ./rtos/src/portable/GCC/ARM_CM3/port.o

.PHONY: clean-rtos-2f-src-2f-portable-2f-GCC-2f-ARM_CM3

