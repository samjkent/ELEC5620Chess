################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c 

OBJS += \
./main.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler'
	armcc -O0 --cpu=Cortex-A9.no_neon.no_vfp -g -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


