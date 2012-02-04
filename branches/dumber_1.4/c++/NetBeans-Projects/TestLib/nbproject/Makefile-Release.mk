#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/2042581214/exception.o \
	${OBJECTDIR}/_ext/2044238718/toolsconvert.o \
	${OBJECTDIR}/_ext/1612523285/server.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/_ext/2042581214/camera.o \
	${OBJECTDIR}/_ext/2042581214/imageShop.o \
	${OBJECTDIR}/_ext/1612523285/message.o \
	${OBJECTDIR}/_ext/413484927/analyseimage.o \
	${OBJECTDIR}/_ext/711556677/communicationrobot.o \
	${OBJECTDIR}/_ext/2044238718/toolstime.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/testlib

dist/Release/GNU-Linux-x86/testlib: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/testlib ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/2042581214/exception.o: /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/video/exception.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2042581214
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2042581214/exception.o /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/video/exception.cpp

${OBJECTDIR}/_ext/2044238718/toolsconvert.o: /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/tools/toolsconvert.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2044238718
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2044238718/toolsconvert.o /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/tools/toolsconvert.cpp

${OBJECTDIR}/_ext/1612523285/server.o: /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/monitorCommunication/server.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1612523285
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1612523285/server.o /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/monitorCommunication/server.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/_ext/2042581214/camera.o: /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/video/camera.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2042581214
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2042581214/camera.o /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/video/camera.cpp

${OBJECTDIR}/_ext/2042581214/imageShop.o: /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/video/imageShop.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2042581214
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2042581214/imageShop.o /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/video/imageShop.cpp

${OBJECTDIR}/_ext/1612523285/message.o: /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/monitorCommunication/message.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1612523285
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1612523285/message.o /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/monitorCommunication/message.cpp

${OBJECTDIR}/_ext/413484927/analyseimage.o: /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/imageAnalyser/analyseimage.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/413484927
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/413484927/analyseimage.o /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/imageAnalyser/analyseimage.cpp

${OBJECTDIR}/_ext/711556677/communicationrobot.o: /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/robotCommunication/communicationrobot.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/711556677
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/711556677/communicationrobot.o /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/robotCommunication/communicationrobot.cpp

${OBJECTDIR}/_ext/2044238718/toolstime.o: /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/tools/toolstime.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2044238718
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2044238718/toolstime.o /Users/Piro/Documents/Implementation/DeStijl/insadestijl/c++/src/tools/toolstime.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/testlib

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
