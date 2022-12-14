################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/i2c.c \
../source/led.c \
../source/main.c \
../source/mma8451q.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/tpm.c 

C_DEPS += \
./source/i2c.d \
./source/led.d \
./source/main.d \
./source/mma8451q.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/tpm.d 

OBJS += \
./source/i2c.o \
./source/led.o \
./source/main.o \
./source/mma8451q.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/tpm.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DPRINTF_FLOAT_ENABLE=0 -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\SpatialDimmer\board" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\SpatialDimmer\source" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\SpatialDimmer" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\SpatialDimmer\drivers" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\SpatialDimmer\CMSIS" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\SpatialDimmer\utilities" -I"C:\Users\dayton.flores\Documents\MCUXpressoIDE_11.6.0_8187\workspace\SpatialDimmer\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/i2c.d ./source/i2c.o ./source/led.d ./source/led.o ./source/main.d ./source/main.o ./source/mma8451q.d ./source/mma8451q.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/tpm.d ./source/tpm.o

.PHONY: clean-source

