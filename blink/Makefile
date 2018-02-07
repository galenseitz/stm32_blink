ifeq (,$(filter _%,$(notdir $(CURDIR))))
include target.mk
else

# DEVICE definition comes from target.mk
TARGET = blink
TARGET_ARCH = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16

# Use newlib-nano. To disable it, specify USE_NANO=
USE_NANO=--specs=nano.specs
# Use seimhosting or not
USE_SEMIHOST=--specs=rdimon.specs
USE_NOHOST=--specs=nosys.specs

VPATH = $(SRCDIR)
CMSIS_PATH=$(SRCDIR)/../CMSIS

STARTUP = startup_stm32f30x.S

SRCS=	\
	blink.c

ASRCS= \
	$(STARTUP)

LD_SCRIPT = gcc.ld

DEFINES = -D$(DEVICE)
${STARTUP:.S=.o}: DEFINES += -D__STARTUP_CLEAR_BSS -D__START=main

INCLUDES = -I$(CMSIS_PATH)/Include -I$(CMSIS_PATH)/Device/ST/STM32F3xx/Include

CPPFLAGS = $(DEFINES) $(INCLUDES)
DEBUGFLAGS = -Os
debug: DEBUGFLAGS = -g -Os
CFLAGS = $(DEBUGFLAGS) -flto -ffunction-sections -fdata-sections \
	$(USE_NANO) $(USE_NOHOST)
ASFLAGS =
LDFLAGS = $(USE_NANO) $(USE_NOHOST) \
	-nostartfiles -Wl,--gc-sections,-Map=$(TARGET).map,--cref -o $@
ifneq ($(strip $(LD_SCRIPT)),)
  LDFLAGS += -Wl,-T$(SRCDIR)/$(LD_SCRIPT)
endif
OBJCOPYFLAGS = -O srec
ARFLAGS = 

CPP = arm-none-eabi-cpp
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
AR = arm-none-eabi-ar
LD = arm-none-eabi-gcc
NM = arm-none-eabi-nm
RANLIB = arm-none-eabi-ranlib
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

ABS = .elf
HEX = .srec

OBJS= ${ASRCS:.S=.o} ${SRCS:.c=.o} 


.PHONY : all
all :

.PHONY : release
release : all

.PHONY : debug
debug : all

PROGRAMS += $(TARGET).bin

all: $(PROGRAMS)

# Dedicated rule to generate hex file.
$(TARGET)$(HEX): $(TARGET)$(ABS)
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

$(TARGET).bin: $(TARGET)$(ABS)
	$(OBJCOPY) -O binary $< $@

.PHONY : clean
clean :

###############################################################################
#
# the stem meta rules
#

# Preserve intermediate files.
.PRECIOUS: %.o %$(ABS)

define BUILD_O
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c -MD -o $@ $<
	@cp $*.d $*.P; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
	rm -f $*.d
endef

%.o : %.c
	$(BUILD_O)

%.o : %.S
	$(BUILD_O)

%$(ABS) : $(LD_SCRIPT) $(OBJS)
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $(OBJS) $(LDLIBS) -o $@

%$(HEX) : %$(ABS)
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@


#----- dependencies
#
-include $(SRCS:.c=.P)
-include $(ASRCS:.S=.P)

endif
