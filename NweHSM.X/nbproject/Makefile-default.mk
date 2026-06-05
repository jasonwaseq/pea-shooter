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
SOURCEFILES_QUOTED_IF_SPACED=../src/AD.c ../src/BOARD.c ../src/ES_CheckEvents.c ../src/ES_Framework.c ../src/ES_KeyboardInput.c ../src/ES_PostList.c ../src/ES_Queue.c ../src/ES_TattleTale.c ../src/ES_Timers.c ../src/IO_Ports.c ../src/LED.c ../src/RC_Servo.c ../src/pwm.c ../src/roach.c ../src/serial.c ../src/timers.c TemplateES_Main.c ../Movement.X/peashooter.c ../BeaconDetector.X/BeaconDetectorService.c ../BeaconDetector.X/BeaconEventChecker.c beacon.c ../Movement.X/SensorsEventChecker.c ../UltrasonicPing.X/ping.c TemplateSubHSM.c AlignTapeSubHSM.c AvoidObstacleSubHSM.c FindBeaconSubHSM.c FollowEdgeSubHSM.c FollowTapeSubHSM.c LauncherSubHSM.c LocateCornerSubHSM.c MainHSM.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/AD.o ${OBJECTDIR}/_ext/1360937237/BOARD.o ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o ${OBJECTDIR}/_ext/1360937237/ES_Framework.o ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o ${OBJECTDIR}/_ext/1360937237/ES_PostList.o ${OBJECTDIR}/_ext/1360937237/ES_Queue.o ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o ${OBJECTDIR}/_ext/1360937237/ES_Timers.o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ${OBJECTDIR}/_ext/1360937237/LED.o ${OBJECTDIR}/_ext/1360937237/RC_Servo.o ${OBJECTDIR}/_ext/1360937237/pwm.o ${OBJECTDIR}/_ext/1360937237/roach.o ${OBJECTDIR}/_ext/1360937237/serial.o ${OBJECTDIR}/_ext/1360937237/timers.o ${OBJECTDIR}/TemplateES_Main.o ${OBJECTDIR}/_ext/677763416/peashooter.o ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o ${OBJECTDIR}/beacon.o ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o ${OBJECTDIR}/_ext/2146788981/ping.o ${OBJECTDIR}/TemplateSubHSM.o ${OBJECTDIR}/AlignTapeSubHSM.o ${OBJECTDIR}/AvoidObstacleSubHSM.o ${OBJECTDIR}/FindBeaconSubHSM.o ${OBJECTDIR}/FollowEdgeSubHSM.o ${OBJECTDIR}/FollowTapeSubHSM.o ${OBJECTDIR}/LauncherSubHSM.o ${OBJECTDIR}/LocateCornerSubHSM.o ${OBJECTDIR}/MainHSM.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/AD.o.d ${OBJECTDIR}/_ext/1360937237/BOARD.o.d ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d ${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d ${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d ${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d ${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d ${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d ${OBJECTDIR}/_ext/1360937237/LED.o.d ${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d ${OBJECTDIR}/_ext/1360937237/pwm.o.d ${OBJECTDIR}/_ext/1360937237/roach.o.d ${OBJECTDIR}/_ext/1360937237/serial.o.d ${OBJECTDIR}/_ext/1360937237/timers.o.d ${OBJECTDIR}/TemplateES_Main.o.d ${OBJECTDIR}/_ext/677763416/peashooter.o.d ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d ${OBJECTDIR}/beacon.o.d ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d ${OBJECTDIR}/_ext/2146788981/ping.o.d ${OBJECTDIR}/TemplateSubHSM.o.d ${OBJECTDIR}/AlignTapeSubHSM.o.d ${OBJECTDIR}/AvoidObstacleSubHSM.o.d ${OBJECTDIR}/FindBeaconSubHSM.o.d ${OBJECTDIR}/FollowEdgeSubHSM.o.d ${OBJECTDIR}/FollowTapeSubHSM.o.d ${OBJECTDIR}/LauncherSubHSM.o.d ${OBJECTDIR}/LocateCornerSubHSM.o.d ${OBJECTDIR}/MainHSM.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/AD.o ${OBJECTDIR}/_ext/1360937237/BOARD.o ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o ${OBJECTDIR}/_ext/1360937237/ES_Framework.o ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o ${OBJECTDIR}/_ext/1360937237/ES_PostList.o ${OBJECTDIR}/_ext/1360937237/ES_Queue.o ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o ${OBJECTDIR}/_ext/1360937237/ES_Timers.o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ${OBJECTDIR}/_ext/1360937237/LED.o ${OBJECTDIR}/_ext/1360937237/RC_Servo.o ${OBJECTDIR}/_ext/1360937237/pwm.o ${OBJECTDIR}/_ext/1360937237/roach.o ${OBJECTDIR}/_ext/1360937237/serial.o ${OBJECTDIR}/_ext/1360937237/timers.o ${OBJECTDIR}/TemplateES_Main.o ${OBJECTDIR}/_ext/677763416/peashooter.o ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o ${OBJECTDIR}/beacon.o ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o ${OBJECTDIR}/_ext/2146788981/ping.o ${OBJECTDIR}/TemplateSubHSM.o ${OBJECTDIR}/AlignTapeSubHSM.o ${OBJECTDIR}/AvoidObstacleSubHSM.o ${OBJECTDIR}/FindBeaconSubHSM.o ${OBJECTDIR}/FollowEdgeSubHSM.o ${OBJECTDIR}/FollowTapeSubHSM.o ${OBJECTDIR}/LauncherSubHSM.o ${OBJECTDIR}/LocateCornerSubHSM.o ${OBJECTDIR}/MainHSM.o

# Source Files
SOURCEFILES=../src/AD.c ../src/BOARD.c ../src/ES_CheckEvents.c ../src/ES_Framework.c ../src/ES_KeyboardInput.c ../src/ES_PostList.c ../src/ES_Queue.c ../src/ES_TattleTale.c ../src/ES_Timers.c ../src/IO_Ports.c ../src/LED.c ../src/RC_Servo.c ../src/pwm.c ../src/roach.c ../src/serial.c ../src/timers.c TemplateES_Main.c ../Movement.X/peashooter.c ../BeaconDetector.X/BeaconDetectorService.c ../BeaconDetector.X/BeaconEventChecker.c beacon.c ../Movement.X/SensorsEventChecker.c ../UltrasonicPing.X/ping.c TemplateSubHSM.c AlignTapeSubHSM.c AvoidObstacleSubHSM.c FindBeaconSubHSM.c FollowEdgeSubHSM.c FollowTapeSubHSM.c LauncherSubHSM.c LocateCornerSubHSM.c MainHSM.c



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
${OBJECTDIR}/_ext/1360937237/AD.o: ../src/AD.c  .generated_files/flags/default/5dc39f069ea267343f88a10ab03286dad6c41464 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/AD.o.d" -o ${OBJECTDIR}/_ext/1360937237/AD.o ../src/AD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/BOARD.o: ../src/BOARD.c  .generated_files/flags/default/d284991f862785a0161d21a9b21ac4efd780c869 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/BOARD.o.d" -o ${OBJECTDIR}/_ext/1360937237/BOARD.o ../src/BOARD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o: ../src/ES_CheckEvents.c  .generated_files/flags/default/9e2ef08bf21b8f5e657eda3912fd8f6dab1c7bc1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o ../src/ES_CheckEvents.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Framework.o: ../src/ES_Framework.c  .generated_files/flags/default/841229a6fcb7d2a5d661c800eedc1be15a7fa52f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Framework.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Framework.o ../src/ES_Framework.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o: ../src/ES_KeyboardInput.c  .generated_files/flags/default/c78799cc4af54b06986c5d6d90d00319594ee44b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o ../src/ES_KeyboardInput.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_PostList.o: ../src/ES_PostList.c  .generated_files/flags/default/c337ccf634f62915c11b2629a9a6d7de9b27d372 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_PostList.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_PostList.o ../src/ES_PostList.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Queue.o: ../src/ES_Queue.c  .generated_files/flags/default/485383fee0cec77c272784f3b3b3bfe33ddcbb0b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Queue.o ../src/ES_Queue.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o: ../src/ES_TattleTale.c  .generated_files/flags/default/f84c1f51419ad618c932ae0ff76d33ec21d96fdc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o ../src/ES_TattleTale.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Timers.o: ../src/ES_Timers.c  .generated_files/flags/default/258e32b6dd55bb250234575bb62ada40983b9888 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Timers.o ../src/ES_Timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/IO_Ports.o: ../src/IO_Ports.c  .generated_files/flags/default/85dc4c2446b0d0a04b7cc725ab49d55a749858a3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/IO_Ports.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d" -o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ../src/IO_Ports.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/LED.o: ../src/LED.c  .generated_files/flags/default/232760f9430bfa370554e1c4a1dd6806b6e11b7d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/LED.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/LED.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/LED.o.d" -o ${OBJECTDIR}/_ext/1360937237/LED.o ../src/LED.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/RC_Servo.o: ../src/RC_Servo.c  .generated_files/flags/default/3df8c95b0215fca0429581d7397237482cebe9ca .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RC_Servo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d" -o ${OBJECTDIR}/_ext/1360937237/RC_Servo.o ../src/RC_Servo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/pwm.o: ../src/pwm.c  .generated_files/flags/default/ec92a96dc4911a2f5074429f1f9e770424211c16 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pwm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/pwm.o.d" -o ${OBJECTDIR}/_ext/1360937237/pwm.o ../src/pwm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/roach.o: ../src/roach.c  .generated_files/flags/default/6c7942657e9e71b385144627061eb13195f3175d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/roach.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/roach.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/roach.o.d" -o ${OBJECTDIR}/_ext/1360937237/roach.o ../src/roach.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/serial.o: ../src/serial.c  .generated_files/flags/default/bfe048a969699ade2bccd76cbe358e02b69972be .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/serial.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/serial.o.d" -o ${OBJECTDIR}/_ext/1360937237/serial.o ../src/serial.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/timers.o: ../src/timers.c  .generated_files/flags/default/d76f44ad310f0127cd56cd807fa27cec85a4027 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/timers.o ../src/timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateES_Main.o: TemplateES_Main.c  .generated_files/flags/default/7f7838f091d1377f5d315ffaeec4c758a748d3be .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateES_Main.o.d 
	@${RM} ${OBJECTDIR}/TemplateES_Main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/TemplateES_Main.o.d" -o ${OBJECTDIR}/TemplateES_Main.o TemplateES_Main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/677763416/peashooter.o: ../Movement.X/peashooter.c  .generated_files/flags/default/d248664d2998c7d9dd05e0843f7ab4d599e29633 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/677763416" 
	@${RM} ${OBJECTDIR}/_ext/677763416/peashooter.o.d 
	@${RM} ${OBJECTDIR}/_ext/677763416/peashooter.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/677763416/peashooter.o.d" -o ${OBJECTDIR}/_ext/677763416/peashooter.o ../Movement.X/peashooter.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o: ../BeaconDetector.X/BeaconDetectorService.c  .generated_files/flags/default/56e3aa2ab5cf3f9e42ed97153c46fdfe6caf81cd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/703160285" 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d" -o ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o ../BeaconDetector.X/BeaconDetectorService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o: ../BeaconDetector.X/BeaconEventChecker.c  .generated_files/flags/default/a983d29d95bafb840f01d27a3a7b8a49eff0c5f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/703160285" 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d" -o ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o ../BeaconDetector.X/BeaconEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/beacon.o: beacon.c  .generated_files/flags/default/f071aa848b26121ea7d352cc98773dd7d801420a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beacon.o.d 
	@${RM} ${OBJECTDIR}/beacon.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/beacon.o.d" -o ${OBJECTDIR}/beacon.o beacon.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o: ../Movement.X/SensorsEventChecker.c  .generated_files/flags/default/30044dc21c5fbef910b89388e0c4f8f9992acac8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/677763416" 
	@${RM} ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d" -o ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o ../Movement.X/SensorsEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2146788981/ping.o: ../UltrasonicPing.X/ping.c  .generated_files/flags/default/94bd1c7ad00f3a565ac1d856a641d7cc2a959727 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2146788981" 
	@${RM} ${OBJECTDIR}/_ext/2146788981/ping.o.d 
	@${RM} ${OBJECTDIR}/_ext/2146788981/ping.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/2146788981/ping.o.d" -o ${OBJECTDIR}/_ext/2146788981/ping.o ../UltrasonicPing.X/ping.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateSubHSM.o: TemplateSubHSM.c  .generated_files/flags/default/edb4d502c88bfd7de3a4822575d954fb9851f771 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateSubHSM.o.d 
	@${RM} ${OBJECTDIR}/TemplateSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/TemplateSubHSM.o.d" -o ${OBJECTDIR}/TemplateSubHSM.o TemplateSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/AlignTapeSubHSM.o: AlignTapeSubHSM.c  .generated_files/flags/default/64583ec1f284358e47ed60ff21d7b74cac3a3611 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AlignTapeSubHSM.o.d 
	@${RM} ${OBJECTDIR}/AlignTapeSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/AlignTapeSubHSM.o.d" -o ${OBJECTDIR}/AlignTapeSubHSM.o AlignTapeSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/AvoidObstacleSubHSM.o: AvoidObstacleSubHSM.c  .generated_files/flags/default/e47e3341b8b24404416e564bf43cabd33fc0eb12 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AvoidObstacleSubHSM.o.d 
	@${RM} ${OBJECTDIR}/AvoidObstacleSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/AvoidObstacleSubHSM.o.d" -o ${OBJECTDIR}/AvoidObstacleSubHSM.o AvoidObstacleSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/FindBeaconSubHSM.o: FindBeaconSubHSM.c  .generated_files/flags/default/26ff874734f1569d8987a510cbe0caa87d56fd6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/FindBeaconSubHSM.o.d 
	@${RM} ${OBJECTDIR}/FindBeaconSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/FindBeaconSubHSM.o.d" -o ${OBJECTDIR}/FindBeaconSubHSM.o FindBeaconSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/FollowEdgeSubHSM.o: FollowEdgeSubHSM.c  .generated_files/flags/default/c8b7784a18e7324e8c29f2863f14b95b5c25e68a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/FollowEdgeSubHSM.o.d 
	@${RM} ${OBJECTDIR}/FollowEdgeSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/FollowEdgeSubHSM.o.d" -o ${OBJECTDIR}/FollowEdgeSubHSM.o FollowEdgeSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/FollowTapeSubHSM.o: FollowTapeSubHSM.c  .generated_files/flags/default/180330918cb96fdfd67d66407e79ff1cb45e5627 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/FollowTapeSubHSM.o.d 
	@${RM} ${OBJECTDIR}/FollowTapeSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/FollowTapeSubHSM.o.d" -o ${OBJECTDIR}/FollowTapeSubHSM.o FollowTapeSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/LauncherSubHSM.o: LauncherSubHSM.c  .generated_files/flags/default/8d293e3213d9aa51a64bfcac2ae25992d41b968b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LauncherSubHSM.o.d 
	@${RM} ${OBJECTDIR}/LauncherSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/LauncherSubHSM.o.d" -o ${OBJECTDIR}/LauncherSubHSM.o LauncherSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/LocateCornerSubHSM.o: LocateCornerSubHSM.c  .generated_files/flags/default/3acf712d577056ee04b30eb2d8b6f8d71e3b7abe .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LocateCornerSubHSM.o.d 
	@${RM} ${OBJECTDIR}/LocateCornerSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/LocateCornerSubHSM.o.d" -o ${OBJECTDIR}/LocateCornerSubHSM.o LocateCornerSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/MainHSM.o: MainHSM.c  .generated_files/flags/default/f856d908025248e14ec3aaa92e1ff10b16374e9e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MainHSM.o.d 
	@${RM} ${OBJECTDIR}/MainHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/MainHSM.o.d" -o ${OBJECTDIR}/MainHSM.o MainHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/1360937237/AD.o: ../src/AD.c  .generated_files/flags/default/90d9e6585c21a555c5fb6f9e33c965067c0622dc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/AD.o.d" -o ${OBJECTDIR}/_ext/1360937237/AD.o ../src/AD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/BOARD.o: ../src/BOARD.c  .generated_files/flags/default/7f3337a952320a1341bd6d42b2c810665769442c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/BOARD.o.d" -o ${OBJECTDIR}/_ext/1360937237/BOARD.o ../src/BOARD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o: ../src/ES_CheckEvents.c  .generated_files/flags/default/e8e8b693d6e911e3f01104076b201c69180f93f7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o ../src/ES_CheckEvents.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Framework.o: ../src/ES_Framework.c  .generated_files/flags/default/8085fc52591ad44456d392f85ff7205c4af32d34 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Framework.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Framework.o ../src/ES_Framework.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o: ../src/ES_KeyboardInput.c  .generated_files/flags/default/efdb806f69bd79c10ac8d7242873cc8e17ccc79d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o ../src/ES_KeyboardInput.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_PostList.o: ../src/ES_PostList.c  .generated_files/flags/default/5b500ce0abea5dd4e01992451ef824225439cc7f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_PostList.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_PostList.o ../src/ES_PostList.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Queue.o: ../src/ES_Queue.c  .generated_files/flags/default/44f89ad8e7f55a446dd885a8971451e35f9a098d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Queue.o ../src/ES_Queue.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o: ../src/ES_TattleTale.c  .generated_files/flags/default/581263bbf3c75ab9a51b490759283ddd104596cd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o ../src/ES_TattleTale.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Timers.o: ../src/ES_Timers.c  .generated_files/flags/default/858009f94ce84fadd4f4254c77516d761e0a2ceb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Timers.o ../src/ES_Timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/IO_Ports.o: ../src/IO_Ports.c  .generated_files/flags/default/3f6c3fbc341212d4cb5799ece9b4c2254f3004e6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/IO_Ports.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d" -o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ../src/IO_Ports.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/LED.o: ../src/LED.c  .generated_files/flags/default/789a8d96d4391afdc74fa5282a7fa6554f76635e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/LED.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/LED.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/LED.o.d" -o ${OBJECTDIR}/_ext/1360937237/LED.o ../src/LED.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/RC_Servo.o: ../src/RC_Servo.c  .generated_files/flags/default/52cf40510529576222fe72ae2c09f2c3d77604e0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RC_Servo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d" -o ${OBJECTDIR}/_ext/1360937237/RC_Servo.o ../src/RC_Servo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/pwm.o: ../src/pwm.c  .generated_files/flags/default/8e9afe285025ffe9fd991545033e061bb9fb1b0b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pwm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/pwm.o.d" -o ${OBJECTDIR}/_ext/1360937237/pwm.o ../src/pwm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/roach.o: ../src/roach.c  .generated_files/flags/default/e693d3c4831f308890ad3f2cea8226ab82fbd591 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/roach.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/roach.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/roach.o.d" -o ${OBJECTDIR}/_ext/1360937237/roach.o ../src/roach.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/serial.o: ../src/serial.c  .generated_files/flags/default/6658e7b9d84cf2517c2e16bcbacb4c1b4036f0b3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/serial.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/serial.o.d" -o ${OBJECTDIR}/_ext/1360937237/serial.o ../src/serial.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/timers.o: ../src/timers.c  .generated_files/flags/default/496f192bbc8c6b60afa65c6cff51b03248dda947 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/timers.o ../src/timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateES_Main.o: TemplateES_Main.c  .generated_files/flags/default/24166f89299a8288c5a67f24f858aae26a674747 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateES_Main.o.d 
	@${RM} ${OBJECTDIR}/TemplateES_Main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/TemplateES_Main.o.d" -o ${OBJECTDIR}/TemplateES_Main.o TemplateES_Main.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/677763416/peashooter.o: ../Movement.X/peashooter.c  .generated_files/flags/default/8a1add04692d4e2d5f31f695cf978f9758dc32ad .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/677763416" 
	@${RM} ${OBJECTDIR}/_ext/677763416/peashooter.o.d 
	@${RM} ${OBJECTDIR}/_ext/677763416/peashooter.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/677763416/peashooter.o.d" -o ${OBJECTDIR}/_ext/677763416/peashooter.o ../Movement.X/peashooter.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o: ../BeaconDetector.X/BeaconDetectorService.c  .generated_files/flags/default/29cd457553b363054e6e88d883aa0b3b7e71b669 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/703160285" 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d" -o ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o ../BeaconDetector.X/BeaconDetectorService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o: ../BeaconDetector.X/BeaconEventChecker.c  .generated_files/flags/default/edfa6c3faa88ad387689f51944468a550a3f2273 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/703160285" 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d" -o ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o ../BeaconDetector.X/BeaconEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/beacon.o: beacon.c  .generated_files/flags/default/3b1d73bdd4792a89ad361a5c895c4d0b730dc59d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beacon.o.d 
	@${RM} ${OBJECTDIR}/beacon.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/beacon.o.d" -o ${OBJECTDIR}/beacon.o beacon.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o: ../Movement.X/SensorsEventChecker.c  .generated_files/flags/default/c41869ba255735c6596543692d23811b99ce673a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/677763416" 
	@${RM} ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d" -o ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o ../Movement.X/SensorsEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2146788981/ping.o: ../UltrasonicPing.X/ping.c  .generated_files/flags/default/e9775f7161080c5de906aa3719c7da763b3e826 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2146788981" 
	@${RM} ${OBJECTDIR}/_ext/2146788981/ping.o.d 
	@${RM} ${OBJECTDIR}/_ext/2146788981/ping.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/_ext/2146788981/ping.o.d" -o ${OBJECTDIR}/_ext/2146788981/ping.o ../UltrasonicPing.X/ping.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/TemplateSubHSM.o: TemplateSubHSM.c  .generated_files/flags/default/b4a6f3551d8b915507b4db0cc97d957ccc39237c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/TemplateSubHSM.o.d 
	@${RM} ${OBJECTDIR}/TemplateSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/TemplateSubHSM.o.d" -o ${OBJECTDIR}/TemplateSubHSM.o TemplateSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/AlignTapeSubHSM.o: AlignTapeSubHSM.c  .generated_files/flags/default/2d176ba31493ab2c23f6e5e8ea45d7d6e48d58e0 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AlignTapeSubHSM.o.d 
	@${RM} ${OBJECTDIR}/AlignTapeSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/AlignTapeSubHSM.o.d" -o ${OBJECTDIR}/AlignTapeSubHSM.o AlignTapeSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/AvoidObstacleSubHSM.o: AvoidObstacleSubHSM.c  .generated_files/flags/default/e14cb543c7d0a024927199895d39785f1901ac85 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AvoidObstacleSubHSM.o.d 
	@${RM} ${OBJECTDIR}/AvoidObstacleSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/AvoidObstacleSubHSM.o.d" -o ${OBJECTDIR}/AvoidObstacleSubHSM.o AvoidObstacleSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/FindBeaconSubHSM.o: FindBeaconSubHSM.c  .generated_files/flags/default/e759647afa3e21c9c315aadacd509d3c6f971bfb .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/FindBeaconSubHSM.o.d 
	@${RM} ${OBJECTDIR}/FindBeaconSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/FindBeaconSubHSM.o.d" -o ${OBJECTDIR}/FindBeaconSubHSM.o FindBeaconSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/FollowEdgeSubHSM.o: FollowEdgeSubHSM.c  .generated_files/flags/default/e037f5a7a71ee33820a965a1a4f4900f4bf23446 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/FollowEdgeSubHSM.o.d 
	@${RM} ${OBJECTDIR}/FollowEdgeSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/FollowEdgeSubHSM.o.d" -o ${OBJECTDIR}/FollowEdgeSubHSM.o FollowEdgeSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/FollowTapeSubHSM.o: FollowTapeSubHSM.c  .generated_files/flags/default/e5195c4609b738cc50336b240d673f5ad6ce1016 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/FollowTapeSubHSM.o.d 
	@${RM} ${OBJECTDIR}/FollowTapeSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/FollowTapeSubHSM.o.d" -o ${OBJECTDIR}/FollowTapeSubHSM.o FollowTapeSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/LauncherSubHSM.o: LauncherSubHSM.c  .generated_files/flags/default/84044f62b456c0614035f5ffe18d91432ea0e829 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LauncherSubHSM.o.d 
	@${RM} ${OBJECTDIR}/LauncherSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/LauncherSubHSM.o.d" -o ${OBJECTDIR}/LauncherSubHSM.o LauncherSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/LocateCornerSubHSM.o: LocateCornerSubHSM.c  .generated_files/flags/default/5c977ab7e67c925703609c5da90935e2b75011a6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LocateCornerSubHSM.o.d 
	@${RM} ${OBJECTDIR}/LocateCornerSubHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/LocateCornerSubHSM.o.d" -o ${OBJECTDIR}/LocateCornerSubHSM.o LocateCornerSubHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/MainHSM.o: MainHSM.c  .generated_files/flags/default/d5c01b365dc148f58e516dc1a2c271b3d95df0ca .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MainHSM.o.d 
	@${RM} ${OBJECTDIR}/MainHSM.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -fno-common -DRUN_HSM -I"../src" -I"../include" -I"C:/Users/asus/Documents/ece118/pea-shooter/NweHSM.X" -I"../BeaconDetector.X" -I"../Movement.X" -I"../UltrasonicPing.X" -MP -MMD -MF "${OBJECTDIR}/MainHSM.o.d" -o ${OBJECTDIR}/MainHSM.o MainHSM.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
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
