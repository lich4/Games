// 2048.h : main header file for the 2048 application
//

#if !defined(AFX_2048_H__86A3BEFF_2A15_422B_8D3B_44AD4395B040__INCLUDED_)
#define AFX_2048_H__86A3BEFF_2A15_422B_8D3B_44AD4395B040__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMy2048App:
// See 2048.cpp for the implementation of this class
//

class CMy2048App : public CWinApp
{
public:
	CMy2048App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy2048App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMy2048App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_2048_H__86A3BEFF_2A15_422B_8D3B_44AD4395B040__INCLUDED_)
