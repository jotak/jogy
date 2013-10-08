################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Geometries/IGeometryQuads.cpp \
../Geometries/IGeometryText.cpp \
../Geometries/IStencilGeometry.cpp 

OBJS += \
./Geometries/IGeometryQuads.o \
./Geometries/IGeometryText.o \
./Geometries/IStencilGeometry.o 

CPP_DEPS += \
./Geometries/IGeometryQuads.d \
./Geometries/IGeometryText.d \
./Geometries/IStencilGeometry.d 


# Each subdirectory must supply rules for building sources it contributes
Geometries/%.o: ../Geometries/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


