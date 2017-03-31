################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := clean

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include drivers/subdir.mk
-include config/subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ELEC5620Chess.axf

# Tool invocations
ELEC5620Chess.axf: $(OBJS) /home/sam/ds5/ELEC5620Chess/config/scatter.scat $(USER_OBJS) $(LIBS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	armlink --entry=__main --cpu=Cortex-A9.no_neon.no_vfp --scatter="/home/sam/ds5/ELEC5620Chess/config/scatter.scat" --info=sizes -o "ELEC5620Chess.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(EXECUTABLES) ELEC5620Chess.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
