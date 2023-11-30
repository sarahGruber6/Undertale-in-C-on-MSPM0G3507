################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
images/other/pipedreams/%.o: ../images/other/pipedreams/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1250/ccs/tools/compiler/ti-cgt-armllvm_3.2.0.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/sarah/OneDrive/Desktop/desktop/MSPM0_ValvanoWare/ECE319K_Lab9" -I"C:/Users/sarah/OneDrive/Desktop/desktop/MSPM0_ValvanoWare/ECE319K_Lab9/Debug" -I"/source/third_party/CMSIS/Core/Include" -I"/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"images/other/pipedreams/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/sarah/OneDrive/Desktop/desktop/MSPM0_ValvanoWare/ECE319K_Lab9/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


