# -------------------------------------------------------------------------------------------------
# section: shell commands
RM := rm -rf

# verbose mode
ifeq ("$(origin V)", "command line")
	Q := 
else
	Q := @
endif

# -------------------------------------------------------------------------------------------------
# section: macros for pretty prints
define print_cc
	$(if $(Q), @echo "[CC]       $(1)")
endef

define print_bin
	$(if $(Q), @echo "[BIN]      $(1)")
endef

define print_rm
	@echo "[RM]       $(1)"
endef

# -------------------------------------------------------------------------------------------------
# section: external libraries:

# - STM LL & HAL driver
SOURCE_DIR_STM := ./externals/STM32L4xx_HAL_Driver/Src
INCLUDE_DIR_STM := ./externals/STM32L4xx_HAL_Driver/Inc
SOURCE_STM := $(wildcard $(SOURCE_DIR_STM)/*.c)
OBJECTS_STM := $(SOURCE_STM:%.c=%.o)

# - CMSIS
SOURCE_DIR_CMSIS := ./externals/CMSIS/Device/ST/STM32L4xx/Source/Templates
INCLUDE_DIR_CMSIS := ./externals/CMSIS/Core/Include ./externals/CMSIS/Device/ST/STM32L4xx/Include
SOURCE_CMSIS := $(wildcard $(SOURCE_DIR_CMSIS)/*.c)
OBJECTS_CMSIS := $(SOURCE_CMSIS:%.c=%.o)

# -------------------------------------------------------------------------------------------------
# section: products
# - bootloader C code
SOURCE_DIR_APP := ./products/bootloader/src
INCLUDE_DIR_APP := ./products/bootloader/inc
SOURCE_APP := $(wildcard $(SOURCE_DIR_APP)/*.c)
OBJECTS_APP := $(SOURCE_APP:%.c=%.o)

# - bootloader assembly code
SOURCE_DIR_STARTUP := ./products/bootloader/startup
INCLUDE_DIR_STARTUP := ./products/bootloader/startup
SOURCE_STARTUP := $(wildcard $(SOURCE_DIR_STARTUP)/*.s)
OBJECTS_STARTUP := $(SOURCE_STARTUP:%.s=%.o)

# -------------------------------------------------------------------------------------------------
# section: internal components:

# - shared_storage
SOURCE_DIR_SHARED_STORAGE := ./components/shared_storage/src
INCLUDE_DIR_SHARED_STORAGE := ./components/shared_storage/inc
SOURCE_SHARED_STORAGE := $(wildcard $(SOURCE_DIR_SHARED_STORAGE)/*.c)
OBJECTS_SHARED_STORAGE := $(SOURCE_SHARED_STORAGE:%.c=%.o)

# - boot_talk
SOURCE_DIR_BOOT_TALK := ./components/boot_talk/src
INCLUDE_DIR_BOOT_TALK := ./components/boot_talk/inc
SOURCE_BOOT_TALK := $(wildcard $(SOURCE_DIR_BOOT_TALK)/*.c)
OBJECTS_BOOT_TALK := $(SOURCE_BOOT_TALK:%.c=%.o)

# - crc
SOURCE_DIR_CRC := ./components/crc/src
INCLUDE_DIR_CRC := ./components/crc/inc
SOURCE_CRC := $(wildcard $(SOURCE_DIR_CRC)/*.c)
OBJECTS_CRC := $(SOURCE_CRC:%.c=%.o)

# - emb_assert
SOURCE_DIR_EMB_ASSERT := ./components/emb_assert/src
INCLUDE_DIR_EMB_ASSERT := ./components/emb_assert/inc
SOURCE_EMB_ASSERT := $(wildcard $(SOURCE_DIR_EMB_ASSERT)/*.c)
OBJECTS_EMB_ASSERT := $(SOURCE_EMB_ASSERT:%.c=%.o)

# -------------------------------------------------------------------------------------------------
# section: collected object files and include paths

# All object files from externals and components
OBJ := $(OBJECTS_STM) $(OBJECTS_CMSIS) $(OBJECTS_APP) \
	   $(OBJECTS_STARTUP) $(OBJECTS_BOOT_TALK) $(OBJECTS_CRC) \
	   $(OBJECTS_SHARED_STORAGE) $(OBJECTS_EMB_ASSERT)

# Collect one variable with all include directories
ALL_INCLUDES_PATHS := $(INCLUDE_DIR_STM) $(INCLUDE_DIR_CMSIS) $(INCLUDE_DIR_APP) \
					  $(INCLUDE_DIR_STARTUP) $(INCLUDE_DIR_BOOT_TALK) $(INCLUDE_DIR_CRC) \
					  $(INCLUDE_DIR_SHARED_STORAGE) $(INCLUDE_DIR_EMB_ASSERT)
INC := $(foreach d, $(ALL_INCLUDES_PATHS), -I$d)

# -------------------------------------------------------------------------------------------------
# section: compiler and linker setting
CC := arm-none-eabi-gcc

ARCHITECTURE_TARGET := -mcpu=cortex-m4 

ARCHITECTURE_FLAGS += \
	-mthumb \
	-mfpu=fpv4-sp-d16 \
	-mfloat-abi=soft \

DEFINES := -DDEBUG -DSTM32L476xx

STANDARD := -std=gnu11

DEBUG_LEVEL := -g3

OPTIMIZIATION_LEVEL := -O0

# TODO: turn on flags one by one and add pragmas in external libraries
C_WARNINGS += -Wall #-Wextra -pedantic -Wcast-align \
		  	  -Winit-self -Wlogical-op -Wmissing-include-dirs \
		  	  -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef  \
		      -Wwrite-strings -Wpointer-arith -Wmissing-declarations \
		  	  -Wuninitialized -Wold-style-definition -Wstrict-prototypes \
		      -Wmissing-prototypes -Wswitch-default -Wbad-function-cast \
		      -Wnested-externs -Wconversion -Wunreachable-code

ADDITIONAL_FLAGS := --specs=nano.specs

CC_FLAGS := -c $(ARCHITECTURE_TARGET) $(ARCHITECTURE_FLAGS) $(STANDARD) $(DEFINES) $(DEBUG_LEVEL) $(OPTIMIZIATION_LEVEL) $(C_WARNINGS) $(ADDITIONAL_FLAGS)

MAP := bootloader.map
LD_FLAGS := $(ARCHITECTURE_TARGET) -nostdlib -T ./products/bootloader/bootloader.ld -Wl,-Map=$(MAP)

# -------------------------------------------------------------------------------------------------
# section: bootloader compilation dependency
EXEC := bootloader.elf

bootloader: $(EXEC)

$(EXEC): $(OBJ)
	$(call print_bin,$@)
	$(Q)$(CC) $(LD_FLAGS) $(INC) $(OBJ) -o $@

%.o:%.c
	$(call print_cc,$<)
	$(Q)$(CC) $(CC_FLAGS) $(INC) -c $< -o $@

%.o:%.s
	$(call print_cc,$<)
	$(Q)$(CC) $(CC_FLAGS) $(INC) -c $< -o $@

# -------------------------------------------------------------------------------------------------
# section: clean removed dependency
clean:
	$(RM) $(EXEC)
	$(RM) $(MAP)
	$(RM) $(SOURCE_DIR_STM)/*.o
	$(RM) $(SOURCE_DIR_CMSIS)/*.o
	$(RM) $(SOURCE_DIR_APP)/*.o
	$(RM) $(SOURCE_DIR_STARTUP)/*.o
	$(RM) $(SOURCE_DIR_SHARED_STORAGE)/*.o
	$(RM) $(SOURCE_DIR_BOOT_TALK)/*.o
	$(RM) $(SOURCE_DIR_CRC)/*.o
	$(RM) $(SOURCE_DIR_EMB_ASSERT)/*.o
