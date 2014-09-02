// 2048Dlg.h : header file
//

#if !defined(AFX_2048DLG_H__F6624F70_0820_4A56_88E0_464E73DAB39B__INCLUDED_)
#define AFX_2048DLG_H__F6624F70_0820_4A56_88E0_464E73DAB39B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
#include <time.h>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CMy2048Dlg dialog

class CMy2048Dlg : public CDialog
{
// Construction
public:
	CMy2048Dlg(CWnd* pParent = NULL);	// standard constructor

	int SIZE;
	int* Data;
	int scroll;
	void InitGame();
	BOOL GetLeftCellsToRandomPlace();
	BOOL Combine(UINT direction);
	BOOL Judge();
	int Get(int x,int y);
	void Set(int x,int y,int val);
	void ShowInfo(int goal);

	time_t begin;
	UINT mtimer;
// Dialog Data
	//{{AFX_DATA(CMy2048Dlg)
	enum { IDD = IDD_MY2048_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy2048Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMy2048Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize44();
	afx_msg void OnSize55();
	afx_msg void OnSize66();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_2048DLG_H__F6624F70_0820_4A56_88E0_464E73DAB39B__INCLUDED_)
