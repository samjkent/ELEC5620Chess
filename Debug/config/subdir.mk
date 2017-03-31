################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../config/exceptions.c \
../config/setup_interrupts.c 

S_SRCS += \
../config/vectors.s 

OBJS += \
./config/exceptions.o \
./config/setup_interrupts.o \
./config/vectors.o 


# Each subdirectory must supply rules for building sources it contributes
config/%.o: ../config/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler'
	armcc -O0 --cpu=Cortex-A9.no_neon.no_vfp -g -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

config/%.o: ../config/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Assembler'
	armasm --cpu=Cortex-A9.no_neon.no_vfp -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


