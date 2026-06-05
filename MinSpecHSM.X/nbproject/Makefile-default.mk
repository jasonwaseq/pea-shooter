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
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/MinSpecHSM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/MinSpecHSM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=../src/AD.c ../src/BOARD.c ../src/ES_CheckEvents.c ../src/ES_Framework.c ../src/ES_KeyboardInput.c ../src/ES_PostList.c ../src/ES_Queue.c ../src/ES_TattleTale.c ../src/ES_Timers.c ../src/IO_Ports.c ../src/LED.c ../src/pwm.c ../src/RC_Servo.c ../src/roach.c ../src/serial.c ../src/timers.c ../UltrasonicPing.X/ping.c ../Indexer.X/IndexerEventChecker.c ../Indexer.X/IndexerService.c ../BeaconDetector.X/BeaconDetectorService.c ../BeaconDetector.X/BeaconEventChecker.c ../Movement.X/peashooter.c ../Movement.X/SensorsEventChecker.c ../shooter.X/ShooterEventChecker.c ../shooter.X/ShooterService.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/AD.o ${OBJECTDIR}/_ext/1360937237/BOARD.o ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o ${OBJECTDIR}/_ext/1360937237/ES_Framework.o ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o ${OBJECTDIR}/_ext/1360937237/ES_PostList.o ${OBJECTDIR}/_ext/1360937237/ES_Queue.o ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o ${OBJECTDIR}/_ext/1360937237/ES_Timers.o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ${OBJECTDIR}/_ext/1360937237/LED.o ${OBJECTDIR}/_ext/1360937237/pwm.o ${OBJECTDIR}/_ext/1360937237/RC_Servo.o ${OBJECTDIR}/_ext/1360937237/roach.o ${OBJECTDIR}/_ext/1360937237/serial.o ${OBJECTDIR}/_ext/1360937237/timers.o ${OBJECTDIR}/_ext/2146788981/ping.o ${OBJECTDIR}/_ext/1138214266/IndexerEventChecker.o ${OBJECTDIR}/_ext/1138214266/IndexerService.o ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o ${OBJECTDIR}/_ext/677763416/peashooter.o ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o ${OBJECTDIR}/_ext/2122814055/ShooterEventChecker.o ${OBJECTDIR}/_ext/2122814055/ShooterService.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/AD.o.d ${OBJECTDIR}/_ext/1360937237/BOARD.o.d ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d ${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d ${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d ${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d ${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d ${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d ${OBJECTDIR}/_ext/1360937237/LED.o.d ${OBJECTDIR}/_ext/1360937237/pwm.o.d ${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d ${OBJECTDIR}/_ext/1360937237/roach.o.d ${OBJECTDIR}/_ext/1360937237/serial.o.d ${OBJECTDIR}/_ext/1360937237/timers.o.d ${OBJECTDIR}/_ext/2146788981/ping.o.d ${OBJECTDIR}/_ext/1138214266/IndexerEventChecker.o.d ${OBJECTDIR}/_ext/1138214266/IndexerService.o.d ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d ${OBJECTDIR}/_ext/677763416/peashooter.o.d ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d ${OBJECTDIR}/_ext/2122814055/ShooterEventChecker.o.d ${OBJECTDIR}/_ext/2122814055/ShooterService.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/AD.o ${OBJECTDIR}/_ext/1360937237/BOARD.o ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o ${OBJECTDIR}/_ext/1360937237/ES_Framework.o ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o ${OBJECTDIR}/_ext/1360937237/ES_PostList.o ${OBJECTDIR}/_ext/1360937237/ES_Queue.o ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o ${OBJECTDIR}/_ext/1360937237/ES_Timers.o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ${OBJECTDIR}/_ext/1360937237/LED.o ${OBJECTDIR}/_ext/1360937237/pwm.o ${OBJECTDIR}/_ext/1360937237/RC_Servo.o ${OBJECTDIR}/_ext/1360937237/roach.o ${OBJECTDIR}/_ext/1360937237/serial.o ${OBJECTDIR}/_ext/1360937237/timers.o ${OBJECTDIR}/_ext/2146788981/ping.o ${OBJECTDIR}/_ext/1138214266/IndexerEventChecker.o ${OBJECTDIR}/_ext/1138214266/IndexerService.o ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o ${OBJECTDIR}/_ext/677763416/peashooter.o ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o ${OBJECTDIR}/_ext/2122814055/ShooterEventChecker.o ${OBJECTDIR}/_ext/2122814055/ShooterService.o

# Source Files
SOURCEFILES=../src/AD.c ../src/BOARD.c ../src/ES_CheckEvents.c ../src/ES_Framework.c ../src/ES_KeyboardInput.c ../src/ES_PostList.c ../src/ES_Queue.c ../src/ES_TattleTale.c ../src/ES_Timers.c ../src/IO_Ports.c ../src/LED.c ../src/pwm.c ../src/RC_Servo.c ../src/roach.c ../src/serial.c ../src/timers.c ../UltrasonicPing.X/ping.c ../Indexer.X/IndexerEventChecker.c ../Indexer.X/IndexerService.c ../BeaconDetector.X/BeaconDetectorService.c ../BeaconDetector.X/BeaconEventChecker.c ../Movement.X/peashooter.c ../Movement.X/SensorsEventChecker.c ../shooter.X/ShooterEventChecker.c ../shooter.X/ShooterService.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/MinSpecHSM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX320F128H
MP_LINKER_FILE_OPTION=,--script="../bootloader320.ld"
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
${OBJECTDIR}/_ext/1360937237/AD.o: ../src/AD.c  .generated_files/flags/default/8de50e9ff61d742dd4de50b57ff9c395147f0c92 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/AD.o.d" -o ${OBJECTDIR}/_ext/1360937237/AD.o ../src/AD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/BOARD.o: ../src/BOARD.c  .generated_files/flags/default/5702c0f5d28aae9d2668af587fc17d8ae6a6fda7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/BOARD.o.d" -o ${OBJECTDIR}/_ext/1360937237/BOARD.o ../src/BOARD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o: ../src/ES_CheckEvents.c  .generated_files/flags/default/fd630504a2115fbbd0d3e182464b5b4d2d14a317 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o ../src/ES_CheckEvents.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Framework.o: ../src/ES_Framework.c  .generated_files/flags/default/8319eb2faafbe473b9b42ea6da7fd46f2dd9f637 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Framework.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Framework.o ../src/ES_Framework.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o: ../src/ES_KeyboardInput.c  .generated_files/flags/default/d7f084e1d660136aa02ec33e6a98242ca926be21 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o ../src/ES_KeyboardInput.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_PostList.o: ../src/ES_PostList.c  .generated_files/flags/default/5fdc588756b31bc26af6e4866f96044ae34782bf .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_PostList.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_PostList.o ../src/ES_PostList.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Queue.o: ../src/ES_Queue.c  .generated_files/flags/default/927bc582178d7e33cce86b5862ecad4fb88ee31f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Queue.o ../src/ES_Queue.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o: ../src/ES_TattleTale.c  .generated_files/flags/default/4ee5e3280b9c086cc22d5766e3437fc1c7ac0b0d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o ../src/ES_TattleTale.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Timers.o: ../src/ES_Timers.c  .generated_files/flags/default/c3a5d74932f817543b8e0b9981a62f008d9abbb3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Timers.o ../src/ES_Timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/IO_Ports.o: ../src/IO_Ports.c  .generated_files/flags/default/86d824ac298e82036cded6a261e36b6752870d77 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/IO_Ports.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d" -o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ../src/IO_Ports.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/LED.o: ../src/LED.c  .generated_files/flags/default/fff0d3f7ef3bfc511babad9166a10eb6b7cbce34 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/LED.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/LED.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/LED.o.d" -o ${OBJECTDIR}/_ext/1360937237/LED.o ../src/LED.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/pwm.o: ../src/pwm.c  .generated_files/flags/default/42d706f567f9c202dd5dad56d8c17ef81c04e22 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pwm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/pwm.o.d" -o ${OBJECTDIR}/_ext/1360937237/pwm.o ../src/pwm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/RC_Servo.o: ../src/RC_Servo.c  .generated_files/flags/default/86882583d9041276793aa3d80dfec7f16f177728 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RC_Servo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d" -o ${OBJECTDIR}/_ext/1360937237/RC_Servo.o ../src/RC_Servo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/roach.o: ../src/roach.c  .generated_files/flags/default/27a1aaaaa2f880607fd5f08d5515a2afdf71d7be .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/roach.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/roach.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/roach.o.d" -o ${OBJECTDIR}/_ext/1360937237/roach.o ../src/roach.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/serial.o: ../src/serial.c  .generated_files/flags/default/d34be03e48603bea403256a302ff9aff5392ed94 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/serial.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/serial.o.d" -o ${OBJECTDIR}/_ext/1360937237/serial.o ../src/serial.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/timers.o: ../src/timers.c  .generated_files/flags/default/6105a786d146c39f4fce3c2614bc70f04511ef96 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/timers.o ../src/timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2146788981/ping.o: ../UltrasonicPing.X/ping.c  .generated_files/flags/default/d86cfb8fd3177d42b83c9f62675b253a5212d8ec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2146788981" 
	@${RM} ${OBJECTDIR}/_ext/2146788981/ping.o.d 
	@${RM} ${OBJECTDIR}/_ext/2146788981/ping.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2146788981/ping.o.d" -o ${OBJECTDIR}/_ext/2146788981/ping.o ../UltrasonicPing.X/ping.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1138214266/IndexerEventChecker.o: ../Indexer.X/IndexerEventChecker.c  .generated_files/flags/default/937d99f7cd157ccb1b138938b831722de94dddfa .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1138214266" 
	@${RM} ${OBJECTDIR}/_ext/1138214266/IndexerEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/1138214266/IndexerEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1138214266/IndexerEventChecker.o.d" -o ${OBJECTDIR}/_ext/1138214266/IndexerEventChecker.o ../Indexer.X/IndexerEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1138214266/IndexerService.o: ../Indexer.X/IndexerService.c  .generated_files/flags/default/abf9042591c4fa84e1446626b40cb1be73784a5d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1138214266" 
	@${RM} ${OBJECTDIR}/_ext/1138214266/IndexerService.o.d 
	@${RM} ${OBJECTDIR}/_ext/1138214266/IndexerService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1138214266/IndexerService.o.d" -o ${OBJECTDIR}/_ext/1138214266/IndexerService.o ../Indexer.X/IndexerService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o: ../BeaconDetector.X/BeaconDetectorService.c  .generated_files/flags/default/d83e313164a002a05e80d9db576fef8127be37f5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/703160285" 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d" -o ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o ../BeaconDetector.X/BeaconDetectorService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o: ../BeaconDetector.X/BeaconEventChecker.c  .generated_files/flags/default/91daf80bb9cafb4016ce62a6d486e381bd0175dc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/703160285" 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d" -o ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o ../BeaconDetector.X/BeaconEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/677763416/peashooter.o: ../Movement.X/peashooter.c  .generated_files/flags/default/96214373dad75f9994a1dceb051cde350bd97c18 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/677763416" 
	@${RM} ${OBJECTDIR}/_ext/677763416/peashooter.o.d 
	@${RM} ${OBJECTDIR}/_ext/677763416/peashooter.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/677763416/peashooter.o.d" -o ${OBJECTDIR}/_ext/677763416/peashooter.o ../Movement.X/peashooter.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o: ../Movement.X/SensorsEventChecker.c  .generated_files/flags/default/602fca76bd1e33b827620b5b4aa01bb55bfb2459 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/677763416" 
	@${RM} ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d" -o ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o ../Movement.X/SensorsEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2122814055/ShooterEventChecker.o: ../shooter.X/ShooterEventChecker.c  .generated_files/flags/default/3b0dd0658dd8b450ed5065051a19a5a6d3d98301 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2122814055" 
	@${RM} ${OBJECTDIR}/_ext/2122814055/ShooterEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/2122814055/ShooterEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2122814055/ShooterEventChecker.o.d" -o ${OBJECTDIR}/_ext/2122814055/ShooterEventChecker.o ../shooter.X/ShooterEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2122814055/ShooterService.o: ../shooter.X/ShooterService.c  .generated_files/flags/default/36e0c401a796bb5bddf9a02c0f1f5dcc7669fd12 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2122814055" 
	@${RM} ${OBJECTDIR}/_ext/2122814055/ShooterService.o.d 
	@${RM} ${OBJECTDIR}/_ext/2122814055/ShooterService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2122814055/ShooterService.o.d" -o ${OBJECTDIR}/_ext/2122814055/ShooterService.o ../shooter.X/ShooterService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/_ext/1360937237/AD.o: ../src/AD.c  .generated_files/flags/default/59ed5cb6850a1c76009114c4790da43540da77d8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/AD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/AD.o.d" -o ${OBJECTDIR}/_ext/1360937237/AD.o ../src/AD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/BOARD.o: ../src/BOARD.c  .generated_files/flags/default/1b3a3435cb03ba39e7ac1c6dcf71b74b5ead1f8d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/BOARD.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/BOARD.o.d" -o ${OBJECTDIR}/_ext/1360937237/BOARD.o ../src/BOARD.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o: ../src/ES_CheckEvents.c  .generated_files/flags/default/c48867623a4bc6f67d50db2ad7d8db3c4d7ab450 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_CheckEvents.o ../src/ES_CheckEvents.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Framework.o: ../src/ES_Framework.c  .generated_files/flags/default/a445532042f71afe893124bd3d1ede4330d8bdb6 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Framework.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Framework.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Framework.o ../src/ES_Framework.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o: ../src/ES_KeyboardInput.c  .generated_files/flags/default/bdde0a99b46f8a28f521584c2f086d8d73215db5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_KeyboardInput.o ../src/ES_KeyboardInput.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_PostList.o: ../src/ES_PostList.c  .generated_files/flags/default/19db8b628595b8c2547e63a1eb7735ea3e2f6597 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_PostList.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_PostList.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_PostList.o ../src/ES_PostList.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Queue.o: ../src/ES_Queue.c  .generated_files/flags/default/e2c2189203ad07b371f5b499c3c445aa698beb2b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Queue.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Queue.o ../src/ES_Queue.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o: ../src/ES_TattleTale.c  .generated_files/flags/default/5b19f5b794533e95198561147fa1cc84caec8fca .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_TattleTale.o ../src/ES_TattleTale.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/ES_Timers.o: ../src/ES_Timers.c  .generated_files/flags/default/2b0e21464c69c2fcaaf3eb4d0aa2e865a3ca4196 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ES_Timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/ES_Timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/ES_Timers.o ../src/ES_Timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/IO_Ports.o: ../src/IO_Ports.c  .generated_files/flags/default/d2520b32e622c3611600395daee6135f30e1afcd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/IO_Ports.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d" -o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ../src/IO_Ports.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/LED.o: ../src/LED.c  .generated_files/flags/default/ff2d03d6fd434965f79b3c792b9c0faa7e11273 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/LED.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/LED.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/LED.o.d" -o ${OBJECTDIR}/_ext/1360937237/LED.o ../src/LED.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/pwm.o: ../src/pwm.c  .generated_files/flags/default/c58e85f2f1ac052702402a10723b54892c01b00b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pwm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/pwm.o.d" -o ${OBJECTDIR}/_ext/1360937237/pwm.o ../src/pwm.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/RC_Servo.o: ../src/RC_Servo.c  .generated_files/flags/default/59c47e094b6d6c616eacdb8d1c4343fcc330471b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/RC_Servo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/RC_Servo.o.d" -o ${OBJECTDIR}/_ext/1360937237/RC_Servo.o ../src/RC_Servo.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/roach.o: ../src/roach.c  .generated_files/flags/default/143597534031e7e2f7345d46dbb67555625b0797 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/roach.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/roach.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/roach.o.d" -o ${OBJECTDIR}/_ext/1360937237/roach.o ../src/roach.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/serial.o: ../src/serial.c  .generated_files/flags/default/ce43fe899316ab017798ab4405d4a9172ec9ab36 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/serial.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/serial.o.d" -o ${OBJECTDIR}/_ext/1360937237/serial.o ../src/serial.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1360937237/timers.o: ../src/timers.c  .generated_files/flags/default/bc1e32ab980eafefab5b8d44a03f3f82f1b0346a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timers.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/timers.o ../src/timers.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2146788981/ping.o: ../UltrasonicPing.X/ping.c  .generated_files/flags/default/fa7d9daba0128693d25bae88211365cd4987de50 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2146788981" 
	@${RM} ${OBJECTDIR}/_ext/2146788981/ping.o.d 
	@${RM} ${OBJECTDIR}/_ext/2146788981/ping.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2146788981/ping.o.d" -o ${OBJECTDIR}/_ext/2146788981/ping.o ../UltrasonicPing.X/ping.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1138214266/IndexerEventChecker.o: ../Indexer.X/IndexerEventChecker.c  .generated_files/flags/default/e1721f6b9994f2020b7a445d8b009496e1db059f .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1138214266" 
	@${RM} ${OBJECTDIR}/_ext/1138214266/IndexerEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/1138214266/IndexerEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1138214266/IndexerEventChecker.o.d" -o ${OBJECTDIR}/_ext/1138214266/IndexerEventChecker.o ../Indexer.X/IndexerEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/1138214266/IndexerService.o: ../Indexer.X/IndexerService.c  .generated_files/flags/default/452cdb25f4a944ae32c95a57d17a9df93f868369 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1138214266" 
	@${RM} ${OBJECTDIR}/_ext/1138214266/IndexerService.o.d 
	@${RM} ${OBJECTDIR}/_ext/1138214266/IndexerService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1138214266/IndexerService.o.d" -o ${OBJECTDIR}/_ext/1138214266/IndexerService.o ../Indexer.X/IndexerService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o: ../BeaconDetector.X/BeaconDetectorService.c  .generated_files/flags/default/d44198d5059ebbe0f668c30cdf553c4bf6e1e8d1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/703160285" 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o.d" -o ${OBJECTDIR}/_ext/703160285/BeaconDetectorService.o ../BeaconDetector.X/BeaconDetectorService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o: ../BeaconDetector.X/BeaconEventChecker.c  .generated_files/flags/default/585ef74145436cfcdb8d1f1dc68a2c201890a192 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/703160285" 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o.d" -o ${OBJECTDIR}/_ext/703160285/BeaconEventChecker.o ../BeaconDetector.X/BeaconEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/677763416/peashooter.o: ../Movement.X/peashooter.c  .generated_files/flags/default/7235004600bf8854406c901dde9251fbbe0d69b7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/677763416" 
	@${RM} ${OBJECTDIR}/_ext/677763416/peashooter.o.d 
	@${RM} ${OBJECTDIR}/_ext/677763416/peashooter.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/677763416/peashooter.o.d" -o ${OBJECTDIR}/_ext/677763416/peashooter.o ../Movement.X/peashooter.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o: ../Movement.X/SensorsEventChecker.c  .generated_files/flags/default/43518315061a81068abc11c5ee931b067dc64e4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/677763416" 
	@${RM} ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o.d" -o ${OBJECTDIR}/_ext/677763416/SensorsEventChecker.o ../Movement.X/SensorsEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2122814055/ShooterEventChecker.o: ../shooter.X/ShooterEventChecker.c  .generated_files/flags/default/66c063dd6943fb5c6b453aa8671e2e3ec1de1ff2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2122814055" 
	@${RM} ${OBJECTDIR}/_ext/2122814055/ShooterEventChecker.o.d 
	@${RM} ${OBJECTDIR}/_ext/2122814055/ShooterEventChecker.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2122814055/ShooterEventChecker.o.d" -o ${OBJECTDIR}/_ext/2122814055/ShooterEventChecker.o ../shooter.X/ShooterEventChecker.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/_ext/2122814055/ShooterService.o: ../shooter.X/ShooterService.c  .generated_files/flags/default/a79e7baa4086b57d38e6e2f92dc309c1f9f23859 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/2122814055" 
	@${RM} ${OBJECTDIR}/_ext/2122814055/ShooterService.o.d 
	@${RM} ${OBJECTDIR}/_ext/2122814055/ShooterService.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"." -I"../src" -I"../include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/2122814055/ShooterService.o.d" -o ${OBJECTDIR}/_ext/2122814055/ShooterService.o ../shooter.X/ShooterService.c    -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/MinSpecHSM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../bootloader320.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_SIMULATOR=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/MinSpecHSM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,--defsym=_min_heap_size=0,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/MinSpecHSM.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../bootloader320.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/MinSpecHSM.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=0,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}/xc32-bin2hex ${DISTDIR}/MinSpecHSM.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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
