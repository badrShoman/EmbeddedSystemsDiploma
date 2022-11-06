################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP/DEVICE_FUNCTIONS.c \
../APP/DEVICE_SCREENS.c \
../APP/HMI_ECU.c 

OBJS += \
./APP/DEVICE_FUNCTIONS.o \
./APP/DEVICE_SCREENS.o \
./APP/HMI_ECU.o 

C_DEPS += \
./APP/DEVICE_FUNCTIONS.d \
./APP/DEVICE_SCREENS.d \
./APP/HMI_ECU.d 


# Each subdirectory must supply rules for building sources it contributes
APP/%.o: ../APP/%.c APP/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


