################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/XMLParser.cpp 

OBJS += \
./src/XMLParser.o 

CPP_DEPS += \
./src/XMLParser.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -IC:/cygwin/usr/include/libxml2 -IC:/cygwin/usr/include/libxml2/libxml -I"Z:\code.google\c-visualkhh\C++\lib_Utilities\src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


