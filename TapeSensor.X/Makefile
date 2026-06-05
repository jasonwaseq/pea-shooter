#
# Project Makefile for TapeSensor.X.
#

MP_EXTRA_CC_PRE += -I. -I../include
export PATH := C:/Program Files/Microchip/MPLABX/v6.30/gnuBins/GnuWin32/bin:$(PATH)

PROJECTNAME = TapeSensor.X
CND_CONF = default
IMAGE_TYPE = production
OUTPUT_SUFFIX = hex
DEBUGGABLE_SUFFIX = elf
OBJECTDIR = build/$(CND_CONF)/$(IMAGE_TYPE)
DISTDIR = dist/$(CND_CONF)/$(IMAGE_TYPE)

MP_PROCESSOR_OPTION = 32MX320F128H
MP_LINKER_FILE_OPTION = ,--script="..\bootloader320.ld"

LOCAL_SOURCES = main.c TapeSensorService.c TapeSensorEventChecker.c
FRAMEWORK_SOURCES = \
	../src/AD.c \
	../src/BOARD.c \
	../src/ES_CheckEvents.c \
	../src/ES_Framework.c \
	../src/ES_KeyboardInput.c \
	../src/ES_PostList.c \
	../src/ES_Queue.c \
	../src/ES_TattleTale.c \
	../src/ES_Timers.c \
	../src/serial.c \
	../src/timers.c

LOCAL_OBJECTS = $(addprefix $(OBJECTDIR)/,$(LOCAL_SOURCES:.c=.o))
FRAMEWORK_OBJECTS = $(addprefix $(OBJECTDIR)/_ext/1360937237/,$(notdir $(FRAMEWORK_SOURCES:.c=.o)))
OBJECTS = $(LOCAL_OBJECTS) $(FRAMEWORK_OBJECTS)

.PHONY: build clean clobber all

build: $(DISTDIR)/$(PROJECTNAME).$(IMAGE_TYPE).$(OUTPUT_SUFFIX)

all: build

$(DISTDIR)/$(PROJECTNAME).$(IMAGE_TYPE).$(OUTPUT_SUFFIX): $(OBJECTS) ../bootloader320.ld
	@gnumkdir -p $(DISTDIR)
	$(MP_CC) $(MP_EXTRA_LD_PRE) -mprocessor=$(MP_PROCESSOR_OPTION) -o $(DISTDIR)/$(PROJECTNAME).$(IMAGE_TYPE).$(DEBUGGABLE_SUFFIX) $(OBJECTS) -DXPRJ_default=$(CND_CONF) -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="$(DISTDIR)/$(PROJECTNAME).$(IMAGE_TYPE).map",--memorysummary,$(DISTDIR)/memoryfile.xml -mdfp="$(DFP_DIR)"
	$(MP_CC_DIR)\xc32-bin2hex $(DISTDIR)/$(PROJECTNAME).$(IMAGE_TYPE).$(DEBUGGABLE_SUFFIX)

$(OBJECTDIR)/%.o: %.c ES_Configure.h TapeSensorEventChecker.h TapeSensorService.h
	@gnumkdir -p $(OBJECTDIR)
	@rm -f $@.d $@
	$(MP_CC) $(MP_EXTRA_CC_PRE) -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O0 -fno-common -MP -MMD -MF "$@.d" -o $@ $< -DXPRJ_default=$(CND_CONF) -mdfp="$(DFP_DIR)"

$(OBJECTDIR)/_ext/1360937237/%.o: ../src/%.c ES_Configure.h TapeSensorEventChecker.h TapeSensorService.h
	@gnumkdir -p $(OBJECTDIR)/_ext/1360937237
	@rm -f $@.d $@
	$(MP_CC) $(MP_EXTRA_CC_PRE) -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O0 -fno-common -MP -MMD -MF "$@.d" -o $@ $< -DXPRJ_default=$(CND_CONF) -mdfp="$(DFP_DIR)"

clean:
	rm -f -r $(OBJECTDIR)
	rm -f -r $(DISTDIR)

clobber: clean

include nbproject/Makefile-local-default.mk
