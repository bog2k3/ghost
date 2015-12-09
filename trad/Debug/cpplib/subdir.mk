################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/bog/work/ghost/cpplib/dir.cpp \
/home/bog/work/ghost/cpplib/log.cpp 

OBJS += \
./cpplib/dir.o \
./cpplib/log.o 

CPP_DEPS += \
./cpplib/dir.d \
./cpplib/log.d 


# Each subdirectory must supply rules for building sources it contributes
cpplib/dir.o: /home/bog/work/ghost/cpplib/dir.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -DDEBUG -I/home/bog/work/ghost/3rd-party/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

cpplib/log.o: /home/bog/work/ghost/cpplib/log.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -DDEBUG -I/home/bog/work/ghost/3rd-party/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


