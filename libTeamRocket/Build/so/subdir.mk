################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../so/libConfig.c \
../so/libSockets.c \
../so/tiempos.c 

OBJS += \
./so/libConfig.o \
./so/libSockets.o \
./so/tiempos.o 

C_DEPS += \
./so/libConfig.d \
./so/libSockets.d \
./so/tiempos.d 


# Each subdirectory must supply rules for building sources it contributes
so/%.o: ../so/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


