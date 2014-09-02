; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMy2048Dlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "2048.h"

ClassCount=3
Class1=CMy2048App
Class2=CMy2048Dlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDR_MAINFRAME
Resource2=IDR_MAINMENU
Resource3=IDD_MY2048_DIALOG

[CLS:CMy2048App]
Type=0
HeaderFile=2048.h
ImplementationFile=2048.cpp
Filter=N

[CLS:CMy2048Dlg]
Type=0
HeaderFile=2048Dlg.h
ImplementationFile=2048Dlg.cpp
Filter=D
LastObject=ID_SIZE66
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=2048Dlg.h
ImplementationFile=2048Dlg.cpp
Filter=D

[DLG:IDD_MY2048_DIALOG]
Type=1
Class=CMy2048Dlg
ControlCount=0

[MNU:IDR_MAINMENU]
Type=1
Class=?
Command1=ID_SIZE44
Command2=ID_SIZE55
Command3=ID_SIZE66
CommandCount=3

