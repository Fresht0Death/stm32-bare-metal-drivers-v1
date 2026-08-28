PROJECT := examples2_4
CONFIG ?= debug
BUILD_DIR := build/$(CONFIG)

C_SOURCES := \
	Src/adc.c \
	Src/gpio.c \
	Src/interrupt.c \
	Src/main.c \
	Src/pwm.c \
	Src/stm_err.c \
	Src/syscalls.c \
	Src/sysmem.c \
	Src/timer.c \
	Src/uart.c 

TOOLCHAIN_BIN ?= /Applications/STM32CubeIDE.app/Contents/Eclipse/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.14.3.rel1.macos64_1.0.100.202602081740/tools/bin

CC := $(TOOLCHAIN_BIN)/arm-none-eabi-gcc
OBJCOPY := $(TOOLCHAIN_BIN)/arm-none-eabi-objcopy
SIZE := $(TOOLCHAIN_BIN)/arm-none-eabi-size
LD_TOOL := $(TOOLCHAIN_BIN)/arm-none-eabi-ld

STM32CUBE_F4_ROOT ?= third_party/STM32CubeF4

ARCH_FLAGS := \
	-mcpu=cortex-m4 \
	-mfpu=fpv4-sp-d16 \
	-mfloat-abi=hard \
	-mthumb
	
	
ASFLAGS := \
	$(ARCH_FLAGS) \
	-x assembler-with-cpp \
	-MMD \
	-MP \
	--specs=nano.specs

CPPFLAGS := \
	-DSTM32F446xx \
	-DSTM32 \
	-DSTM32F4 \
	-DSTM32F446RETx \
	-IInc \
	-I$(STM32CUBE_F4_ROOT)/Drivers/CMSIS/Include \
	-I$(STM32CUBE_F4_ROOT)/Drivers/CMSIS/Device/ST/STM32F4xx/Include

CFLAGS := \
	$(ARCH_FLAGS) \
	-std=gnu11 \
	-ffunction-sections \
	-fdata-sections \
	-Wall \
	-fstack-usage \
	-fcyclomatic-complexity \
	-MMD \
	-MP \
	--specs=nano.specs

$(BUILD_DIR)/%.o: %.s
	@mkdir -p "$(@D)"
	$(CC) $(ASFLAGS) -c "$<" -o "$@"
	

ifeq ($(CONFIG),debug)
CPPFLAGS += -DDEBUG
CFLAGS += -O0 -g3
ASFLAGS += -g3 -DDEBUG
else ifeq ($(CONFIG),release)
CFLAGS += -Os -Werror=date-time
else ifeq ($(CONFIG),release)
CFLAGS += -Os
else
$(error Unknown CONFIG '$(CONFIG)'; use debug or release)
endif
	


ASM_SOURCES := \
	Startup/startup_stm32f446retx.s

C_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
ASM_OBJECTS := $(patsubst %.s,$(BUILD_DIR)/%.o,$(ASM_SOURCES))
OBJECTS := $(C_OBJECTS) $(ASM_OBJECTS)

DEP_FILES := $(OBJECTS:.o=.d)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p "$(@D)"
	$(CC) $(CPPFLAGS) $(CFLAGS) -c "$<" -o "$@"

-include $(DEP_FILES)

ELF := $(BUILD_DIR)/$(PROJECT).elf
MAP := $(BUILD_DIR)/$(PROJECT).map
LINKER_SCRIPT := STM32F446RETX_FLASH.ld

BIN := $(BUILD_DIR)/$(PROJECT).bin
HEX := $(BUILD_DIR)/$(PROJECT).hex

$(BIN): $(ELF)
	$(OBJCOPY) -O binary "$<" "$@"

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex "$<" "$@"
	
all: $(ELF) $(BIN) $(HEX)

size: $(ELF)
	$(SIZE) "$<"
	
LDFLAGS := \
	$(ARCH_FLAGS) \
	-T$(LINKER_SCRIPT) \
	--specs=nosys.specs \
	--specs=nano.specs \
	-static \
	-Wl,--gc-sections \
	-Wl,-Map=$(MAP)

LDLIBS := \
	-Wl,--start-group \
	-lc \
	-lm \
	-Wl,--end-group
	
$(ELF): $(OBJECTS) $(LINKER_SCRIPT)
	@mkdir -p "$(@D)"
	$(CC) $(OBJECTS) $(LDFLAGS) $(LDLIBS) -o "$@"
	
.DEFAULT_GOAL := all

clean:
	$(RM) -r build

objects: $(OBJECTS)

EXPECTED_GCC_BANNER := arm-none-eabi-gcc (GNU Tools for STM32 14.3.rel1.20251027-0700) 14.3.1 20250623
EXPECTED_LD_BANNER := GNU ld (GNU Tools for STM32 14.3.rel1.20251027-0700) 2.44.0.20250616
EXPECTED_OBJCOPY_BANNER := GNU objcopy (GNU Tools for STM32 14.3.rel1.20251027-0700) 2.44.0.20250616

.PHONY: all objects show-objects size clean check-toolchain release

release: check-toolchain
	$(MAKE) clean
	$(MAKE) CONFIG=release all
	$(MAKE) CONFIG=release size
	@cd build/release && shasum -a 256 $(PROJECT).elf $(PROJECT).bin $(PROJECT).hex $(PROJECT).map > SHA256SUMS
	@cat build/release/SHA256SUMS

check-toolchain:
	@$(CC) --version | sed -n '1p' | grep -Fqx '$(EXPECTED_GCC_BANNER)' || { echo "Unexpected compiler"; exit 1; }
	@$(LD_TOOL) --version | sed -n '1p' | grep -Fqx '$(EXPECTED_LD_BANNER)' || { echo "Unexpected linker"; exit 1; }
	@$(OBJCOPY) --version | sed -n '1p' | grep -Fqx '$(EXPECTED_OBJCOPY_BANNER)' || { echo "Unexpected objcopy"; exit 1; }
	@echo "Toolchain matches the v1 reference"

show-objects:
	@printf '%s\n' $(OBJECTS)