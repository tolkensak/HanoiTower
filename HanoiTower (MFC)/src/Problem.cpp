
#include "stdafx.h"
#include "App.h"
#include "Problem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRing

CRing::CRing()
{
}

void CRing::Create(CRect rcRing)
{
	m_rcRing=rcRing;
	m_crColor=RGB(Rand(255), Rand(255), Rand(255));
	m_eState=State_Up;
	m_nTower=0;
}

UINT CRing::Rand(UINT nCon)
{
	int nRand=rand();
	float fNormal=(float)nCon/RAND_MAX;
	float fRet=(float)nRand*fNormal+0.5f;
	return (UINT)fRet;
}

/////////////////////////////////////////////////////////////////////////////
// CHanoiTower

IMPLEMENT_DYNCREATE(CHanoiTower, CWinThread)

CHanoiTower::CHanoiTower()
{
	m_peState=NULL;
	m_penVelocity=NULL;
	m_ptrzBoard=NULL;
}

BOOL CHanoiTower::InitInstance()
{
	return TRUE;
}

int CHanoiTower::Run() 
{
	if(*m_peState==State_End)
		AfxEndThread(0, TRUE);
	Resolve(m_aRing.GetSize(), 0, 1, 2);
	Sleep(1000);
	*m_peState=State_Horz;
	m_ptrzBoard->Invalidate();
	return 0;
}

BEGIN_MESSAGE_MAP(CHanoiTower, CWinThread)
	//{{AFX_MSG_MAP(CHanoiTower)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CHanoiTower::Create(CWnd* ptrzBoard, int nRing, int* penVelocity, enum EState* peState)
{
	m_ptrzBoard=ptrzBoard;
	m_penVelocity=penVelocity;
	m_peState=peState;

	CRect rcBoard;
	ptrzBoard->GetClientRect(rcBoard);

	int nWidth,
		nDistance=rcBoard.Width()/6,
		nUnit=nDistance/nRing;

	m_nWide=rcBoard.Height()/(nRing+3);
	m_nBottom=rcBoard.Height()-m_nWide;
	m_nTop=m_nBottom-nRing*m_nWide;
	m_nRow=m_nTop-m_nWide/2;
	m_aRing.RemoveAll();
	m_aRing.SetSize(nRing);
	for(int i=0; i<nRing; i++)
	{
		nWidth=nUnit*(nRing-i);
		m_aRing[i].Create(CRect(CPoint(nDistance-nWidth, m_nBottom-m_nWide*(i+1)), CSize(2*nWidth, m_nWide)));
	}
	
	for(i=0; i<3; i++)
	{
		Postion(i, (2*i+1)*nDistance);
		Top(i, m_nBottom);
	}
	Top(0, Ring(nRing-1).top);
	Draw();
}

void CHanoiTower::Draw()
{
	m_ptrzBoard->RedrawWindow();
	CClientDC dc(m_ptrzBoard);
	CPen penWhite;
	penWhite.CreatePen(PS_SOLID, 1, RGB(255,255,255));
	CPen* penOriginal;
	penOriginal=dc.SelectObject(&penWhite);
	for(int i=0; i<3; i++)
	{
		dc.MoveTo(Postion(i), m_nTop);
		dc.LineTo(Postion(i), m_nBottom);
	}
	
	for(i=0; i<m_aRing.GetSize(); i++)
		dc.FillSolidRect(Ring(i), Color(i));
	dc.SelectObject(penOriginal);
}

void CHanoiTower::Resolve(int nRing, int nOne, int nTwo, int nThree)
{
	if(nRing==1)
	{
		while(!Move(nOne, nThree))
		{
			Sleep(*m_penVelocity);
			if(*m_peState==State_End)
				AfxEndThread(0, TRUE);
		}
	}
	else
	{
		Resolve(nRing-1,nOne,nThree,nTwo);
		while(!Move(nOne, nThree))
		{
			Sleep(*m_penVelocity);
			if(*m_peState==State_End)
				AfxEndThread(0, TRUE);
		}
		Resolve(nRing-1,nTwo,nOne,nThree);
	}
}

BOOL CHanoiTower::Move(int nExit, int nEnter)
{
	CClientDC dc(m_ptrzBoard);
	for(int nRing=m_aRing.GetSize()-1; nRing>=0; nRing--)
		if(Tower(nRing)==nExit)	break;

	switch(State(nRing))
	{
		case State_Up:
		{
			CPoint ptLeft, ptRight;
			CPen penBoard, penRing, *penOriginal;
			penBoard.CreatePen(PS_SOLID, 1, (COLORREF)GetSysColor(COLOR_BTNFACE));
			penRing.CreatePen(PS_SOLID, 1, Color(nRing));

			penOriginal=dc.SelectObject(&penBoard);
			ptLeft=Ring(nRing).TopLeft();
			ptRight=Ring(nRing).BottomRight();
			ptLeft.y=ptRight.y=ptRight.y-1;
			dc.MoveTo(ptLeft);
			dc.LineTo(ptRight);

			Ring(nRing).OffsetRect(0, -1);
			dc.SelectObject(&penRing);

			ptLeft=Ring(nRing).TopLeft();
			ptRight=Ring(nRing).BottomRight();
			ptRight.y=ptLeft.y;
			dc.MoveTo(ptLeft);
			dc.LineTo(ptRight);

			int nLimit=Ring(nRing).bottom;
			if(nLimit>=m_nTop)
				dc.SetPixel(Postion(nExit), nLimit, RGB(255, 255, 255));
			if(Ring(nRing).bottom<=m_nRow)
				State(nRing, State_Horz);
			dc.SelectObject(penOriginal);
			return FALSE;
		}
		case State_Horz:
		{
			CPoint ptTop, ptBottom;
			CPen penBoard, penRing, *penOriginal;
			penBoard.CreatePen(PS_SOLID, 1, (COLORREF)GetSysColor(COLOR_BTNFACE));
			penRing.CreatePen(PS_SOLID, 1, Color(nRing));
			penOriginal=dc.SelectObject(&penBoard);
			if(nExit<nEnter)
			{
				ptTop=Ring(nRing).TopLeft();
				ptBottom=Ring(nRing).BottomRight();
				ptBottom.x=ptTop.x;
				dc.MoveTo(ptTop);
				dc.LineTo(ptBottom);

				Ring(nRing).OffsetRect(1, 0);
				dc.SelectObject(&penRing);
			
				ptTop=Ring(nRing).TopLeft();
				ptBottom=Ring(nRing).BottomRight();
				ptTop.x=ptBottom.x=ptBottom.x-1;
				dc.MoveTo(ptTop);
				dc.LineTo(ptBottom);

				if(Ring(nRing).CenterPoint().x>=Postion(nEnter))
					State(nRing, State_Down);
			}
			else
			{
				ptTop=Ring(nRing).TopLeft();
				ptBottom=Ring(nRing).BottomRight();
				ptTop.x=ptBottom.x=ptBottom.x-1;
				dc.MoveTo(ptTop);
				dc.LineTo(ptBottom);

				Ring(nRing).OffsetRect(-1, 0);
				dc.SelectObject(&penRing);
				
				ptTop=Ring(nRing).TopLeft();
				ptBottom=Ring(nRing).BottomRight();
				ptBottom.x=ptTop.x;
				dc.MoveTo(ptTop);
				dc.LineTo(ptBottom);

				if(Ring(nRing).CenterPoint().x<=Postion(nEnter))
					State(nRing, State_Down);
			}
			dc.SelectObject(penOriginal);
			return FALSE;
		}
		case State_Down:
		{
			CPoint ptLeft, ptRight;
			CPen penBoard, penRing, *penOriginal;
			penBoard.CreatePen(PS_SOLID, 1, (COLORREF)GetSysColor(COLOR_BTNFACE));
			penRing.CreatePen(PS_SOLID, 1, Color(nRing));
			penOriginal=dc.SelectObject(&penBoard);
			ptLeft=Ring(nRing).TopLeft();
			ptRight=Ring(nRing).BottomRight();
			ptRight.y=ptLeft.y;
			dc.MoveTo(ptLeft);
			dc.LineTo(ptRight);

			Ring(nRing).OffsetRect(0, 1);
			dc.SelectObject(&penRing);
			
			ptLeft=Ring(nRing).TopLeft();
			ptRight=Ring(nRing).BottomRight();
			ptLeft.y=ptRight.y=ptRight.y-1;
			dc.MoveTo(ptLeft);
			dc.LineTo(ptRight);

			int nLimit=Ring(nRing).top-1;
			if(nLimit>=m_nTop)
				dc.SetPixel(Postion(nEnter), nLimit, RGB(255, 255, 255));
			if(Ring(nRing).bottom>=Top(nEnter))
				State(nRing, State_End);
			dc.SelectObject(penOriginal);
			return FALSE;
		}
	}	
	Top(nExit, Top(nExit)+m_nWide);
	Top(nEnter, Top(nEnter)-m_nWide);
	Tower(nRing, nEnter);
	State(nRing, State_Up);
	return TRUE;
}
