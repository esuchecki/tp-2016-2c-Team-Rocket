################################################################################
# ESTOS MAKEFILE FUERON MODIFICADOS!!!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/lib/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: entrenador

dependents:
	-cd /home/utnso/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/libTeamRocket/Build && $(MAKE) clean all

# Tool invocations
entrenador: $(OBJS) $(USER_OBJS) /home/utnso/tp-2016-2c-Team-Rocket/Build/liblibTeamRocket.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/libTeamRocket/Build" -o "entrenador" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) entrenador
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/tp-2016-2c-Team-Rocket/Build/liblibTeamRocket.so:

-include ../makefile.targets
