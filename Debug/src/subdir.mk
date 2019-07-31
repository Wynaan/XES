################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc175x_6x.cpp \
../src/engine.cpp \
../src/entities.cpp \
../src/game.cpp \
../src/geometry.cpp \
../src/gui.cpp \
../src/image.cpp \
../src/irqhandlers.cpp \
../src/lpc_xav.cpp \
../src/m_obj.cpp \
../src/main.cpp \
../src/map.cpp \
../src/ps2controller.cpp \
../src/renderer.cpp \
../src/savemanager.cpp \
../src/segdisplay.cpp \
../src/text.cpp 

C_SRCS += \
../src/crp.c \
../src/sysinit.c 

OBJS += \
./src/cr_cpp_config.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/engine.o \
./src/entities.o \
./src/game.o \
./src/geometry.o \
./src/gui.o \
./src/image.o \
./src/irqhandlers.o \
./src/lpc_xav.o \
./src/m_obj.o \
./src/main.o \
./src/map.o \
./src/ps2controller.o \
./src/renderer.o \
./src/savemanager.o \
./src/segdisplay.o \
./src/sysinit.o \
./src/text.o 

CPP_DEPS += \
./src/cr_cpp_config.d \
./src/cr_startup_lpc175x_6x.d \
./src/engine.d \
./src/entities.d \
./src/game.d \
./src/geometry.d \
./src/gui.d \
./src/image.d \
./src/irqhandlers.d \
./src/lpc_xav.d \
./src/m_obj.d \
./src/main.d \
./src/map.d \
./src/ps2controller.d \
./src/renderer.d \
./src/savemanager.d \
./src/segdisplay.d \
./src/text.d 

C_DEPS += \
./src/crp.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC17XX__ -I"C:\Users\xavie\Cegep\Processeurs\HeroOrZero\inc" -I"C:\Users\xavie\Cegep\Processeurs\HeroOrZero\resource" -I"C:\Users\xavie\Cegep\Processeurs\lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Users\xavie\Cegep\Processeurs\lpc_chip_175x_6x\inc" -O0 -fno-common -g3 -Wall -Wa,-a,-ad="$*.lst" -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC17XX__ -D__NEWLIB__ -I"C:\Users\xavie\Cegep\Processeurs\HeroOrZero\inc" -I"C:\Users\xavie\Cegep\Processeurs\lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Users\xavie\Cegep\Processeurs\lpc_chip_175x_6x\inc" -O0 -fno-common -g3 -Wall -c -Wa,-a,-ad="$*.lst" -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


