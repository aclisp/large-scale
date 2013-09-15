# Microsoft Developer Studio Project File - Name="StatisticsCalculation" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=StatisticsCalculation - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "StatisticsCalculation.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "StatisticsCalculation.mak" CFG="StatisticsCalculation - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "StatisticsCalculation - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "StatisticsCalculation - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RRM/Scheduler/source/schWorkspace/StatisticsCalculation", VJAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "StatisticsCalculation - Win32 Release"

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
# ADD CPP /nologo /G5 /MT /W3 /GX /Ot /Og /Oi /Op /Ob1 /Gf /Gy /I "../common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"Release/StatsCalc.pch" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Release/StatsCalc.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\StatsCalc.lib"

!ELSEIF  "$(CFG)" == "StatisticsCalculation - Win32 Debug"

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
# ADD CPP /nologo /G5 /MTd /W3 /GX /Z7 /Od /I "../common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"Debug/StatsCalc.pch" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Debug/StatsCalc.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\StatsCalc.lib"

!ENDIF 

# Begin Target

# Name "StatisticsCalculation - Win32 Release"
# Name "StatisticsCalculation - Win32 Debug"
# Begin Source File

SOURCE=.\ContinuousHistogram.cpp
# End Source File
# Begin Source File

SOURCE=.\ContinuousHistogram.h
# End Source File
# Begin Source File

SOURCE=.\CumulativeHistogram.cpp
# End Source File
# Begin Source File

SOURCE=.\CumulativeHistogram.h
# End Source File
# Begin Source File

SOURCE=.\DescriptiveStatistic.cpp
# End Source File
# Begin Source File

SOURCE=.\DescriptiveStatistic.h
# End Source File
# Begin Source File

SOURCE=.\DiscreteHistogram.cpp
# End Source File
# Begin Source File

SOURCE=.\DiscreteHistogram.h
# End Source File
# Begin Source File

SOURCE=.\HighPrecisionWinAvg.cpp
# End Source File
# Begin Source File

SOURCE=.\HighPrecisionWinAvg.h
# End Source File
# Begin Source File

SOURCE=.\Marker.h
# End Source File
# Begin Source File

SOURCE=.\Mean.cpp
# End Source File
# Begin Source File

SOURCE=.\Mean.h
# End Source File
# Begin Source File

SOURCE=.\Quantile.cpp
# End Source File
# Begin Source File

SOURCE=.\Quantile.h
# End Source File
# Begin Source File

SOURCE=.\StatsCalc.h
# End Source File
# Begin Source File

SOURCE=.\WindowedAverage.cpp
# End Source File
# Begin Source File

SOURCE=.\WindowedAverage.h
# End Source File
# End Target
# End Project
