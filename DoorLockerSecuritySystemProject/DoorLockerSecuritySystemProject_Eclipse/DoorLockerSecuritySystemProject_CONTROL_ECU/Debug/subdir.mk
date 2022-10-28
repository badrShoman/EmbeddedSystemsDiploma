################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CONTROL_ECU.c \
../buzzer.c \
../dc_motor.c \
../external_eeprom.c \
../gpio.c \
../i2c.c \
../timer.c \
../usart.c 

OBJS += \
./CONTROL_ECU.o \
./buzzer.o \
./dc_motor.o \
./external_eeprom.o \
./gpio.o \
./i2c.o \
./timer.o \
./usart.o 

C_DEPS += \
./CONTROL_ECU.d \
./buzzer.d \
./dc_motor.d \
./external_eeprom.d \
./gpio.d \
./i2c.d \
./timer.d \
./usart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


