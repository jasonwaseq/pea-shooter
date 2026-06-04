#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/NweHSM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/NweHSM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/AD.c ../src/BOARD.c ../src/ES_CheckEvents.c ../src/ES_Framework.c ../src/ES_KeyboardInput.c ../src/ES_PostList.c ../src/ES_Queue.c ../src/ES_TattleTale.c ../src/ES_Timers.c ../src/IO_Ports.c ../src/LED.c ../src/RC_Servo.c ../src/pwm.c ../src/roach.c ../src/serial.c ../src/timers.c TemplateES_Main.c TemplateFSM.c TemplateHSM.c TemplateService.c TemplateSubHSM.c ../Movement.X/peashooter.c ../BeaconDetector.X/BeaconDetectorService.c ../BeaconDetector.X/BeaconEventChecker.c beacon.c ../Movement.X/SensorsEventChecker.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/AD.o ${OBJECTDIR}/_ext/1360937237/BOARD.o ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o ${OBJECTDIR}/_ext/1360937237/ES_Framework.o ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o ${OBJECTDIR}/_ext/1360937237/ES_PostList.o ${OBJECTDIR}/_ext/1360937237/ES_Queue.o ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o ${OBJECTDIR}/_ext/1360937237/ES_Timers.o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ${OBJECTDIR}/_ext/1360937237/LED.o ${OBJECTDIR}/_ext/1360937237/RC_Servo.o ${OBJECTDIR}/_ext/1360937237/pwm.o ${OBJECTDIR}/_ext/1360937237/roach.o ${OBJECTDIR}/_ext/1360937237/serial.o ${OBJECTDIR}/_ext/1360937237/timers.o ${OBJECTDIR}/TemplateES_Main.o ${OBJECTDIR}/TemplateFSM.o ${OBJECTDIR}/TemplateHSM.o ${OBJECTDIR}/TemplateService.o ${OBJECTDIR}/TemplateSubHSM.o ${OBJECTDIR}/_ext/677763416/peashooter.o ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o ${OBJECTDIR}/beacon.o ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/AD.o.d ${OBJECTDIR}/_ext/1360937237/BOARD.o.d ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d ${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d ${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d ${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d ${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d ${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d ${OBJECTDIR}/_ext/1360937237/LED.o.d ${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d ${OBJECTDIR}/_ext/1360937237/pwm.o.d ${OBJECTDIR}/_ext/1360937237/roach.o.d ${OBJECTDIR}/_ext/1360937237/serial.o.d ${OBJECTDIR}/_ext/1360937237/timers.o.d ${OBJECTDIR}/TemplateES_Main.o.d ${OBJECTDIR}/TemplateFSM.o.d ${OBJECTDIR}/TemplateHSM.o.d ${OBJECTDIR}/TemplateService.o.d ${OBJECTDIR}/TemplateSubHSM.o.d ${OBJECTDIR}/_ext/677763416/peashooter.o.d ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d ${OBJECTDIR}/beacon.o.d ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/AD.o ${OBJECTDIR}/_ext/1360937237/BOARD.o ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o ${OBJECTDIR}/_ext/1360937237/ES_Framework.o ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o ${OBJECTDIR}/_ext/1360937237/ES_PostList.o ${OBJECTDIR}/_ext/1360937237/ES_Queue.o ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o ${OBJECTDIR}/_ext/1360937237/ES_Timers.o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ${OBJECTDIR}/_ext/1360937237/LED.o ${OBJECTDIR}/_ext/1360937237/RC_Servo.o ${OBJECTDIR}/_ext/1360937237/pwm.o ${OBJECTDIR}/_ext/1360937237/roach.o ${OBJECTDIR}/_ext/1360937237/serial.o ${OBJECTDIR}/_ext/1360937237/timers.o ${OBJECTDIR}/TemplateES_Main.o ${OBJECTDIR}/TemplateFSM.o ${OBJECTDIR}/TemplateHSM.o ${OBJECTDIR}/TemplateService.o ${OBJECTDIR}/TemplateSubHSM.o ${OBJECTDIR}/_ext/677763416/peashooter.o ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o ${OBJECTDIR}/beacon.o ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o

# Source Files
SOURCEFILES=../src/AD.c ../src/BOARD.c ../src/ES_CheckEvents.c ../src/ES_Framework.c ../src/ES_KeyboardInput.c ../src/ES_PostList.c ../src/ES_Queue.c ../src/ES_TattleTale.c ../src/ES_Timers.c ../src/IO_Ports.c ../src/LED.c ../src/RC_Servo.c ../src/pwm.c ../src/roach.c ../src/serial.c ../src/timers.c TemplateES_Main.c TemplateFSM.c TemplateHSM.c TemplateService.c TemplateSubHSM.c ../Movement.X/peashooter.c ../BeaconDetector.X/BeaconDetectorService.c ../BeaconDetector.X/BeaconEventChecker.c beacon.c ../Movement.X/SensorsEventChecker.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/NweHSM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX320F128H
MP_LINKER_FILE_OPTION=,--script="..\bootloader320.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/AD.o: ../src/AD.c  .generated_files/flags/default/27e07e816ccb9e852e778ee667813eda73d385fd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/AD.o.d" -o ${OBJECTDIR}/_ext/1360937237/AD.o ../src/AD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/BOARD.o: ../src/BOARD.c  .generated_files/flags/default/2e3fa1939ebb4ffa8a82351e7f0df54031ff1761 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/BOARD.o.d" -o ${OBJECTDIR}/_ext/1360937237/BOARD.o ../src/BOARD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o: ../src/ES_CheckEvents.c  .generated_files/flags/default/67223ec4af4061f872d2d118667a6b3a1d1dddd0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o ../src/ES_CheckEvents.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Framework.o: ../src/ES_Framework.c  .generated_files/flags/default/1d8cc9fe3f802652d3c15f76369401594bb481df .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Framework.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Framework.o ../src/ES_Framework.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o: ../src/ES_KeyboardInput.c  .generated_files/flags/default/aaece14ed015b11521651a1cfca9568f3b9a2c06 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o ../src/ES_KeyboardInput.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_PostList.o: ../src/ES_PostList.c  .generated_files/flags/default/c1a28b2265005202a82f8b6a1fdb5dca882bbb43 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_PostList.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_PostList.o ../src/ES_PostList.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Queue.o: ../src/ES_Queue.c  .generated_files/flags/default/4e9142ffba3f4a44ad096a25c5539bcec9bd492c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Queue.o ../src/ES_Queue.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o: ../src/ES_TattleTale.c  .generated_files/flags/default/741e34594593d749b2da47317295ab174af6f364 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o ../src/ES_TattleTale.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Timers.o: ../src/ES_Timers.c  .generated_files/flags/default/ba1bc9edba4979f850e4e8056e558bb6211bc59d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Timers.o ../src/ES_Timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/IO_Ports.o: ../src/IO_Ports.c  .generated_files/flags/default/d66e588d1712516984ddcfe109b2a69bbbf4d35e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/IO_Ports.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d" -o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ../src/IO_Ports.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/LED.o: ../src/LED.c  .generated_files/flags/default/4acbb337f79f54df0f56004351d9e078440bc87e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/LED.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/LED.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/LED.o.d" -o ${OBJECTDIR}/_ext/1360937237/LED.o ../src/LED.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/RC_Servo.o: ../src/RC_Servo.c  .generated_files/flags/default/81e5615ec2c44be3d6efd5ccd8ac7ef7b8a8cb79 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RC_Servo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d" -o ${OBJECTDIR}/_ext/1360937237/RC_Servo.o ../src/RC_Servo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/pwm.o: ../src/pwm.c  .generated_files/flags/default/7b87fec05b4ec1344d39ef214e0e9f326c72a61c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pwm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/pwm.o.d" -o ${OBJECTDIR}/_ext/1360937237/pwm.o ../src/pwm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/roach.o: ../src/roach.c  .generated_files/flags/default/746adf06f74aebdb5a804232072be0748e7c8c8b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/roach.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/roach.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/roach.o.d" -o ${OBJECTDIR}/_ext/1360937237/roach.o ../src/roach.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/serial.o: ../src/serial.c  .generated_files/flags/default/28a93923275a867614f39b854b2921d2b7c927b2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/serial.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/serial.o.d" -o ${OBJECTDIR}/_ext/1360937237/serial.o ../src/serial.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/timers.o: ../src/timers.c  .generated_files/flags/default/a21907aed22fbe434bb2e155fe0fd56165e88590 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/timers.o ../src/timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateES_Main.o: TemplateES_Main.c  .generated_files/flags/default/b75b912f924335e3642d7c8c6a4402f959bc0d9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateES_Main.o.d 
	@${RM} ${OBJECTDIR}/TemplateES_Main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/TemplateES_Main.o.d" -o ${OBJECTDIR}/TemplateES_Main.o TemplateES_Main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateFSM.o: TemplateFSM.c  .generated_files/flags/default/260036f22d7abdf512c38aa86ffb7e22e3398e71 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateFSM.o.d 
	@${RM} ${OBJECTDIR}/TemplateFSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/TemplateFSM.o.d" -o ${OBJECTDIR}/TemplateFSM.o TemplateFSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateHSM.o: TemplateHSM.c  .generated_files/flags/default/e549843177030bfb344905ff9bfabfe3dff1d24e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateHSM.o.d 
	@${RM} ${OBJECTDIR}/TemplateHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/TemplateHSM.o.d" -o ${OBJECTDIR}/TemplateHSM.o TemplateHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateService.o: TemplateService.c  .generated_files/flags/default/81cbb759452582553f9ce7a55fe3ac17a8c655ce .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateService.o.d 
	@${RM} ${OBJECTDIR}/TemplateService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/TemplateService.o.d" -o ${OBJECTDIR}/TemplateService.o TemplateService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateSubHSM.o: TemplateSubHSM.c  .generated_files/flags/default/f2d41ecf75a66949414bb450c60933deaaa2fc15 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateSubHSM.o.d 
	@${RM} ${OBJECTDIR}/TemplateSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/TemplateSubHSM.o.d" -o ${OBJECTDIR}/TemplateSubHSM.o TemplateSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/677763416/peashooter.o: ../Movement.X/peashooter.c  .generated_files/flags/default/6089d181bee01d677e2c87c33defe02cfb5cd5e7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/677763416" 
	@${RM} ${OBJECTDIR}/_ext/677763416/peashooter.o.d 
	@${RM} ${OBJECTDIR}/_ext/677763416/peashooter.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/677763416/peashooter.o.d" -o ${OBJECTDIR}/_ext/677763416/peashooter.o ../Movement.X/peashooter.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o: ../BeaconDetector.X/BeaconDetectorService.c  .generated_files/flags/default/f1feda218426cd729d773c62dab67b02d7d7bc20 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/703160285" 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d" -o ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o ../BeaconDetector.X/BeaconDetectorService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o: ../BeaconDetector.X/BeaconEventChecker.c  .generated_files/flags/default/8b7c494f7f11399fc4f7471482cc2ed6b22589d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/703160285" 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d" -o ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o ../BeaconDetector.X/BeaconEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/beacon.o: beacon.c  .generated_files/flags/default/22eaff765aba3ae5e6c51dc272b75c77421f331f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beacon.o.d 
	@${RM} ${OBJECTDIR}/beacon.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/beacon.o.d" -o ${OBJECTDIR}/beacon.o beacon.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o: ../Movement.X/SensorsEventChecker.c  .generated_files/flags/default/11e0aa94a34402d014dbfd8c86e0d1df2ba374eb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/677763416" 
	@${RM} ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d" -o ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o ../Movement.X/SensorsEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/1360937237/AD.o: ../src/AD.c  .generated_files/flags/default/f7c1ffdf2f94ba2d15dce03d491d555a08acba70 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/AD.o.d" -o ${OBJECTDIR}/_ext/1360937237/AD.o ../src/AD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/BOARD.o: ../src/BOARD.c  .generated_files/flags/default/88b72223a0fe1f666323df9c0a57e33b9a36bdc0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/BOARD.o.d" -o ${OBJECTDIR}/_ext/1360937237/BOARD.o ../src/BOARD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o: ../src/ES_CheckEvents.c  .generated_files/flags/default/85500a69978d9b1aeba7b918fdc0bcca9eb6c8c1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o ../src/ES_CheckEvents.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Framework.o: ../src/ES_Framework.c  .generated_files/flags/default/46a9f2f73c9c197a7a18b63e5d1c96906db1a37f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Framework.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Framework.o ../src/ES_Framework.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o: ../src/ES_KeyboardInput.c  .generated_files/flags/default/d7a6c62527b22a111ee9c6f13097f9093582686c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o ../src/ES_KeyboardInput.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_PostList.o: ../src/ES_PostList.c  .generated_files/flags/default/988c4e0e40c9f7183c810a0464dbbd2b31625818 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_PostList.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_PostList.o ../src/ES_PostList.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Queue.o: ../src/ES_Queue.c  .generated_files/flags/default/66ca7d52b120e8ce9ac04cd4be62bf0f1c1f47fe .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Queue.o ../src/ES_Queue.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o: ../src/ES_TattleTale.c  .generated_files/flags/default/f7d47afafdf9d9a46e5efb1c5bd3796f6273ee36 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o ../src/ES_TattleTale.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Timers.o: ../src/ES_Timers.c  .generated_files/flags/default/ef69330a324739d9eede0b831920b2e81b9d6808 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Timers.o ../src/ES_Timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/IO_Ports.o: ../src/IO_Ports.c  .generated_files/flags/default/5f01d209d9bf21d362f6f5907e922b859e4152a0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/IO_Ports.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d" -o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ../src/IO_Ports.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/LED.o: ../src/LED.c  .generated_files/flags/default/3ff76b9d192297387c13a8602a6c28e1c11c77c5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/LED.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/LED.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/LED.o.d" -o ${OBJECTDIR}/_ext/1360937237/LED.o ../src/LED.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/RC_Servo.o: ../src/RC_Servo.c  .generated_files/flags/default/30ff453e72777214abab662f2c4191905bbc761b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RC_Servo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d" -o ${OBJECTDIR}/_ext/1360937237/RC_Servo.o ../src/RC_Servo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/pwm.o: ../src/pwm.c  .generated_files/flags/default/508e09cd1f0c21383b58003df2b0b95c5a2864ef .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pwm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/pwm.o.d" -o ${OBJECTDIR}/_ext/1360937237/pwm.o ../src/pwm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/roach.o: ../src/roach.c  .generated_files/flags/default/ddfb13b0fba1628dd75b5cd38108ee315ad9d185 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/roach.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/roach.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/roach.o.d" -o ${OBJECTDIR}/_ext/1360937237/roach.o ../src/roach.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/serial.o: ../src/serial.c  .generated_files/flags/default/605a2785357d10bf07cf28a6392f1c56ccdfc722 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/serial.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/serial.o.d" -o ${OBJECTDIR}/_ext/1360937237/serial.o ../src/serial.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/timers.o: ../src/timers.c  .generated_files/flags/default/995b89fcec735215a76b1ac93200dbc48f6140e3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/timers.o ../src/timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateES_Main.o: TemplateES_Main.c  .generated_files/flags/default/5fa622fb584bc4f4931760365e5c2ce595a6860a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateES_Main.o.d 
	@${RM} ${OBJECTDIR}/TemplateES_Main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/TemplateES_Main.o.d" -o ${OBJECTDIR}/TemplateES_Main.o TemplateES_Main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateFSM.o: TemplateFSM.c  .generated_files/flags/default/c776d968d8e89c694ec70fc92d0837bdd70cc0be .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateFSM.o.d 
	@${RM} ${OBJECTDIR}/TemplateFSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/TemplateFSM.o.d" -o ${OBJECTDIR}/TemplateFSM.o TemplateFSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateHSM.o: TemplateHSM.c  .generated_files/flags/default/29538195b8185da762810cc2ffa7f90ec38d0c31 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateHSM.o.d 
	@${RM} ${OBJECTDIR}/TemplateHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/TemplateHSM.o.d" -o ${OBJECTDIR}/TemplateHSM.o TemplateHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateService.o: TemplateService.c  .generated_files/flags/default/e4d2e789779494db50fed050df23048b98bfa026 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateService.o.d 
	@${RM} ${OBJECTDIR}/TemplateService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/TemplateService.o.d" -o ${OBJECTDIR}/TemplateService.o TemplateService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateSubHSM.o: TemplateSubHSM.c  .generated_files/flags/default/f1035ffd8bf51cd30d6304101e5d98bd048253b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateSubHSM.o.d 
	@${RM} ${OBJECTDIR}/TemplateSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/TemplateSubHSM.o.d" -o ${OBJECTDIR}/TemplateSubHSM.o TemplateSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/677763416/peashooter.o: ../Movement.X/peashooter.c  .generated_files/flags/default/c75da15856df4a123f8e96d562eff99068da7c3c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/677763416" 
	@${RM} ${OBJECTDIR}/_ext/677763416/peashooter.o.d 
	@${RM} ${OBJECTDIR}/_ext/677763416/peashooter.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/677763416/peashooter.o.d" -o ${OBJECTDIR}/_ext/677763416/peashooter.o ../Movement.X/peashooter.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o: ../BeaconDetector.X/BeaconDetectorService.c  .generated_files/flags/default/8ec7d10dba13f7a05a683e259029676af5dd6593 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/703160285" 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d" -o ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o ../BeaconDetector.X/BeaconDetectorService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o: ../BeaconDetector.X/BeaconEventChecker.c  .generated_files/flags/default/7c68f843739a517a47c855b8af877298d042463b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/703160285" 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d" -o ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o ../BeaconDetector.X/BeaconEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/beacon.o: beacon.c  .generated_files/flags/default/f97e1597c12ff062ae7b7035004b3ddd17941bc4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beacon.o.d 
	@${RM} ${OBJECTDIR}/beacon.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/beacon.o.d" -o ${OBJECTDIR}/beacon.o beacon.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o: ../Movement.X/SensorsEventChecker.c  .generated_files/flags/default/8a52356e570591a683e8285efef587cc53512415 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/677763416" 
	@${RM} ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DEVENTCHECKER_TEST -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -MP -MMD -MF "${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d" -o ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o ../Movement.X/SensorsEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/NweHSM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../bootloader320.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_SIMULATOR=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/NweHSM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/NweHSM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../bootloader320.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/NweHSM.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/NweHSM.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
