################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/sharedlib/Room.cpp \
../src/sharedlib/Session.cpp \
../src/sharedlib/User.cpp 

OBJS += \
./src/sharedlib/Room.o \
./src/sharedlib/Session.o \
./src/sharedlib/User.o 

CPP_DEPS += \
./src/sharedlib/Room.d \
./src/sharedlib/Session.d \
./src/sharedlib/User.d 


# Each subdirectory must supply rules for building sources it contributes
src/sharedlib/%.o: ../src/sharedlib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/user/Desktop/FinalProject/FinalProjectMSA/msocketsLib" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


