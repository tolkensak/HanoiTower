
#if !defined(AFX_XANOYESEBI_H__4B196FBE_3BD0_4F8E_8ECA_B2BD22EF53F5__INCLUDED_)
#define AFX_XANOYESEBI_H__4B196FBE_3BD0_4F8E_8ECA_B2BD22EF53F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTower Class

class CTower
{
private:
	int m_nPostion, m_nTop;

public:
	int Postion(){ return m_nPostion; };
	void Postion(int nPostion){ m_nPostion=nPostion; };
	int Top(){ return m_nTop; };
	void Top(int nTop){ m_nTop=nTop; };
};

/////////////////////////////////////////////////////////////////////////////

enum EState { State_End, State_Up, State_Horz, State_Down, State_Stop, State_Running };

/////////////////////////////////////////////////////////////////////////////
// CRing Class

class CRing  
{
public:
	CRing();
	void Create(CRect rcRing);
	int Tower(){ return m_nTower; };
	void Tower(int nTower){ m_nTower=nTower; };
	EState State(){ return m_eState; };
	void State(EState eState){ m_eState=eState; };
	COLORREF Color(){ return m_crColor; };
	CRect& Ring(){ return m_rcRing; };

private:
	int m_nTower;
	EState m_eState;
	CRect m_rcRing;
	COLORREF m_crColor;
	UINT Rand(UINT nCon);
};

/////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CHanoiTower thread

class CHanoiTower : public CWinThread
{
	DECLARE_DYNCREATE(CHanoiTower)

// Attributes
public:
	CHanoiTower();
	void Draw();
	void Create(CWnd* ptrzBoard, int nRing, int* penVelocity, enum EState* peState);

private:
	void Resolve(int nRing, int nOne, int nTwo, int nThree);
	BOOL Move(int nExit, int nEnter);

	int Tower(int nRet){ return m_aRing[nRet].Tower(); }
	void Tower(int nRet, int nTower){ m_aRing[nRet].Tower(nTower); }
	EState State(int nRet){ return m_aRing[nRet].State(); };
	void State(int nRet, EState eState){ m_aRing[nRet].State(eState); };
	COLORREF Color(int nRet){ return m_aRing[nRet].Color(); };
	CRect& Ring(int nRet){ return m_aRing[nRet].Ring(); };
	int Postion(int nTower){ return m_Tower[nTower].Postion(); }
	void Postion(int nTower, int nPostion){ m_Tower[nTower].Postion(nPostion); }
	int Top(int nTower){ return m_Tower[nTower].Top(); }
	void Top(int nTower, int nTop){ m_Tower[nTower].Top(nTop); }

private:
	int m_nTop, m_nBottom, m_nWide, m_nRow;
	CTower m_Tower[3];
	CArray <CRing, CRing> m_aRing;

	int* m_penVelocity;
	CWnd* m_ptrzBoard;
	EState* m_peState;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHanoiTower)
	public:
	virtual BOOL InitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHanoiTower)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XANOYESEBI_H__4B196FBE_3BD0_4F8E_8ECA_B2BD22EF53F5__INCLUDED_)
