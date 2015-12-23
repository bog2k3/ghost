################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../housekeeping.cpp \
../main.cpp \
../matching.cpp \
../orders.cpp \
../query.cpp \
../test.cpp 

OBJS += \
./housekeeping.o \
./main.o \
./matching.o \
./orders.o \
./query.o \
./test.o 

CPP_DEPS += \
./housekeeping.d \
./main.d \
./matching.d \
./orders.d \
./query.d \
./test.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -DDEBUG -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


