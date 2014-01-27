################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
C:/code/CPP/BlackBox/RTTI_NEW/src/RTTI_NEW.cpp \
C:/code/CPP/BlackBox/RTTI_NEW/src/test.cpp \
C:/code/CPP/BlackBox/RTTI_NEW/src/wow.cpp 

OBJS += \
./va/RTTI_NEW.o \
./va/test.o \
./va/wow.o 

CPP_DEPS += \
./va/RTTI_NEW.d \
./va/test.d \
./va/wow.d 


# Each subdirectory must supply rules for building sources it contributes
va/RTTI_NEW.o: C:/code/CPP/BlackBox/RTTI_NEW/src/RTTI_NEW.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"C:\code\CPP\BlackBox\RTTI_NEW\src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

va/test.o: C:/code/CPP/BlackBox/RTTI_NEW/src/test.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"C:\code\CPP\BlackBox\RTTI_NEW\src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

va/wow.o: C:/code/CPP/BlackBox/RTTI_NEW/src/wow.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -I"C:\code\CPP\BlackBox\RTTI_NEW\src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


