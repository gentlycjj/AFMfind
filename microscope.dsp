# Microsoft Developer Studio Project File - Name="microscope" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=microscope - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "microscope.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "microscope.mak" CFG="microscope - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "microscope - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "microscope - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "microscope - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib pci2003.lib PCI8620_32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Release/AFM_USB_1±¶ËÙ100-400.exe"

!ELSEIF  "$(CFG)" == "microscope - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib pci2003.lib PCI8620_32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/AFM_USB_1±¶ËÙ.exe" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "microscope - Win32 Release"
# Name "microscope - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\adinit.cpp
# End Source File
# Begin Source File

SOURCE=.\Atomforce.cpp
# End Source File
# Begin Source File

SOURCE=.\BCAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Dibapi.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Measure.cpp
# End Source File
# Begin Source File

SOURCE=.\microscope.cpp
# End Source File
# Begin Source File

SOURCE=.\microscope.rc
# End Source File
# Begin Source File

SOURCE=.\microscopeDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\microscopeView.cpp
# End Source File
# Begin Source File

SOURCE=.\Myfile.cpp
# End Source File
# Begin Source File

SOURCE=.\RectTrackerEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Roughness.cpp
# End Source File
# Begin Source File

SOURCE=.\Show3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Singlescanner.cpp
# End Source File
# Begin Source File

SOURCE=.\SizeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Atomforce.h
# End Source File
# Begin Source File

SOURCE=.\BCAdjust.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\Dibapi.h
# End Source File
# Begin Source File

SOURCE=.\easyusb_card_dll.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Measure.h
# End Source File
# Begin Source File

SOURCE=.\microscope.h
# End Source File
# Begin Source File

SOURCE=.\microscopeDoc.h
# End Source File
# Begin Source File

SOURCE=.\microscopeView.h
# End Source File
# Begin Source File

SOURCE=.\PCI2003.h
# End Source File
# Begin Source File

SOURCE=.\PCI8620.h
# End Source File
# Begin Source File

SOURCE=.\RectTrackerEx.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Roughness.h
# End Source File
# Begin Source File

SOURCE=.\Show3d.h
# End Source File
# Begin Source File

SOURCE=.\Singlescanner.h
# End Source File
# Begin Source File

SOURCE=.\SizeDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\microscope.ico
# End Source File
# Begin Source File

SOURCE=.\res\microscopeDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\easyusb_card_dll.dll
# End Source File
# Begin Source File

SOURCE=.\easyusb_card_dll.lib
# End Source File
# End Target
# End Project
