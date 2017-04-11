################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ChessLogic/ChessBoard.c \
../ChessLogic/ChessInput.c \
../ChessLogic/ChessMoves.c \
../ChessLogic/PieceMoves.c 

OBJS += \
./ChessLogic/ChessBoard.o \
./ChessLogic/ChessInput.o \
./ChessLogic/ChessMoves.o \
./ChessLogic/PieceMoves.o 


# Each subdirectory must supply rules for building sources it contributes
ChessLogic/%.o: ../ChessLogic/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler'
	armcc -O0 --cpu=Cortex-A9.no_neon.no_vfp -g -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


