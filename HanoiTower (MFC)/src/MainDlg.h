
#if !defined(AFX_XANOYMUNARASITAH_H__FBF337ED_1472_4EDF_A3FD_DF54423D120E__INCLUDED_)
#define AFX_XANOYMUNARASITAH_H__FBF337ED_1472_4EDF_A3FD_DF54423D120E__INCLUDED_

#include "Problem.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHanoiTowerDlg dialog

class CHanoiTowerDlg : public CDialog
{
// Construction
public:
	CHanoiTowerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CHanoiTowerDlg)
	enum { IDD = IDD_MAIN };
	int		m_nRing;
	int		m_nVelocity;
	CStatic	m_stBoard;
	CStatic	m_stLamp;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHanoiTowerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	EState m_eState;
	CBitmap m_bmLampOn, m_bmLampOff;
	CHanoiTower* m_pHanoiTower;

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CHanoiTowerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnBiycik();
	afx_msg void OnBnRing();
	afx_msg void OnStLamp();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XANOYMUNARASITAH_H__FBF337ED_1472_4EDF_A3FD_DF54423D120E__INCLUDED_)
