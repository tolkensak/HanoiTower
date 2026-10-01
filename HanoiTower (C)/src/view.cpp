
#include "std.h"
#include "glo.h"
#include "doc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* variable */

HWND g_hWndView=NULL;
BOOL g_bManual=FALSE;

int _nRingMoving=-1;
HCURSOR _hCurPoint=NULL;
HCURSOR _hCurHold=NULL;


/* function */

/* message map */
LRESULT View_Cmd(HWND, UINT, WPARAM, LPARAM);
LRESULT View_Create(HWND, UINT, WPARAM, LPARAM);
LRESULT View_EraseBkgnd(HWND, UINT, WPARAM, LPARAM);
LRESULT View_Paint(HWND, UINT, WPARAM, LPARAM);
LRESULT View_Size(HWND, UINT, WPARAM, LPARAM);
LRESULT View_MouseMove(HWND, UINT, WPARAM, LPARAM);
LRESULT View_LButtonDblClk(HWND, UINT, WPARAM, LPARAM);
LRESULT View_LButtonDown(HWND, UINT, WPARAM, LPARAM);
LRESULT View_LButtonUp(HWND, UINT, WPARAM, LPARAM);
LRESULT View_Mouse(HWND, UINT, WPARAM, LPARAM);
LRESULT View_Destroy(HWND, UINT, WPARAM, LPARAM);

MSGMAP mmView[]=
{
	{WM_CREATE,        View_Create},
	{WM_ERASEBKGND,    View_EraseBkgnd},
	{WM_PAINT,         View_Paint},
	{WM_SIZE,          View_Size},
	{WM_MOUSEMOVE,     View_MouseMove},
	{WM_LBUTTONDBLCLK, View_LButtonDblClk},
	{WM_LBUTTONDOWN,   View_LButtonDown},
	{WM_LBUTTONUP,     View_LButtonUp},
	{WM_RBUTTONDBLCLK, View_Mouse},
	{WM_RBUTTONDOWN,   View_Mouse},
	{WM_RBUTTONUP,     View_Mouse},
	{WM_MBUTTONDBLCLK, View_Mouse},
	{WM_MBUTTONDOWN,   View_Mouse},
	{WM_MBUTTONUP,     View_Mouse},
	{WM_XBUTTONDBLCLK, View_Mouse},
	{WM_XBUTTONDOWN,   View_Mouse},
	{WM_XBUTTONUP,     View_Mouse},
	{WM_DESTROY,       View_Destroy}
};

MSGMAPINF mmiView={ sizeof(mmView)/sizeof(MSGMAP), mmView, dwpWindow };


/* implementation */

LRESULT CALLBACK View_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Dis_Msg(&mmiView, hWnd, uMsg, wParam, lParam);
}

LRESULT View_Create(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int n;
	DWORD dw;
	COLORREF clr;
	HINSTANCE hInst=g_hInst;
	TCHAR pch[MAX_PATH];

	if(!Doc_Make())
		return -1;

	App_GetRegKey(pch, MAX_PATH, NULL);

	dw=sizeof(g_bManual);
	Reg_GetValue(HKEY_CURRENT_USER, pch, _T("Manual"), NULL, (LPBYTE)&g_bManual, &dw);

	dw=sizeof(n);
	if(Reg_GetValue(HKEY_CURRENT_USER, pch, _T("Rings"), NULL, (LPBYTE)&n, &dw)==ERROR_SUCCESS)
		Doc_SetRingNum(n);

	App_GetRegKey(pch, MAX_PATH, _T("Colors"));

	dw=sizeof(clr);
	if(Reg_GetValue(HKEY_CURRENT_USER, pch, _T("Ring"), NULL, (LPBYTE)&clr, &dw)==ERROR_SUCCESS)
		g_clrRing=clr;

	dw=sizeof(clr);
	if(Reg_GetValue(HKEY_CURRENT_USER, pch, _T("Tower"), NULL, (LPBYTE)&clr, &dw)==ERROR_SUCCESS)
		g_clrTower=clr;

	dw=sizeof(clr);
	if(Reg_GetValue(HKEY_CURRENT_USER, pch, _T("Background"), NULL, (LPBYTE)&clr, &dw)==ERROR_SUCCESS)
		g_clrBkgnd=clr;

	_hCurPoint=(HCURSOR)LoadImage(hInst, MAKEINTRESOURCE(IDU_POINT), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
	_hCurHold=(HCURSOR)LoadImage(hInst, MAKEINTRESOURCE(IDU_HOLD), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);

	Frm_ChangeMode();
	return 0;
}

LRESULT View_EraseBkgnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 1;
}

void TOLCALL View_Draw(HWND hWnd, HDC hDC, LPCRECT prc)
{
	Doc_Draw(hDC);
}

LRESULT View_Paint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC=BeginPaint(hWnd, &ps);
	Draw_SmoothPaint(hWnd, hDC, View_Draw, NULL);
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT View_Size(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rc={0, 0, LOWORD(lParam), HIWORD(lParam)};
	Doc_SetRect(&rc);
	return 0;
}

LRESULT View_MouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int x=GET_X_LPARAM(lParam); 
	int y=GET_Y_LPARAM(lParam);

	if(!g_bManual)
		return 0;

	if(_nRingMoving==-1)
	{
		SetCursor(_hCurPoint);
	}
	else
	{
		Doc_DragRing(_nRingMoving, x, y);
		SetCursor(_hCurHold);
		InvalidateRect(hWnd, NULL, FALSE);
	}

	return 0;
}

LRESULT View_LButtonDblClk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(g_bManual)
		SetCursor(_hCurPoint);

	SendMessage(g_hWndMain, WM_COMMAND, MAKELONG(ID_PROB_RING, 0), 0);

	return 0;
}

LRESULT View_LButtonDown(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int x=GET_X_LPARAM(lParam); 
	int y=GET_Y_LPARAM(lParam);

	if(!g_bManual)
		return 0;

	_nRingMoving=Doc_PtOnRing(x, y);

	if(_nRingMoving==-1)
		SetCursor(_hCurPoint);
	else
	{
		RECT rc;

		SetCursor(_hCurHold);

		GetClientRect(hWnd, &rc);
		MapWindowPoints(hWnd, NULL, (LPPOINT)&rc, 2);
		ClipCursor(&rc);
	}

	return 0;
}

LRESULT View_LButtonUp(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int x=GET_X_LPARAM(lParam); 
	int y=GET_Y_LPARAM(lParam);

	if(!g_bManual)
		return 0;

	SetCursor(_hCurPoint);

	if(_nRingMoving!=-1)
	{
		Doc_DropRing(Doc_NearTower(x, y), _nRingMoving);
		_nRingMoving=-1;
		ClipCursor(NULL);
		InvalidateRect(hWnd, NULL, FALSE);
	}

	return 0;
}

LRESULT View_Mouse(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(g_bManual)
		SetCursor(_hCurPoint);

	return 0;
}

LRESULT View_Destroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int n;
	TCHAR pch[MAX_PATH];

	App_GetRegKey(pch, MAX_PATH, NULL);
	n=Doc_GetRingNum();
	Reg_SetValue(HKEY_CURRENT_USER, pch, NULL, _T("Rings"), REG_BINARY, (LPBYTE)&n, sizeof(n));
	Reg_SetValue(HKEY_CURRENT_USER, pch, NULL, _T("Manual"), REG_BINARY, (LPBYTE)&g_bManual, sizeof(g_bManual));

	App_GetRegKey(pch, MAX_PATH, _T("Colors"));
	Reg_SetValue(HKEY_CURRENT_USER, pch, NULL, _T("Ring"), REG_BINARY, (LPBYTE)&g_clrRing, sizeof(g_clrRing));
	Reg_SetValue(HKEY_CURRENT_USER, pch, NULL, _T("Tower"), REG_BINARY, (LPBYTE)&g_clrTower, sizeof(g_clrTower));
	Reg_SetValue(HKEY_CURRENT_USER, pch, NULL, _T("Background"), REG_BINARY, (LPBYTE)&g_clrBkgnd, sizeof(g_clrBkgnd));

	if(_hCurPoint)
		DestroyCursor(_hCurPoint);

	if(_hCurHold)
		DestroyCursor(_hCurHold);

	Doc_Kill();
	return 0;
}

HWND CreateView()
{
	WNDCLASSEX wcex={0};
	LPCTSTR pcWndClass=_T("HanoiTowerViewClass");

	wcex.cbSize=sizeof(WNDCLASSEX);
	wcex.style=CS_DBLCLKS;
	wcex.lpfnWndProc=(WNDPROC)View_WndProc;
	wcex.hInstance=g_hInst;
	wcex.hCursor=/*LoadCursor(NULL, IDC_ARROW)*/(HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(32512/*OCR_NORMAL*/), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE|LR_SHARED);
	wcex.lpszClassName=pcWndClass;

	if(!RegisterClassEx(&wcex))
		return NULL;

	return CreateWindowEx(0, pcWndClass, NULL, WS_CHILD|WS_VISIBLE|WS_TABSTOP,
		0, 0, 0, 0, g_hWndMain, (HMENU)IDC_VIEW, g_hInst, NULL);
}

void View_ChangeMode()
{
}

#ifdef __cplusplus
} /* extern "C" */
#endif
