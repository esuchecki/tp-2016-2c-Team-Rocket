################################################################################
# ESTOS MAKEFILE FUERON MODIFICADOS!!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/utnso/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/OSADA_FS/src/osada_functions.c 

OBJS += \
./src/osada_src/osada_functions.o 

C_DEPS += \
./src/osada_src/osada_functions.d 


# Each subdirectory must supply rules for building sources it contributes
src/osada_src/osada_functions.o: /home/utnso/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/OSADA_FS/src/osada_functions.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -D_FILE_OFFSET_BITS=64 -I"/home/utnso/tp-2016-2c-Team-Rocket/tp-2016-2c-Team-Rocket/libTeamRocket" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


