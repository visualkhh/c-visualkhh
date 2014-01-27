################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/RTTI_NEW.cpp \
../src/test.cpp \
../src/wow.cpp 

OBJS += \
./src/RTTI_NEW.o \
./src/test.o \
./src/wow.o 

CPP_DEPS += \
./src/RTTI_NEW.d \
./src/test.d \
./src/wow.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


