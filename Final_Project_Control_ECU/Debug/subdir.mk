################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Buzzer.c \
../Control_ECU_App.c \
../DcMotor.c \
../TWI.c \
../Timer0.c \
../Timer1.c \
../UART.c \
../external_eeprom.c \
../gpio.c 

OBJS += \
./Buzzer.o \
./Control_ECU_App.o \
./DcMotor.o \
./TWI.o \
./Timer0.o \
./Timer1.o \
./UART.o \
./external_eeprom.o \
./gpio.o 

C_DEPS += \
./Buzzer.d \
./Control_ECU_App.d \
./DcMotor.d \
./TWI.d \
./Timer0.d \
./Timer1.d \
./UART.d \
./external_eeprom.d \
./gpio.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


