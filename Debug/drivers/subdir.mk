################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/joystick_drv.c \
../drivers/kbd_drv.c 

OBJS += \
./drivers/joystick_drv.o \
./drivers/kbd_drv.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler'
	armcc -O0 --cpu=Cortex-A9.no_neon.no_vfp -g -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


