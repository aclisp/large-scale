# Microsoft Developer Studio Project File - Name="SchedulerTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SchedulerTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SchedulerTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SchedulerTest.mak" CFG="SchedulerTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SchedulerTest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SchedulerTest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RRM/Scheduler/source/schWorkspace/SchedulerTest", WNAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SchedulerTest - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /Ot /Og /Oi /Op /Ob1 /Gf /Gy /I "..\common" /I "..\StatisticsCalculation" /I "..\RandomVariableGeneration" /I "..\SimulationManagement" /I "..\TraceAndLog" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib RandomGen.lib simulation.lib statscalc.lib trclog.lib /nologo /subsystem:console /machine:I386 /out:"Release/SchTest.exe" /libpath:"../SimulationManagement/Release" /libpath:"../TraceAndLog/Release" /libpath:"../StatisticsCalculation/Release" /libpath:"../RandomVariableGeneration/Release"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SchedulerTest - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /I "..\common" /I "..\StatisticsCalculation" /I "..\SimulationManagement" /I "..\TraceAndLog" /I "..\RandomVariableGeneration" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /I /Op".)\RandomVariableGeneration" /c
# ADD BASE RSC /l 0x416 /d "_DEBUG"
# ADD RSC /l 0x416 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib RandomGen.lib simulation.lib StatsCalc.lib trclog.lib /nologo /subsystem:console /debug /machine:I386 /out:"Debug/SchTest.exe" /pdbtype:sept /libpath:"../SimulationManagement/Debug" /libpath:"../TraceAndLog/Debug" /libpath:"../StatisticsCalculation/Debug" /libpath:"../RandomVariableGeneration/Debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "SchedulerTest - Win32 Release"
# Name "SchedulerTest - Win32 Debug"
# Begin Source File

SOURCE=.\Attendant.cpp
# End Source File
# Begin Source File

SOURCE=.\Attendant.h
# End Source File
# Begin Source File

SOURCE=.\bank.cpp
# End Source File
# Begin Source File

SOURCE=.\Bank.h
# End Source File
# Begin Source File

SOURCE=.\Client.cpp
# End Source File
# Begin Source File

SOURCE=.\Client.h
# End Source File
# Begin Source File

SOURCE=.\ClientGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientGenerator.h
# End Source File
# Begin Source File

SOURCE=.\SimulationSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\SimulationSystem.h
# End Source File
# End Target
# End Project
