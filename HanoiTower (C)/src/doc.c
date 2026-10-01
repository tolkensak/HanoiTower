
#include "std.h"
#include "glo.h"
#include "doc.h"

#define NUM_TOWER 3 

#ifdef __cplusplus
extern "C" {
#endif

/* variables */

typedef struct _Ring
{
	int nTower;
	int x, y, cx, cy;
} RING, *PRING;

typedef struct _Tower
{
	int x;
	int nRingNum;
	PINT pnRing;
} TOWER, *PTOWER;


COLORREF g_clrBkgnd=0;
COLORREF g_clrRing=RGB(128, 128, 128);
COLORREF g_clrTower=RGB(255, 255, 255);

int _nRingNum=0;
PRING _rings=NULL;
TOWER _towers[NUM_TOWER];

BOOL _bPaused=FALSE;
HANDLE _hThread=NULL;

RECT _rect;
int _x, _y;
double _dx, _dy;


/* functions */

void Ring_GetRect(PRING pRing, LPRECT prc)
{
	prc->left=pRing->x-pRing->cx/2;
	prc->right=prc->left+pRing->cx;

	prc->bottom=pRing->y;
	prc->top=prc->bottom-pRing->cy;
}

void Ring_Draw(HDC hDC, BOOL bDraw)
{
	int i;
	RECT rc;

	double x;
	double bottom=_rect.bottom-_dy;
	double top=bottom-_nRingNum*_dy;

	HPEN hpen, hpenOld;
	HBRUSH hbr, hbrOld;

	/* Background */
	hbr=CreateSolidBrush(g_clrBkgnd);
	FillRect(hDC, &_rect, hbr);
	DeleteObject(hbr);


	/* Tower */
	hpen=CreatePen(PS_SOLID, 1, g_clrTower);
	hpenOld=SelectObject(hDC, hpen);

	for(x=_dx, i=0; i<NUM_TOWER; i++, x+=_dx)
	{
		MoveToEx(hDC, (int)x, (int)bottom, NULL);
		LineTo(hDC, (int)x, (int)top);
	}

	SelectObject(hDC, hpenOld);
	DeleteObject(hpen);


	/* Ring */
	hbr=CreateSolidBrush(g_clrRing);
	hbrOld=SelectObject(hDC, hbr);

	for(i=0; i<_nRingNum; i++)
	{
		Ring_GetRect(_rings+i, &rc);
		RoundRect(hDC, rc.left, rc.top, rc.right, rc.bottom, 20, 20);
	}

	SelectObject(hDC, hbrOld);
	DeleteObject(hbr);
}

BOOL Doc_Make()
{
	return Doc_SetRingNum(3);
}

void Doc_Kill()
{
	int i;
	HANDLE heap=GetProcessHeap();

	Doc_Stop();

	if(_rings)
		HeapFree(heap, 0, _rings);

	_rings=NULL;
	_nRingNum=0;

	for(i=0; i<NUM_TOWER; i++)
	{
		if(_towers[i].pnRing)
			HeapFree(heap, 0, _towers[i].pnRing);

		_towers[i].pnRing=NULL;
		_towers[i].nRingNum=0;
	}
}

int Doc_GetRingNum(){ return _nRingNum; }

BOOL Doc_SetRingNum(int nRingNum)
{
	int i;
	PRING rings=NULL;
	PINT pn[NUM_TOWER];
	HANDLE heap=GetProcessHeap();

	if(nRingNum==_nRingNum)
		return TRUE;

	if(nRingNum<=0 || nRingNum>MAX_RING_NUM)
		return FALSE;

	rings=HeapAlloc(heap, HEAP_ZERO_MEMORY, nRingNum*sizeof(RING));
	if(!rings)
		return FALSE;

	for(i=0; i<NUM_TOWER; i++)
	{
		pn[i]=HeapAlloc(heap, 0, nRingNum*sizeof(int));
		if(!pn[i])
			break;
	}

	if(i<NUM_TOWER)
	{
		HeapFree(heap, 0, rings);

		for(i=0; i<NUM_TOWER; i++)
			if(pn[i])
				HeapFree(heap, 0, pn[i]);

		return FALSE;
	}

	Doc_Kill();

	_rings=rings;
	_nRingNum=nRingNum;

	for(i=0; i<NUM_TOWER; i++)
		_towers[i].pnRing=pn[i];

	for(i=0; i<nRingNum; i++)
		_towers[0].pnRing[i]=i;

	_towers[0].nRingNum=nRingNum;

	Doc_SetRect(NULL);

	return TRUE;
}

void Doc_SetRect(LPCRECT prc)
{
	int i, j, r;
	double x, y;

	if(prc)
		CopyRect(&_rect, prc);
	else
		prc=&_rect;

	x=prc->right-prc->left;
	y=prc->bottom-prc->top;
	_dx=x/(NUM_TOWER+1);
	_dy=y/(_nRingNum+3);

	for(x=_dx, i=0; i<NUM_TOWER; i++, x+=_dx)
		_towers[i].x=(int)x;

	x=_dx/_nRingNum;
	for(i=0; i<NUM_TOWER; i++)
	{
		y=prc->bottom;
		for(y-=_dy, j=0; j<_towers[i].nRingNum; j++, y-=_dy)
		{
			r=_towers[i].pnRing[j];
			_rings[r].x=(int)((i+1)*_dx);
			_rings[r].cx=(int)(_dx-x*r);
			_rings[r].y=(int)y;
			_rings[r].cy=(int)_dy;
		}
	}
}

void Doc_Draw(HDC hDC)
{
	int i;
	RECT rc;

	double x;
	double bottom=_rect.bottom-_dy;
	double top=bottom-_nRingNum*_dy;

	HPEN hpen, hpenOld;
	HBRUSH hbr, hbrOld;

	/* Background */
	hbr=CreateSolidBrush(g_clrBkgnd);
	FillRect(hDC, &_rect, hbr);
	DeleteObject(hbr);


	/* Tower */
	hpen=CreatePen(PS_SOLID, 10, g_clrTower);
	hpenOld=SelectObject(hDC, hpen);

	for(x=_dx, i=0; i<NUM_TOWER; i++, x+=_dx)
	{
		MoveToEx(hDC, (int)x, (int)bottom, NULL);
		LineTo(hDC, (int)x, (int)top);
	}

	SelectObject(hDC, hpenOld);
	DeleteObject(hpen);


	/* Ring */
	hbr=CreateSolidBrush(g_clrRing);
	hbrOld=SelectObject(hDC, hbr);

	for(i=0; i<_nRingNum; i++)
	{
		Ring_GetRect(_rings+i, &rc);
		RoundRect(hDC, rc.left, rc.top, rc.right, rc.bottom, 30, 30);
	}

	SelectObject(hDC, hbrOld);
	DeleteObject(hbr);
}

int Doc_PtOnRing(int x, int y)
{
	RECT rc;
	int i, j;
	POINT pt={x, y};

	for(i=0; i<NUM_TOWER; i++)
	{
		j=_towers[i].nRingNum;

		if(j)
		{
			j=_towers[i].pnRing[j-1];
			Ring_GetRect(_rings+j, &rc);

			if(PtInRect(&rc, pt))
			{
				_x=_rings[j].x-x;
				_y=_rings[j].y-y;

				return j;
			}
		}
	}

	return -1;
}

int Doc_NearTower(int x, int y)
{
	int i, s, t=0, d=_rect.right-_rect.left;

	for(i=0; i<NUM_TOWER; i++)
	{
		s=x-_towers[i].x;

		if(s<0)
			s=-s;

		if(s<d)
		{
			d=s;
			t=i;
		}
	}

	return t;
}

void Doc_DragRing(int nRing, int x, int y)
{
    _rings[nRing].x=x+_x;
	_rings[nRing].y=y+_y;
}

void Doc_DropRing(int nTower, int nRing)
{
	int i, j, t;

	t=_rings[nRing].nTower;
	i=--_towers[t].nRingNum;
	j=_towers[nTower].nRingNum++;
	_towers[nTower].pnRing[j]=_towers[t].pnRing[i];

	_rings[nRing].nTower=nTower;
	_rings[nRing].x=_towers[nTower].x;
	_rings[nRing].y=(int)(_rect.bottom-(j+1)*_dy);
}

void Doc_Solve(int nRing, int nTower1, int nTower2, int nTower3)
{
	if(nRing>0)
	{
		Doc_Solve(nRing-1, nTower1, nTower3, nTower2);
		Doc_DropRing(nTower3, _towers[nTower1].pnRing[_towers[nTower1].nRingNum-1]);
		InvalidateRect(g_hWndView, NULL, FALSE);
		Sleep(300);
		Doc_Solve(nRing-1, nTower2, nTower1, nTower3);
	}
	else if(nRing==0)
	{
		Doc_DropRing(nTower3, _towers[nTower1].pnRing[_towers[nTower1].nRingNum-1]);
		InvalidateRect(g_hWndView, NULL, FALSE);
		Sleep(300);
	}
}

DWORD WINAPI Doc_Thread(LPVOID lpParam)
{
	Doc_Solve(_nRingNum-1, 0, 1, 2);

	CloseHandle(_hThread);
	_hThread=NULL;

	if(_bPaused)
	{
		_bPaused=FALSE;
		Frm_ChangePause(_bPaused);
	}

	return 0;
}

void Doc_Start()
{
	int i;

	Doc_Stop();

	for(i=1; i<NUM_TOWER; i++)
		_towers[i].nRingNum=0;

	_towers[0].nRingNum=_nRingNum;
	for(i=0; i<_nRingNum; i++)
	{
		_towers[0].pnRing[i]=i;
		_rings[i].nTower=0;
	}

	Doc_SetRect(NULL);
	InvalidateRect(g_hWndView, NULL, FALSE);

	_hThread=CreateThread(NULL, 0, Doc_Thread, NULL, 0, NULL);
}

void Doc_Pause()
{
	if(!_hThread)
		return;

	if(_bPaused)
		ResumeThread(_hThread);
	else
		SuspendThread(_hThread);

	_bPaused=!_bPaused;
	Frm_ChangePause(_bPaused);
}

void Doc_Stop()
{
	if(!_hThread)
		return;

	TerminateThread(_hThread, 0);
	CloseHandle(_hThread);
	_hThread=NULL;

	if(_bPaused)
	{
		_bPaused=FALSE;
		Frm_ChangePause(_bPaused);
	}
}

//DWORD WINAPI MoveThrdProc(LPVOID lpParam)
//{
//	return 0;
//}
//
//void SolveProb(int nRing, int nTower1, int nTower2, int nTower3)
//{
//	HANDLE hThrd;
//	POINT pt={nTower1, nTower3};
//
//	if(nRing==1)
//	{
//		hThrd=CreateThread(NULL, 0, MoveThrdProc, (LPVOID)&pt, 0, NULL);
//		WaitForSingleObject(hThrd, INFINITE);
//		CloseHandle(hThrd);
//	}
//	else
//	{
//		SolveProb(nRing-1, nTower1, nTower3, nTower2);
//
//		hThrd=CreateThread(NULL, 0, MoveThrdProc, (LPVOID)&pt, 0, NULL);
//		WaitForSingleObject(hThrd, INFINITE);
//		CloseHandle(hThrd);
//
//		SolveProb(nRing-1, nTower2, nTower1, nTower3);
//	}
//}

#ifdef __cplusplus
} /* extern "C" */
#endif
