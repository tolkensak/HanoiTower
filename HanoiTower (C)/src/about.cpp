
#include "std.h"
#include "glo.h"

#define ICON_CX 48
#define ICON_CY 48

#ifdef __cplusplus
extern "C" {
#endif

/* variables */

HICON _hIconAbout=NULL;


/* functions */

LRESULT CALLBACK About_WndProc(HWND, UINT, WPARAM, LPARAM);

/* message map */
LRESULT About_Cmd(HWND, UINT, WPARAM, LPARAM);
LRESULT About_Init(HWND, UINT, WPARAM, LPARAM);
LRESULT About_DrawItem(HWND, UINT, WPARAM, LPARAM);

MSGMAP mmAbout[]=
{
	{WM_COMMAND,    About_Cmd},
	{WM_INITDIALOG, About_Init},
	{WM_DRAWITEM,   About_DrawItem}
};

MSGMAPINF mmiAbout={ sizeof(mmAbout)/sizeof(MSGMAP), mmAbout, dwpNone };

/* command map */
LRESULT About_End(HWND, WORD, WORD, HWND);

CMDMAP cmAbout[]=
{
	{IDOK,     About_End},
	{IDCANCEL, About_End}
};

CMDMAPINF cmiAbout={ sizeof(cmAbout)/sizeof(CMDMAP), cmAbout, dwpNone };


/* implementation */

LRESULT Frm_About(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	DialogBox(App_GetResInst(), MAKEINTRESOURCE(IDD_ABOUT), hWnd, (DLGPROC)About_WndProc);
	return 0;
}

LRESULT CALLBACK About_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Dis_Msg(&mmiAbout, hWnd, uMsg, wParam, lParam);
}

LRESULT About_Init(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	_hIconAbout=(HICON)LoadImage(g_hInst, MAKEINTRESOURCE(IDR_MAIN), IMAGE_ICON, ICON_CX, ICON_CY, 0);
	Wnd_MoveToCenter(hWnd, g_hWndMain);
	return TRUE;
}

LRESULT About_DrawItem(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*UINT uID=(UINT)wParam;*/
	LPDRAWITEMSTRUCT pdis=(LPDRAWITEMSTRUCT)lParam;

	if(_hIconAbout && pdis->CtlType==ODT_STATIC)
		DrawIconEx(pdis->hDC, 0, 0, _hIconAbout, ICON_CX, ICON_CY, 0, NULL, DI_NORMAL);

	return TRUE;
}

LRESULT About_Cmd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Dis_Cmd(&cmiAbout, hWnd, wParam, lParam);
}

LRESULT About_End(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	if(_hIconAbout)
		DestroyIcon(_hIconAbout);

	EndDialog(hWnd, wCmd);
	return TRUE;
}

#ifdef __cplusplus
} /* extern "C" */
#endif
