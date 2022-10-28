################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../adc.c \
../dc_motor.c \
../fan_controller_project.c \
../gpio.c \
../lcd.c \
../lm35_three_terminal_sensor.c \
../timer.c 

OBJS += \
./adc.o \
./dc_motor.o \
./fan_controller_project.o \
./gpio.o \
./lcd.o \
./lm35_three_terminal_sensor.o \
./timer.o 

C_DEPS += \
./adc.d \
./dc_motor.d \
./fan_controller_project.d \
./gpio.d \
./lcd.d \
./lm35_three_terminal_sensor.d \
./timer.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


