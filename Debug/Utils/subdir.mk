################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Utils/TimeController.cpp \
../Utils/md5.cpp \
../Utils/utils.cpp 

OBJS += \
./Utils/TimeController.o \
./Utils/md5.o \
./Utils/utils.o 

CPP_DEPS += \
./Utils/TimeController.d \
./Utils/md5.d \
./Utils/utils.d 


# Each subdirectory must supply rules for building sources it contributes
Utils/%.o: ../Utils/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jotak/dev/joson" -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


