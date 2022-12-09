################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Command_Processor.c \
../source/PES_Final_Project.c \
../source/PWM.c \
../source/Systick.c \
../source/TSI.c \
../source/UART.c \
../source/accelerometer.c \
../source/cbfifo.c \
../source/i2c.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/test_cases.c 

C_DEPS += \
./source/Command_Processor.d \
./source/PES_Final_Project.d \
./source/PWM.d \
./source/Systick.d \
./source/TSI.d \
./source/UART.d \
./source/accelerometer.d \
./source/cbfifo.d \
./source/i2c.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/test_cases.d 

OBJS += \
./source/Command_Processor.o \
./source/PES_Final_Project.o \
./source/PWM.o \
./source/Systick.o \
./source/TSI.o \
./source/UART.o \
./source/accelerometer.o \
./source/cbfifo.o \
./source/i2c.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/test_cases.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DNDEBUG -D__REDLIB__ -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_Final_Project\board" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_Final_Project\source" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_Final_Project" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_Final_Project\drivers" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_Final_Project\CMSIS" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_Final_Project\utilities" -I"C:\Users\Shashank C\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_Final_Project\startup" -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/Command_Processor.d ./source/Command_Processor.o ./source/PES_Final_Project.d ./source/PES_Final_Project.o ./source/PWM.d ./source/PWM.o ./source/Systick.d ./source/Systick.o ./source/TSI.d ./source/TSI.o ./source/UART.d ./source/UART.o ./source/accelerometer.d ./source/accelerometer.o ./source/cbfifo.d ./source/cbfifo.o ./source/i2c.d ./source/i2c.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/test_cases.d ./source/test_cases.o

.PHONY: clean-source

