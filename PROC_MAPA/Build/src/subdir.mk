################################################################################
# ESTOS MAKEFILE FUERON MODIFICADOS!!!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/mapa.c 

OBJS += \
./src/mapa.o 

C_DEPS += \
./src/mapa.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/libTeamRocket"  -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


