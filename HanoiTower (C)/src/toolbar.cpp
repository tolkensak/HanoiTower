
#include "std.h"
#include "glo.h"

#ifdef __cplusplus
extern "C" {
#endif

/* variable */

HWND g_hWndToolbar=NULL;
WNDPROC _pfnDefToolbarProc=NULL;


/* functions */

LRESULT CALLBACK Toolbar_WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT Toolbar_MouseMove(HWND, UINT, WPARAM, LPARAM);
LRESULT Toolbar_MouseLeave(HWND, UINT, WPARAM, LPARAM);
/*LRESULT Toolbar_Notify(HWND, UINT, WPARAM, LPARAM);*/
/*LRESULT Toolbar_Draw(HWND, UINT, WPARAM, LPARAM);*/


/* implementation */

HWND CreateToolbar(HWND hWndParent, UINT uID)
{
	g_hWndToolbar=CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
		WS_CHILD|WS_VISIBLE|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|CCS_TOP,
		0, 0, 0, 0, hWndParent, (HMENU)(UINT_PTR)uID, g_hInst, NULL);

	if(g_hWndToolbar)
		_pfnDefToolbarProc=(WNDPROC)(LONG_PTR)SetWindowLongPtr(g_hWndToolbar, GWLP_WNDPROC, (LONG)(LONG_PTR)Toolbar_WndProc);

	return g_hWndToolbar;
}

LRESULT CALLBACK Toolbar_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		Toolbar_MouseMove(hWnd, uMsg, wParam, lParam);
		break;

	case WM_MOUSELEAVE:
		Toolbar_MouseLeave(hWnd, uMsg, wParam, lParam);
		break;

	/*case WM_NOTIFY:
		return Toolbar_Notify(hWnd, uMsg, wParam, lParam);*/

	case WM_DESTROY:
		SetWindowLongPtr(g_hWndToolbar, GWLP_WNDPROC, (LONG)(LONG_PTR)_pfnDefToolbarProc);
		break;
	}

	return CallWindowProc(_pfnDefToolbarProc, hWnd, uMsg, wParam, lParam);
}

LRESULT Toolbar_MouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt={GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

	if(SendMessage(hWnd, TB_HITTEST, 0, (LPARAM)&pt)<0)
		Status_SetText(NULL, STATUSPART_INFO, 0);

	return 0;
}

LRESULT Toolbar_MouseLeave(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Status_SetText(NULL, STATUSPART_INFO, 0);
	return 0;
}

/*LRESULT Toolbar_Notify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LPNMHDR pnmh=(LPNMHDR)lParam;

	if(pnmh->code==TTN_GETDISPINFO)
	{
		UINT uID=(UINT)wParam;
		LPTOOLTIPTEXT pttt=(LPTOOLTIPTEXT)lParam;
		Status_SetResText(uID, STATUSPART_INFO, 0);
		return g_bShowTip && ResTip_GetName(App_GetResInst(), uID, pttt->szText, 80, g_bShowKeyInTip);
	}

	return 0;
}*/

/*LRESULT Toolbar_Draw(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LPNMTBCUSTOMDRAW pnmtbcd=(LPNMTBCUSTOMDRAW)lParam;

	FillRect(pnmtbcd->nmcd.hdc, &pnmtbcd->nmcd.rc, GetSysColorBrush(COLOR_BTNFACE));
	DrawEdge(pnmtbcd->nmcd.hdc, &pnmtbcd->nmcd.rc, BDR_RAISEDINNER, BF_BOTTOM);

	return 0;
}*/

#ifdef __cplusplus
} /* extern "C" */
#endif
