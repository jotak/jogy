################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../guiButton.cpp \
../guiComboBox.cpp \
../guiComponent.cpp \
../guiContainer.cpp \
../guiDocument.cpp \
../guiEditBox.cpp \
../guiFrame.cpp \
../guiGauge.cpp \
../guiImage.cpp \
../guiLabel.cpp \
../guiList.cpp \
../guiObject.cpp \
../guiPopup.cpp \
../guiSmartSlider.cpp \
../guiTabbedFrame.cpp \
../guiToggleButton.cpp \
../jogy.cpp 

OBJS += \
./guiButton.o \
./guiComboBox.o \
./guiComponent.o \
./guiContainer.o \
./guiDocument.o \
./guiEditBox.o \
./guiFrame.o \
./guiGauge.o \
./guiImage.o \
./guiLabel.o \
./guiList.o \
./guiObject.o \
./guiPopup.o \
./guiSmartSlider.o \
./guiTabbedFrame.o \
./guiToggleButton.o \
./jogy.o 

CPP_DEPS += \
./guiButton.d \
./guiComboBox.d \
./guiComponent.d \
./guiContainer.d \
./guiDocument.d \
./guiEditBox.d \
./guiFrame.d \
./guiGauge.d \
./guiImage.d \
./guiLabel.d \
./guiList.d \
./guiObject.d \
./guiPopup.d \
./guiSmartSlider.d \
./guiTabbedFrame.d \
./guiToggleButton.d \
./jogy.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


