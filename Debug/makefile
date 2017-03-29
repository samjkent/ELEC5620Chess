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
all: display_test.axf

# Tool invocations
display_test.axf: $(OBJS) M:\DS-5_Workspace\display_test\scatterDDR.scat $(USER_OBJS) $(LIBS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	armlink --entry=__main --cpu=Cortex-A9.no_neon.no_vfp --scatter="M:\DS-5_Workspace\display_test\scatterDDR.scat" --info=sizes -o "display_test.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(EXECUTABLES) display_test.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
