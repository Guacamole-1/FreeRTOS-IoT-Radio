################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rtos/src/croutine.c \
../rtos/src/event_groups.c \
../rtos/src/list.c \
../rtos/src/queue.c \
../rtos/src/stream_buffer.c \
../rtos/src/tasks.c \
../rtos/src/timers.c 

C_DEPS += \
./rtos/src/croutine.d \
./rtos/src/event_groups.d \
./rtos/src/list.d \
./rtos/src/queue.d \
./rtos/src/stream_buffer.d \
./rtos/src/tasks.d \
./rtos/src/timers.d 

OBJS += \
./rtos/src/croutine.o \
./rtos/src/event_groups.o \
./rtos/src/list.o \
./rtos/src/queue.o \
./rtos/src/stream_buffer.o \
./rtos/src/tasks.o \
./rtos/src/timers.o 


# Each subdirectory must supply rules for building sources it contributes
rtos/src/%.o: ../rtos/src/%.c rtos/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC17xx -D__LPC17XX__ -D__NEWLIB__ -I"/home/guack/ISEL/6º semestre/SEIoT/CMSIS_CORE_LPC17xx/inc" -I"/home/guack/ISEL/6º semestre/SEIoT/FreeRTOS-Env/rtos/include" -I"/home/guack/ISEL/6º semestre/SEIoT/FreeRTOS-Env/rtos/src/portable/GCC/ARM_CM3" -I"/home/guack/ISEL/6º semestre/SEIoT/FreeRTOS-Env/inc" -I"/home/guack/ISEL/6º semestre/SEIoT/DRIVERS/inc" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-rtos-2f-src

clean-rtos-2f-src:
	-$(RM) ./rtos/src/croutine.d ./rtos/src/croutine.o ./rtos/src/event_groups.d ./rtos/src/event_groups.o ./rtos/src/list.d ./rtos/src/list.o ./rtos/src/queue.d ./rtos/src/queue.o ./rtos/src/stream_buffer.d ./rtos/src/stream_buffer.o ./rtos/src/tasks.d ./rtos/src/tasks.o ./rtos/src/timers.d ./rtos/src/timers.o

.PHONY: clean-rtos-2f-src

