; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDspView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "dsp.h"
LastPage=0

ClassCount=7
Class1=CDspApp
Class2=CDspDoc
Class3=CDspView
Class4=CMainFrame

ResourceCount=5
Resource1=IDR_MAINFRAME
Resource2=IDD_DLG_HISTOGRAM
Class5=CChildFrame
Class6=CAboutDlg
Resource3=IDD_ABOUTBOX
Resource4=IDR_DSPTYPE
Class7=CRadius
Resource5=IDD_DIALOG1

[CLS:CDspApp]
Type=0
HeaderFile=dsp.h
ImplementationFile=dsp.cpp
Filter=N

[CLS:CDspDoc]
Type=0
HeaderFile=dspDoc.h
ImplementationFile=dspDoc.cpp
Filter=N
LastObject=CDspDoc

[CLS:CDspView]
Type=0
HeaderFile=dspView.h
ImplementationFile=dspView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=ID_FFT


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M


[CLS:CAboutDlg]
Type=0
HeaderFile=dsp.cpp
ImplementationFile=dsp.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_DSPTYPE]
Type=1
Class=CDspView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_APP_ABOUT
Command22=ID_FANBAI
Command23=ID_REMOVERED
Command24=ID_MENU_Histogram
Command25=ID_HISTOGRAM_EQU
Command26=ID_SMOOTH
Command27=ID_LAPLACEENHANCED
Command28=ID_NEW_RECTANGLE
Command29=ID_NEW_CIRCLE
Command30=ID_FFT
CommandCount=30

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_DLG_HISTOGRAM]
Type=1
Class=?
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_HISTOGRAM,static,1342308352

[DLG:IDD_DIALOG1]
Type=1
Class=CRadius
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552

[CLS:CRadius]
Type=0
HeaderFile=Radius.h
ImplementationFile=Radius.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CRadius

