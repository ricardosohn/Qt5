CFLAGS_M32=-m32
CFLAGS_M64=-m64
ASM_INCLUDES = -I$(SRC_PATH)codec/common/x86/
ifneq ($(ENABLE64BIT),)
ifeq ($(ENABLE64BIT), Yes)
ARCH = x86_64
else
ARCH = x86
endif
endif
ifeq ($(ARCH), x86_64)
CFLAGS += $(CFLAGS_M64)
LDFLAGS += $(CFLAGS_M64)
ASMFLAGS_PLATFORM = -DUNIX64
else
CFLAGS += $(CFLAGS_M32)
LDFLAGS += $(CFLAGS_M32)
ASMFLAGS_PLATFORM = -DX86_32
ifeq ($(ENABLEPIC), Yes)
CFLAGS += -DX86_32_PICASM
CXXFLAGS += -DX86_32_PICASM
ASMFLAGS += -DX86_32_PICASM
endif
endif
ifeq ($(USE_ASM),Yes)
CFLAGS += -DX86_ASM
ifneq ($(ARCH), x86_64)
CFLAGS += -DX86_32_ASM
endif
ASM_ARCH = x86
endif
ASM = nasm
ASMFLAGS += $(ASMFLAGS_PLATFORM)