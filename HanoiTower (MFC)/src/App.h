
#if !defined(AFX_XANOYMUNARASI_H__D5F0BDDA_A089_4666_A95D_CC6BC41AC479__INCLUDED_)
#define AFX_XANOYMUNARASI_H__D5F0BDDA_A089_4666_A95D_CC6BC41AC479__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHanoiTower:
// See Xanoy Towersi.cpp for the implementation of this class
//

class CHanoiTower : public CWinApp
{
public:
	CHanoiTower();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHanoiTower)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHanoiTower)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XANOYMUNARASI_H__D5F0BDDA_A089_4666_A95D_CC6BC41AC479__INCLUDED_)
