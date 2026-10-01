
#include "std.h"
#include "glo.h"
#include "doc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* variable */

HWND g_hWndMain=NULL;
BOOL g_bShowTip=TRUE;
BOOL g_bShowKeyInTip=TRUE;

HIMAGELIST _himlToolbar=NULL;

TBBUTTON tbbtns[]=
{
	{ 0, ID_PROB_MANUAL,   TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, -1},
	{-1, 0,                TBSTATE_ENABLED, BTNS_SEP,       0, 0, -1},
	{ 1, ID_PROB_START,    TBSTATE_ENABLED, BTNS_BUTTON,    0, 0, -1},
	{ 2, ID_PROB_PAUSE,    TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, -1},
	{ 3, ID_PROB_STOP,     TBSTATE_ENABLED, BTNS_BUTTON,    0, 0, -1},
	{-1, 0,                TBSTATE_ENABLED, BTNS_SEP,       0, 0, -1},
	{ 4, ID_PROB_RING,     TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, -1},
	{ 5, ID_TOOL_OPT,      TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0, -1},
	{ 6, ID_HELP_ABOUT,    TBSTATE_ENABLED, BTNS_BUTTON,    0, 0, -1},
};


/* function */

LRESULT CALLBACK Frm_WndProc(HWND, UINT, WPARAM, LPARAM);

/* message map */
LRESULT Frm_Cmd(HWND, UINT, WPARAM, LPARAM);
LRESULT Frm_Create(HWND, UINT, WPARAM, LPARAM);
LRESULT Frm_EraseBkgnd(HWND, UINT, WPARAM, LPARAM);
LRESULT Frm_Paint(HWND, UINT, WPARAM, LPARAM);
LRESULT Frm_Size(HWND, UINT, WPARAM, LPARAM);
LRESULT Frm_SetFocus(HWND, UINT, WPARAM, LPARAM);
LRESULT Frm_MenuSelect(HWND, UINT, WPARAM, LPARAM);
LRESULT Frm_Notify(HWND, UINT, WPARAM, LPARAM);
LRESULT Frm_Close(HWND, UINT, WPARAM, LPARAM);
LRESULT Frm_Destroy(HWND, UINT, WPARAM, LPARAM);

MSGMAP mmMain[]=
{
	{WM_COMMAND,     Frm_Cmd},
	{WM_CREATE,      Frm_Create},
	{WM_ERASEBKGND,  Frm_EraseBkgnd},
	{WM_PAINT,       Frm_Paint},
	{WM_SIZE,        Frm_Size},
	{WM_SETFOCUS,    Frm_SetFocus},
	{WM_MENUSELECT,  Frm_MenuSelect},
	{WM_NOTIFY,      Frm_Notify},
	{WM_CLOSE,       Frm_Close},
	{WM_DESTROY,     Frm_Destroy}
};

MSGMAPINF mmiMain={ sizeof(mmMain)/sizeof(MSGMAP), mmMain, dwpWindow };

/* command map */
LRESULT Frm_Manual(HWND, WORD, WORD, HWND);
LRESULT Frm_Start(HWND, WORD, WORD, HWND);
LRESULT Frm_Pause(HWND, WORD, WORD, HWND);
LRESULT Frm_Stop(HWND, WORD, WORD, HWND);
LRESULT Frm_Exit(HWND, WORD, WORD, HWND);

CMDMAP cmMain[]=
{
	{ID_PROB_RING,   Frm_Ring},
	{ID_PROB_MANUAL, Frm_Manual},
	{ID_PROB_START,  Frm_Start},
	{ID_PROB_PAUSE,  Frm_Pause},
	{ID_PROB_STOP,   Frm_Stop},
	{ID_HELP_ABOUT,  Frm_About},
	{ID_TOOL_OPT,    Frm_Opt},
	{ID_FILE_EXIT,   Frm_Exit}
};

CMDMAPINF cmiMain={ sizeof(cmMain)/sizeof(CMDMAP), cmMain, dwpWindow };


/* implementation */

LRESULT Frm_Cmd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//if(View_nCmd(g_hWndView, wParam, lParam))
	//	return 0;

	return Dis_Cmd(&cmiMain, hWnd, wParam, lParam);
}

LRESULT Frm_Manual(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	if(!g_bManual)
		Doc_Stop();

	g_bManual=!g_bManual;
	Frm_ChangeMode();
	return 0;
}

LRESULT Frm_Start(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	Doc_Start();
	return 0;
}

LRESULT Frm_Pause(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	Doc_Pause();
	return 0;
}

LRESULT Frm_Stop(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	Doc_Stop();
	return 0;
}

LRESULT Frm_Exit(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	SendMessage(hWnd, WM_CLOSE, 0, 0);
	return 0;
}

LRESULT CALLBACK Frm_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Dis_Msg(&mmiMain, hWnd, uMsg, wParam, lParam);
}

LRESULT Frm_Create(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	g_hWndMain=hWnd;

	_himlToolbar=ImageList_LoadImage(g_hInst, MAKEINTRESOURCE(IDB_TOOLBAR), 16, 0, RGB(0, 255, 255), IMAGE_BITMAP, LR_CREATEDIBSECTION);
	if(!_himlToolbar)
		return -1;

	g_hWndToolbar=CreateToolbar(hWnd, IDC_TOOLBAR);
	if(!g_hWndToolbar)
		return -1;

	SendMessage(g_hWndToolbar, TB_SETIMAGELIST, 0, (LPARAM)_himlToolbar);
	SendMessage(g_hWndToolbar, TB_ADDBUTTONS, sizeof(tbbtns)/sizeof(tbbtns[0]), (LPARAM)&tbbtns);

	g_hWndRebar=CreateRebar(hWnd, IDC_REBAR);
	if(!g_hWndRebar)
		return -1;

	if(!Rebar_InsertBand(g_hWndRebar, -1, g_hWndToolbar))
		return -1;

	g_hWndStatus=CreateStatus(hWnd);
	if(!g_hWndStatus)
		return -1;

	g_hWndView=CreateView();
	if(!g_hWndView)
		return -1;

	return 0;
}

LRESULT Frm_EraseBkgnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 1;
}

LRESULT Frm_Paint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT Frm_Size(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rc;
	static int ctls[]={1, IDC_TOOLBAR, 1, IDC_REBAR, 1, IDC_STATUS, 0, 0};

	/*SendMessage(g_hWndToolbar, uMsg, wParam, lParam);*/
	SendMessage(g_hWndRebar, uMsg, wParam, lParam);

	SendMessage(g_hWndStatus, uMsg, wParam, lParam);
	Status_Init();

	GetEffectiveClientRect(hWnd, &rc, ctls);
	MoveWindow(g_hWndView, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, TRUE);

	return 0;
}

LRESULT Frm_SetFocus(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetFocus(g_hWndView);
	return 0;
}

LRESULT Frm_MenuSelect(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//static UINT popups[]={ID_MENU_FILE, ID_MENU_PROB, ID_MENU_TOOL, ID_MENU_HELP};

	UINT   uID;
	UINT   uCmd=GET_WM_MENUSELECT_CMD(wParam, lParam);
	HMENU  hMenu=GET_WM_MENUSELECT_HMENU(wParam, lParam);
	UINT   uFlags=GET_WM_MENUSELECT_FLAGS(wParam, lParam) & 0xffff;

	if(uFlags & MFT_SEPARATOR)             // Ignore separators
		uID=0;
	else if(uFlags==0xffff && hMenu==NULL) // Menu has been closed
		uID=0;
	else if(uFlags & MF_POPUP)             // Popup menu
		uID=0;
	//{
	//	if(uFlags & MF_SYSMENU)            // System menu
	//		uID=ID_MENU_SYS;
	//	else // Get string ID for popup menu from idPopup array 
	//		uID=((uCmd < sizeof(popups)/sizeof(popups[0])) ? popups[uCmd] : 0);
	//}
	else
		uID=uCmd;

	Status_SetResText(uID, STATUSPART_INFO, 0);
	return 0;
}

LRESULT Frm_Notify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT uID=(UINT)wParam;
	LPNMHDR pnmh=(LPNMHDR)lParam;

	switch(pnmh->code)
	{
	case TTN_GETDISPINFO:
		{
			LPTOOLTIPTEXT pttt=(LPTOOLTIPTEXT)lParam;
			Status_SetResText(uID, STATUSPART_INFO, 0);

			if(g_bShowTip)
			{
				static TCHAR pch[TOL_MAXSTR];
				Resource_LoadStr(App_GetResInst(), uID, pch, TOL_MAXSTR, 0);
				Resource_GetTooltip(pch, pttt->szText, 80, g_bShowKeyInTip);
				return TRUE;
			}

			return FALSE;
		}

	/*toolbar CCS_ADJUSTABLE*/
	/*case TBN_QUERYINSERT:
	case TBN_QUERYDELETE:
	case TBN_DELETINGBUTTON:
	case TBN_BEGINDRAG:
	case TBN_ENDDRAG:
	case TBN_BEGINADJUST:
	case TBN_INITCUSTOMIZE:
		return TRUE;*/

	/*toolbar TBSTYLE_CUSTOMERASE*/
	/*case NM_CUSTOMDRAW:
		Toolbar_Draw(hWnd, uMsg, wParam, lParam);
		break;*/
	}

	return 0;
}

LRESULT Frm_Close(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DestroyWindow(hWnd);
	return 0;
}

LRESULT Frm_Destroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WINDOWPLACEMENT wp;
	TCHAR pch[MAX_PATH];

	App_GetRegKey(pch, MAX_PATH, NULL);
	GetWindowPlacement(hWnd, &wp);

	Reg_SetValue(HKEY_CURRENT_USER, pch, NULL, _T("Show"), REG_BINARY, (LPBYTE)&wp.showCmd, sizeof(wp.showCmd));
	Reg_SetValue(HKEY_CURRENT_USER, pch, NULL, _T("Place"), REG_BINARY, (LPBYTE)&wp.rcNormalPosition, sizeof(wp.rcNormalPosition));
	Reg_SetValue(HKEY_CURRENT_USER, pch, NULL, _T("Tooltip"), REG_BINARY, (LPBYTE)&g_bShowTip, sizeof(g_bShowTip));
	Reg_SetValue(HKEY_CURRENT_USER, pch, NULL, _T("ShortKey"), REG_BINARY, (LPBYTE)&g_bShowKeyInTip, sizeof(g_bShowKeyInTip));

	ImageList_Destroy(_himlToolbar);
	PostQuitMessage(0);
	return 0;
}

void Frm_ChangeMode()
{
	HMENU hm=GetMenu(g_hWndMain);

	CheckMenuItem(hm, ID_PROB_MANUAL, MF_BYCOMMAND|(g_bManual?MF_CHECKED:MF_UNCHECKED));
	EnableMenuItem(hm, ID_PROB_START, MF_BYCOMMAND|(!g_bManual?MF_ENABLED:MF_GRAYED));
	EnableMenuItem(hm, ID_PROB_PAUSE, MF_BYCOMMAND|(!g_bManual?MF_ENABLED:MF_GRAYED));
	EnableMenuItem(hm, ID_PROB_STOP, MF_BYCOMMAND|(!g_bManual?MF_ENABLED:MF_GRAYED));

	SendMessage(g_hWndToolbar, TB_CHECKBUTTON, ID_PROB_MANUAL, MAKELONG(g_bManual, 0));
	SendMessage(g_hWndToolbar, TB_ENABLEBUTTON, ID_PROB_START, MAKELONG(!g_bManual, 0));
	SendMessage(g_hWndToolbar, TB_ENABLEBUTTON, ID_PROB_PAUSE, MAKELONG(!g_bManual, 0));
	SendMessage(g_hWndToolbar, TB_ENABLEBUTTON, ID_PROB_STOP, MAKELONG(!g_bManual, 0));

	View_ChangeMode();
}

void Frm_ChangePause(BOOL bPaused)
{
	HMENU hm=GetMenu(g_hWndMain);
	CheckMenuItem(hm, ID_PROB_PAUSE, MF_BYCOMMAND|(bPaused?MF_CHECKED:MF_UNCHECKED));
	SendMessage(g_hWndToolbar, TB_CHECKBUTTON, ID_PROB_PAUSE, MAKELONG(bPaused, 0));
}

HWND CreateMainWnd()
{
	LONG lRes;
	RECT rc;
	DWORD dw;
	int nShow;
	HWND hWnd;
	WNDCLASSEX wcex={0};
	TCHAR pch[MAX_PATH];
	TCHAR pchCaption[TOL_MAXSTR];
	LPCTSTR pcWndClass=_T("HanoiTowerFrameClass");

	wcex.cbSize=sizeof(WNDCLASSEX);
	wcex.style=CS_HREDRAW|CS_VREDRAW;
	wcex.lpfnWndProc=(WNDPROC)Frm_WndProc;
	wcex.hInstance=g_hInst;
	wcex.hIcon=(HICON)LoadImage(g_hInst, MAKEINTRESOURCE(IDR_MAIN), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE|LR_DEFAULTCOLOR|LR_SHARED);
	wcex.hCursor=/*LoadCursor(NULL, IDC_ARROW)*/(HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(32512/*OCR_NORMAL*/), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE|LR_SHARED);
	wcex.hbrBackground=NULL; /*(HBRUSH)(COLOR_APPWORKSPACE+1);*/
	wcex.lpszMenuName=MAKEINTRESOURCE(IDR_MAIN);
	wcex.lpszClassName=pcWndClass;
	wcex.hIconSm=(HICON)LoadImage(g_hInst, MAKEINTRESOURCE(IDR_MAIN), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);

	if(!RegisterClassEx(&wcex))
		return NULL;

	if(!LoadString(App_GetResInst(), IDR_MAIN, pchCaption, TOL_MAXSTR))
		lstrcpy(pchCaption, _T("Hanoi Tower"));

	App_GetRegKey(pch, MAX_PATH, NULL);

	dw=sizeof(g_bShowTip);
	Reg_GetValue(HKEY_CURRENT_USER, pch, _T("Tooltip"), NULL, (LPBYTE)&g_bShowTip, &dw);

	dw=sizeof(g_bShowKeyInTip);
	Reg_GetValue(HKEY_CURRENT_USER, pch, _T("ShortKey"), NULL, (LPBYTE)&g_bShowKeyInTip, &dw);

	dw=sizeof(nShow);
	lRes=Reg_GetValue(HKEY_CURRENT_USER, pch, _T("Show"), NULL, (LPBYTE)&nShow, &dw);

	if(lRes!=ERROR_SUCCESS || nShow==SW_SHOWMINIMIZED)
		nShow=g_nCmdShow;

	dw=sizeof(rc);
	lRes=Reg_GetValue(HKEY_CURRENT_USER, pch, _T("Place"), NULL, (LPBYTE)&rc, &dw);

	if(lRes==ERROR_SUCCESS)
	{
		rc.right-=rc.left;
		rc.bottom-=rc.top;
	}
	else
		rc.left=rc.top=rc.right=rc.bottom=CW_USEDEFAULT;

	hWnd=CreateWindowEx(0, pcWndClass, pchCaption, WS_OVERLAPPEDWINDOW,
		rc.left, rc.top, rc.right, rc.bottom, NULL, NULL, g_hInst, NULL);

	if(hWnd)
	{
		ShowWindow(hWnd, nShow);
		UpdateWindow(hWnd);
	}

	return hWnd;
}

#ifdef __cplusplus
} /* extern "C" */
#endif
