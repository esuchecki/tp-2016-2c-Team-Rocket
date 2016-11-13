################################################################################
# ESTOS MAKEFILE FUERON MODIFICADOS!!!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/abortarEntrenador.c \
../src/lib/batallaPkmn.c \
../src/lib/entrenadorConfig.c \
../src/lib/estadisticas.c \
../src/lib/libConfigStruct.c \
../src/lib/movimiento.c \
../src/lib/seniales.c 

OBJS += \
./src/lib/abortarEntrenador.o \
./src/lib/batallaPkmn.o \
./src/lib/entrenadorConfig.o \
./src/lib/estadisticas.o \
./src/lib/libConfigStruct.o \
./src/lib/movimiento.o \
./src/lib/seniales.o 

C_DEPS += \
./src/lib/abortarEntrenador.d \
./src/lib/batallaPkmn.d \
./src/lib/entrenadorConfig.d \
./src/lib/estadisticas.d \
./src/lib/libConfigStruct.d \
./src/lib/movimiento.d \
./src/lib/seniales.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/%.o: ../src/lib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/libTeamRocket" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


