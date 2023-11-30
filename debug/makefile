################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-armllvm_3.2.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./ADC.o" \
"./Clock.o" \
"./DAC5.o" \
"./FIFO1.o" \
"./JoyStick.o" \
"./LED.o" \
"./Lab9Main.o" \
"./LaunchPad.o" \
"./SPI.o" \
"./ST7735.o" \
"./SmallFont.o" \
"./Sound.o" \
"./SoundData.o" \
"./Switch.o" \
"./TExaS.o" \
"./Timer.o" \
"./UART1.o" \
"./UART2.o" \
"./images/other/pipedreams/PipeSound.o" \
"./ticlang/startup_mspm0g3507_ticlang.o" \
"../mspm0g3507.cmd" \
$(GEN_CMDS__FLAG) \
-Wl,-l"/source/ti/driverlib/lib/ticlang/m0p/mspm0g1x0x_g3x0x/driverlib.a" \
-Wl,-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include images/subdir_vars.mk
-include images/other/aesprite\ files/subdir_vars.mk
-include images/other/pipedreams/subdir_vars.mk
-include ticlang/subdir_vars.mk
-include subdir_rules.mk
-include images/subdir_rules.mk
-include images/other/aesprite\ files/subdir_rules.mk
-include images/other/pipedreams/subdir_rules.mk
-include ticlang/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
ECE319K_Lab9.out \

EXE_OUTPUTS__QUOTED += \
"ECE319K_Lab9.out" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "ECE319K_Lab9.out"

# Tool invocations
ECE319K_Lab9.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-armllvm_3.2.0.LTS/bin/tiarmclang.exe" -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -D__MSPM0G3507__ -gdwarf-3 -Wl,-m"ECE319K_Lab9.map" -Wl,--heap_size=512 -Wl,--stack_size=512 -Wl,-i"C:/Users/sarah/OneDrive/Desktop/desktop/MSPM0_ValvanoWare/ECE319K_Lab9/Debug/syscfg" -Wl,-i"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-armllvm_3.2.0.LTS/lib" -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="ECE319K_Lab9_linkInfo.xml" -Wl,--rom_model -o "ECE319K_Lab9.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "ADC.o" "Clock.o" "DAC5.o" "FIFO1.o" "JoyStick.o" "LED.o" "Lab9Main.o" "LaunchPad.o" "SPI.o" "ST7735.o" "SmallFont.o" "Sound.o" "SoundData.o" "Switch.o" "TExaS.o" "Timer.o" "UART1.o" "UART2.o" "images\other\pipedreams\PipeSound.o" "ticlang\startup_mspm0g3507_ticlang.o" 
	-$(RM) "ADC.d" "Clock.d" "DAC5.d" "FIFO1.d" "JoyStick.d" "LED.d" "Lab9Main.d" "LaunchPad.d" "SPI.d" "ST7735.d" "SmallFont.d" "Sound.d" "SoundData.d" "Switch.d" "TExaS.d" "Timer.d" "UART1.d" "UART2.d" "images\other\pipedreams\PipeSound.d" "ticlang\startup_mspm0g3507_ticlang.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

