# Microsoft Developer Studio Project File - Name="SimulationManagement" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=SimulationManagement - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SimulationManagement.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SimulationManagement.mak" CFG="SimulationManagement - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SimulationManagement - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "SimulationManagement - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RRM/Scheduler/source/schWorkspace/SimulationManagement", IJAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SimulationManagement - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /Ot /Og /Oi /Op /Ob1 /Gf /Gy /I "../common" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D _WIN32_WINNT=0x0400 /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release/Simulation.lib"

!ELSEIF  "$(CFG)" == "SimulationManagement - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /Z7 /Od /I "../common" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D _WIN32_WINNT=0x0400 /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug/Simulation.lib"

!ENDIF 

# Begin Target

# Name "SimulationManagement - Win32 Release"
# Name "SimulationManagement - Win32 Debug"
# Begin Source File

SOURCE=.\Clause.h
# End Source File
# Begin Source File

SOURCE=.\ClauseType.h
# End Source File
# Begin Source File

SOURCE=.\ControlProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlProcess.h
# End Source File
# Begin Source File

SOURCE=.\CountTime.cpp
# End Source File
# Begin Source File

SOURCE=.\CountTime.h
# End Source File
# Begin Source File

SOURCE=.\PositionType.h
# End Source File
# Begin Source File

SOURCE=.\ProcessList.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessList.h
# End Source File
# Begin Source File

SOURCE=.\ProcessState.h
# End Source File
# Begin Source File

SOURCE=.\SimProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\SimProcess.h
# End Source File
# Begin Source File

SOURCE=.\Simulation.h
# End Source File
# Begin Source File

SOURCE=.\SimulationManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SimulationManager.h
# End Source File
# Begin Source File

SOURCE=.\SingleSimulationManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SingleSimulationManager.h
# End Source File
# Begin Source File

SOURCE=.\Statistics.h
# End Source File
# End Target
# End Project
