################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../FrameEffects/EffectComeIn.cpp \
../FrameEffects/EffectExtract.cpp \
../FrameEffects/guiFrameEffect.cpp \
../FrameEffects/guiFrameFlash.cpp \
../FrameEffects/guiFrameIntro.cpp \
../FrameEffects/guiFrameMouseFocus.cpp \
../FrameEffects/guiFrameOutro.cpp 

OBJS += \
./FrameEffects/EffectComeIn.o \
./FrameEffects/EffectExtract.o \
./FrameEffects/guiFrameEffect.o \
./FrameEffects/guiFrameFlash.o \
./FrameEffects/guiFrameIntro.o \
./FrameEffects/guiFrameMouseFocus.o \
./FrameEffects/guiFrameOutro.o 

CPP_DEPS += \
./FrameEffects/EffectComeIn.d \
./FrameEffects/EffectExtract.d \
./FrameEffects/guiFrameEffect.d \
./FrameEffects/guiFrameFlash.d \
./FrameEffects/guiFrameIntro.d \
./FrameEffects/guiFrameMouseFocus.d \
./FrameEffects/guiFrameOutro.d 


# Each subdirectory must supply rules for building sources it contributes
FrameEffects/%.o: ../FrameEffects/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


