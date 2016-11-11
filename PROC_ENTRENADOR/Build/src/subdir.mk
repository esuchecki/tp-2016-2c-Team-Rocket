################################################################################
# ESTOS MAKEFILE FUERON MODIFICADOS!!!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/entrenador.c 

OBJS += \
./src/entrenador.o 

C_DEPS += \
./src/entrenador.d 


# Each subdirectory must supply rules for building sources it contributes
src/entrenador.o: ../src/entrenador.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/libTeamRocket" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/entrenador.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


