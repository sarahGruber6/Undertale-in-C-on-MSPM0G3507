################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
sounds/%.o: ../sounds/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"F:/Dropbox/TIteachingProject/MSPM0_ValvanoWare/ECE319K_Lab9" -I"F:/Dropbox/TIteachingProject/MSPM0_ValvanoWare/ECE319K_Lab9/Debug" -I"C:/ti/mspm0_sdk_1_10_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_10_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"sounds/$(basename $(<F)).d_raw" -MT"$(@)" -I"F:/Dropbox/TIteachingProject/MSPM0_ValvanoWare/ECE319K_Lab9/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


