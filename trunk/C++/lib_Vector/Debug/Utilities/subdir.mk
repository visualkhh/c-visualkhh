################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/code/CPP/BlackBox/lib_Utilities/src/Utilities.cpp 

OBJS += \
./Utilities/Utilities.o 

CPP_DEPS += \
./Utilities/Utilities.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/Utilities.o: C:/code/CPP/BlackBox/lib_Utilities/src/Utilities.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"C:\code\CPP\BlackBox\lib_Utilities\src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


