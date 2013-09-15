# Microsoft Developer Studio Project File - Name="RandomVariableGeneration" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=RandomVariableGeneration - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RandomVariableGeneration.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RandomVariableGeneration.mak" CFG="RandomVariableGeneration - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RandomVariableGeneration - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "RandomVariableGeneration - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RRM/Scheduler/source/schWorkspace/RandomVariableGeneration", HIAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RandomVariableGeneration - Win32 Release"

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
# ADD CPP /nologo /G5 /MT /W3 /GX /Ot /Og /Oi /Op /Ob1 /Gf /Gy /I "../common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\RandomGen.lib"

!ELSEIF  "$(CFG)" == "RandomVariableGeneration - Win32 Debug"

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
# ADD CPP /nologo /G5 /MTd /W3 /GX /Z7 /Od /I "../common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\RandomGen.lib"

!ENDIF 

# Begin Target

# Name "RandomVariableGeneration - Win32 Release"
# Name "RandomVariableGeneration - Win32 Debug"
# Begin Source File

SOURCE=.\BaseDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseDistribution.h
# End Source File
# Begin Source File

SOURCE=.\BaseGen.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseGen.h
# End Source File
# Begin Source File

SOURCE=.\BetaGen.cpp
# End Source File
# Begin Source File

SOURCE=.\BetaGen.h
# End Source File
# Begin Source File

SOURCE=.\BinomialGen.cpp
# End Source File
# Begin Source File

SOURCE=.\BinomialGen.h
# End Source File
# Begin Source File

SOURCE=.\BinomialGenBtpec.cpp
# End Source File
# Begin Source File

SOURCE=.\BinomialGenBtpec.h
# End Source File
# Begin Source File

SOURCE=.\BooleanDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\BooleanDistribution.h
# End Source File
# Begin Source File

SOURCE=.\CauchyGen.cpp
# End Source File
# Begin Source File

SOURCE=.\CauchyGen.h
# End Source File
# Begin Source File

SOURCE=.\ContinuousDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\ContinuousDistribution.h
# End Source File
# Begin Source File

SOURCE=.\DiscreteDistribution.cpp
# End Source File
# Begin Source File

SOURCE=.\DiscreteDistribution.h
# End Source File
# Begin Source File

SOURCE=.\DiscreteGen.cpp
# End Source File
# Begin Source File

SOURCE=.\DiscreteGen.h
# End Source File
# Begin Source File

SOURCE=.\DrawGen.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawGen.h
# End Source File
# Begin Source File

SOURCE=.\ErlangGen.cpp
# End Source File
# Begin Source File

SOURCE=.\ErlangGen.h
# End Source File
# Begin Source File

SOURCE=.\GammaGen.cpp
# End Source File
# Begin Source File

SOURCE=.\GammaGen.h
# End Source File
# Begin Source File

SOURCE=.\GeometricGen.cpp
# End Source File
# Begin Source File

SOURCE=.\GeometricGen.h
# End Source File
# Begin Source File

SOURCE=.\LogNormalGen.cpp
# End Source File
# Begin Source File

SOURCE=.\LogNormalGen.h
# End Source File
# Begin Source File

SOURCE=.\NegExpGen.cpp
# End Source File
# Begin Source File

SOURCE=.\NegExpGen.h
# End Source File
# Begin Source File

SOURCE=.\NormalGen.cpp
# End Source File
# Begin Source File

SOURCE=.\NormalGen.h
# End Source File
# Begin Source File

SOURCE=.\ParetoGen.cpp
# End Source File
# Begin Source File

SOURCE=.\ParetoGen.h
# End Source File
# Begin Source File

SOURCE=.\PoissonGen.cpp
# End Source File
# Begin Source File

SOURCE=.\PoissonGen.h
# End Source File
# Begin Source File

SOURCE=.\RandIntGen.cpp
# End Source File
# Begin Source File

SOURCE=.\RandIntGen.h
# End Source File
# Begin Source File

SOURCE=.\RandomGen.h
# End Source File
# Begin Source File

SOURCE=.\RayleighGen.cpp
# End Source File
# Begin Source File

SOURCE=.\RayleighGen.h
# End Source File
# Begin Source File

SOURCE=.\TruncatedCauchyGen.cpp
# End Source File
# Begin Source File

SOURCE=.\TruncatedCauchyGen.h
# End Source File
# Begin Source File

SOURCE=.\TruncatedGeometricGen.cpp
# End Source File
# Begin Source File

SOURCE=.\TruncatedGeometricGen.h
# End Source File
# Begin Source File

SOURCE=.\TruncatedLogNormalGen.cpp
# End Source File
# Begin Source File

SOURCE=.\TruncatedLogNormalGen.h
# End Source File
# Begin Source File

SOURCE=.\TruncatedParetoGen.cpp
# End Source File
# Begin Source File

SOURCE=.\TruncatedParetoGen.h
# End Source File
# Begin Source File

SOURCE=.\TruncatedWeibullGen.cpp
# End Source File
# Begin Source File

SOURCE=.\TruncatedWeibullGen.h
# End Source File
# Begin Source File

SOURCE=.\UniformGen.cpp
# End Source File
# Begin Source File

SOURCE=.\UniformGen.h
# End Source File
# Begin Source File

SOURCE=.\WeibullGen.cpp
# End Source File
# Begin Source File

SOURCE=.\WeibullGen.h
# End Source File
# End Target
# End Project
