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
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=${DISTDIR}/ds_dehumi2021.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=${DISTDIR}/ds_dehumi2021.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Main.c Buzzer.c Crc16.c Dehumi.c Eeprom.c Rs485.c Segment.c Timer.c Testmode.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Main.p1 ${OBJECTDIR}/Buzzer.p1 ${OBJECTDIR}/Crc16.p1 ${OBJECTDIR}/Dehumi.p1 ${OBJECTDIR}/Eeprom.p1 ${OBJECTDIR}/Rs485.p1 ${OBJECTDIR}/Segment.p1 ${OBJECTDIR}/Timer.p1 ${OBJECTDIR}/Testmode.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/Main.p1.d ${OBJECTDIR}/Buzzer.p1.d ${OBJECTDIR}/Crc16.p1.d ${OBJECTDIR}/Dehumi.p1.d ${OBJECTDIR}/Eeprom.p1.d ${OBJECTDIR}/Rs485.p1.d ${OBJECTDIR}/Segment.p1.d ${OBJECTDIR}/Timer.p1.d ${OBJECTDIR}/Testmode.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Main.p1 ${OBJECTDIR}/Buzzer.p1 ${OBJECTDIR}/Crc16.p1 ${OBJECTDIR}/Dehumi.p1 ${OBJECTDIR}/Eeprom.p1 ${OBJECTDIR}/Rs485.p1 ${OBJECTDIR}/Segment.p1 ${OBJECTDIR}/Timer.p1 ${OBJECTDIR}/Testmode.p1

# Source Files
SOURCEFILES=Main.c Buzzer.c Crc16.c Dehumi.c Eeprom.c Rs485.c Segment.c Timer.c Testmode.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/ds_dehumi2021.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=16F946
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Main.p1: Main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Main.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Main.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Main.p1: > ${OBJECTDIR}/Main.p1.d
	@cat ${OBJECTDIR}/Main.dep >> ${OBJECTDIR}/Main.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Main.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Buzzer.p1: Buzzer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Buzzer.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Buzzer.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Buzzer.p1: > ${OBJECTDIR}/Buzzer.p1.d
	@cat ${OBJECTDIR}/Buzzer.dep >> ${OBJECTDIR}/Buzzer.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Buzzer.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Crc16.p1: Crc16.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Crc16.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Crc16.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Crc16.p1: > ${OBJECTDIR}/Crc16.p1.d
	@cat ${OBJECTDIR}/Crc16.dep >> ${OBJECTDIR}/Crc16.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Crc16.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Dehumi.p1: Dehumi.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Dehumi.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Dehumi.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Dehumi.p1: > ${OBJECTDIR}/Dehumi.p1.d
	@cat ${OBJECTDIR}/Dehumi.dep >> ${OBJECTDIR}/Dehumi.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Dehumi.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Eeprom.p1: Eeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Eeprom.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Eeprom.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Eeprom.p1: > ${OBJECTDIR}/Eeprom.p1.d
	@cat ${OBJECTDIR}/Eeprom.dep >> ${OBJECTDIR}/Eeprom.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Eeprom.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Rs485.p1: Rs485.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Rs485.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Rs485.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Rs485.p1: > ${OBJECTDIR}/Rs485.p1.d
	@cat ${OBJECTDIR}/Rs485.dep >> ${OBJECTDIR}/Rs485.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Rs485.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Segment.p1: Segment.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Segment.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Segment.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Segment.p1: > ${OBJECTDIR}/Segment.p1.d
	@cat ${OBJECTDIR}/Segment.dep >> ${OBJECTDIR}/Segment.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Segment.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Timer.p1: Timer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Timer.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Timer.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Timer.p1: > ${OBJECTDIR}/Timer.p1.d
	@cat ${OBJECTDIR}/Timer.dep >> ${OBJECTDIR}/Timer.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Timer.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Testmode.p1: Testmode.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Testmode.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Testmode.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Testmode.p1: > ${OBJECTDIR}/Testmode.p1.d
	@cat ${OBJECTDIR}/Testmode.dep >> ${OBJECTDIR}/Testmode.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Testmode.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/Main.p1: Main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Main.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Main.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Main.p1: > ${OBJECTDIR}/Main.p1.d
	@cat ${OBJECTDIR}/Main.dep >> ${OBJECTDIR}/Main.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Main.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Buzzer.p1: Buzzer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Buzzer.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Buzzer.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Buzzer.p1: > ${OBJECTDIR}/Buzzer.p1.d
	@cat ${OBJECTDIR}/Buzzer.dep >> ${OBJECTDIR}/Buzzer.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Buzzer.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Crc16.p1: Crc16.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Crc16.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Crc16.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Crc16.p1: > ${OBJECTDIR}/Crc16.p1.d
	@cat ${OBJECTDIR}/Crc16.dep >> ${OBJECTDIR}/Crc16.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Crc16.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Dehumi.p1: Dehumi.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Dehumi.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Dehumi.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Dehumi.p1: > ${OBJECTDIR}/Dehumi.p1.d
	@cat ${OBJECTDIR}/Dehumi.dep >> ${OBJECTDIR}/Dehumi.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Dehumi.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Eeprom.p1: Eeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Eeprom.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Eeprom.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Eeprom.p1: > ${OBJECTDIR}/Eeprom.p1.d
	@cat ${OBJECTDIR}/Eeprom.dep >> ${OBJECTDIR}/Eeprom.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Eeprom.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Rs485.p1: Rs485.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Rs485.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Rs485.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Rs485.p1: > ${OBJECTDIR}/Rs485.p1.d
	@cat ${OBJECTDIR}/Rs485.dep >> ${OBJECTDIR}/Rs485.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Rs485.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Segment.p1: Segment.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Segment.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Segment.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Segment.p1: > ${OBJECTDIR}/Segment.p1.d
	@cat ${OBJECTDIR}/Segment.dep >> ${OBJECTDIR}/Segment.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Segment.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Timer.p1: Timer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Timer.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Timer.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Timer.p1: > ${OBJECTDIR}/Timer.p1.d
	@cat ${OBJECTDIR}/Timer.dep >> ${OBJECTDIR}/Timer.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Timer.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/Testmode.p1: Testmode.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	${MP_CC} --pass1 Testmode.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@${MP_CC} --scandep  Testmode.c $(MP_EXTRA_CC_PRE) -q --chip=$(MP_PROCESSOR_OPTION) -P  --outdir="${OBJECTDIR}" -P -N31 --warn=0 --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --opt=default,+asm,-asmfile,-speed,+space,-debug,9  --double=32 --float=32 --addrqual=ignore --mode=pro -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"
	@echo ${OBJECTDIR}/Testmode.p1: > ${OBJECTDIR}/Testmode.p1.d
	@cat ${OBJECTDIR}/Testmode.dep >> ${OBJECTDIR}/Testmode.p1.d
	@${FIXDEPS} "${OBJECTDIR}/Testmode.p1.d" $(SILENT) -ht  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/ds_dehumi2021.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_LD} $(MP_EXTRA_LD_PRE) -o${DISTDIR}/ds_dehumi2021.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  -m${DISTDIR}/ds_dehumi2021.X.${IMAGE_TYPE}.map --summary=default,-psect,-class,+mem,-hex --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -D__DEBUG --debugger=pickit3 -P -N31 --warn=0  --double=32 --float=32 --addrqual=ignore --mode=pro --output=default,-inhx032 -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" ${OBJECTFILES_QUOTED_IF_SPACED}  
	@${RM} ${DISTDIR}/ds_dehumi2021.X.${IMAGE_TYPE}.hex
else
${DISTDIR}/ds_dehumi2021.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_LD} $(MP_EXTRA_LD_PRE) -o${DISTDIR}/ds_dehumi2021.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -m${DISTDIR}/ds_dehumi2021.X.${IMAGE_TYPE}.map --summary=default,-psect,-class,+mem,-hex --chip=$(MP_PROCESSOR_OPTION) -P --runtime=default,+clear,+init,-keep,+osccal,-resetbits,-download,-stackcall,+clib --summary=default,-psect,-class,+mem,-hex --opt=default,+asm,-asmfile,-speed,+space,-debug,9 -P -N31 --warn=0  --double=32 --float=32 --addrqual=ignore --mode=pro --output=default,-inhx032 -g --asmlist "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" ${OBJECTFILES_QUOTED_IF_SPACED}  
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

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
