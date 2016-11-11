################################################################################
# ESTOS MAKEFILE FUERON MODIFICADOS!!!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/batallaPkmn.c \
../src/lib/conexiones.c \
../src/lib/deadlock.c \
../src/lib/libGrafica.c \
../src/lib/libPlanificador.c \
../src/lib/mapaConfig.c \
../src/lib/seniales.c 

OBJS += \
./src/lib/batallaPkmn.o \
./src/lib/conexiones.o \
./src/lib/deadlock.o \
./src/lib/libGrafica.o \
./src/lib/libPlanificador.o \
./src/lib/mapaConfig.o \
./src/lib/seniales.o 

C_DEPS += \
./src/lib/batallaPkmn.d \
./src/lib/conexiones.d \
./src/lib/deadlock.d \
./src/lib/libGrafica.d \
./src/lib/libPlanificador.d \
./src/lib/mapaConfig.d \
./src/lib/seniales.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/%.o: ../src/lib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/libTeamRocket"  -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


