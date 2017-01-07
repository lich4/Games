# Microsoft Developer Studio Generated NMAKE File, Based on 俄罗斯方块.dsp
!IF $(CFG)" == "
CFG=俄罗斯方块 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to 俄罗斯方块 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "俄罗斯方块 - Win32 Release" && "$(CFG)" != "俄罗斯方块 - Win32 Debug"
!MESSAGE 指定的配置 "$(CFG)" 无效.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "俄罗斯方块.mak" CFG="俄罗斯方块 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "俄罗斯方块 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "俄罗斯方块 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF $(OS)" == "Windows_NT
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "俄罗斯方块 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# 开始自定义宏
OutDir=.\Release
# 结束自定义宏

ALL : "$(OUTDIR)\俄罗斯方块.exe"


CLEAN :
	-@erase "$(INTDIR)\Script1.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\俄罗斯方块.obj"
	-@erase "$(OUTDIR)\俄罗斯方块.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\俄罗斯方块.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\Script1.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\俄罗斯方块.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\俄罗斯方块.pdb" /machine:I386 /out:"$(OUTDIR)\俄罗斯方块.exe" 
LINK32_OBJS= \
	"$(INTDIR)\俄罗斯方块.obj" \
	"$(INTDIR)\Script1.res"

"$(OUTDIR)\俄罗斯方块.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "俄罗斯方块 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# 开始自定义宏
OutDir=.\Debug
# 结束自定义宏

ALL : "$(OUTDIR)\俄罗斯方块.exe"


CLEAN :
	-@erase "$(INTDIR)\Script1.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\俄罗斯方块.obj"
	-@erase "$(OUTDIR)\俄罗斯方块.exe"
	-@erase "$(OUTDIR)\俄罗斯方块.ilk"
	-@erase "$(OUTDIR)\俄罗斯方块.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\俄罗斯方块.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\Script1.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\俄罗斯方块.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\俄罗斯方块.pdb" /debug /machine:I386 /out:"$(OUTDIR)\俄罗斯方块.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\俄罗斯方块.obj" \
	"$(INTDIR)\Script1.res"

"$(OUTDIR)\俄罗斯方块.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("俄罗斯方块.dep")
!INCLUDE "俄罗斯方块.dep"
!ELSE 
!MESSAGE Warning: cannot find "俄罗斯方块.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "俄罗斯方块 - Win32 Release" || "$(CFG)" == "俄罗斯方块 - Win32 Debug"
SOURCE=.\Script1.rc

"$(INTDIR)\Script1.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=".\俄罗斯方块.cpp"

"$(INTDIR)\俄罗斯方块.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

