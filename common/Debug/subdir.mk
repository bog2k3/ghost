################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../DummySQLResult.cpp \
../DummySQLSock.cpp \
../SQLSock.cpp \
../cmdLine.cpp \
../configFile.cpp \
../dir.cpp \
../eMailer.cpp \
../listFile.cpp \
../log.cpp \
../sanitize.cpp \
../strCompare.cpp \
../strManip.cpp \
../wordFreq.cpp 

OBJS += \
./DummySQLResult.o \
./DummySQLSock.o \
./SQLSock.o \
./cmdLine.o \
./configFile.o \
./dir.o \
./eMailer.o \
./listFile.o \
./log.o \
./sanitize.o \
./strCompare.o \
./strManip.o \
./wordFreq.o 

CPP_DEPS += \
./DummySQLResult.d \
./DummySQLSock.d \
./SQLSock.d \
./cmdLine.d \
./configFile.d \
./dir.d \
./eMailer.d \
./listFile.d \
./log.d \
./sanitize.d \
./strCompare.d \
./strManip.d \
./wordFreq.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -DDEBUG -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


