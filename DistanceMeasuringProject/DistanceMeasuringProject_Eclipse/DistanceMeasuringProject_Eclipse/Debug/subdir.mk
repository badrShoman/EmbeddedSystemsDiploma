################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../distance_measuring_project.c \
../gpio.c \
../icu.c \
../lcd.c \
../timer.c \
../ultrasonic_four_terminal_sensor.c 

OBJS += \
./distance_measuring_project.o \
./gpio.o \
./icu.o \
./lcd.o \
./timer.o \
./ultrasonic_four_terminal_sensor.o 

C_DEPS += \
./distance_measuring_project.d \
./gpio.d \
./icu.d \
./lcd.d \
./timer.d \
./ultrasonic_four_terminal_sensor.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


