################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../_Linux/DirectoryChecker.cpp \
../_Linux/linux_utils.cpp \
../_Linux/main.cpp 

OBJS += \
./_Linux/DirectoryChecker.o \
./_Linux/linux_utils.o \
./_Linux/main.o 

CPP_DEPS += \
./_Linux/DirectoryChecker.d \
./_Linux/linux_utils.d \
./_Linux/main.d 


# Each subdirectory must supply rules for building sources it contributes
_Linux/%.o: ../_Linux/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jotak/dev/joson" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


