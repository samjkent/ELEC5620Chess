################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DE1-SoC_LT24.c \
../Delay.c \
../board2board.c \
../graphics_chess.c \
../main.c 

OBJS += \
./DE1-SoC_LT24.o \
./Delay.o \
./board2board.o \
./graphics_chess.o \
./main.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler'
	armcc -O0 --cpu=Cortex-A9.no_neon.no_vfp -g -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


