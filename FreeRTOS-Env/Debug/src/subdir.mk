################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc175x_6x-FreeRTOS.c \
../src/crp.c \
../src/display.c \
../src/main.c \
../src/retarget_printf.c \
../src/rtos-hooks.c \
../src/rtos-stat.c \
../src/task_app.c 

C_DEPS += \
./src/cr_startup_lpc175x_6x-FreeRTOS.d \
./src/crp.d \
./src/display.d \
./src/main.d \
./src/retarget_printf.d \
./src/rtos-hooks.d \
./src/rtos-stat.d \
./src/task_app.d 

OBJS += \
./src/cr_startup_lpc175x_6x-FreeRTOS.o \
./src/crp.o \
./src/display.o \
./src/main.o \
./src/retarget_printf.o \
./src/rtos-hooks.o \
./src/rtos-stat.o \
./src/task_app.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__NEWLIB__ -I"/home/guack/ISEL/6º semestre/SEIoT/CMSIS_CORE_LPC17xx/inc" -I"/home/guack/ISEL/6º semestre/SEIoT/FreeRTOS-Env/rtos/include" -I"/home/guack/ISEL/6º semestre/SEIoT/FreeRTOS-Env/rtos/src/portable/GCC/ARM_CM3" -I"/home/guack/ISEL/6º semestre/SEIoT/FreeRTOS-Env/inc" -I"/home/guack/ISEL/6º semestre/SEIoT/DRIVERS/inc" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/cr_startup_lpc175x_6x-FreeRTOS.d ./src/cr_startup_lpc175x_6x-FreeRTOS.o ./src/crp.d ./src/crp.o ./src/display.d ./src/display.o ./src/main.d ./src/main.o ./src/retarget_printf.d ./src/retarget_printf.o ./src/rtos-hooks.d ./src/rtos-hooks.o ./src/rtos-stat.d ./src/rtos-stat.o ./src/task_app.d ./src/task_app.o

.PHONY: clean-src

