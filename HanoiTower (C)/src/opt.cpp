
#include "std.h"
#include "glo.h"
#include "doc.h"

#define NUM_COLOR  3
#define COLOR_RING  0
#define COLOR_TOWER 1
#define COLOR_BKGND 2

#ifdef __cplusplus
extern "C" {
#endif

/* variable */

BOOL _bChanged=TRUE;
COLORREF _colors[NUM_COLOR];


/* function */

LRESULT CALLBACK Opt_WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL Opt_Verify(HWND);
void Opt_Change(HWND, BOOL);
void Opt_ChooseColor(HWND, COLORREF*);

/* message map */
LRESULT Opt_Cmd(HWND, UINT, WPARAM, LPARAM);
LRESULT Opt_Init(HWND, UINT, WPARAM, LPARAM);
LRESULT Opt_DrawItem(HWND, UINT, WPARAM, LPARAM);

MSGMAP mmOpt[]=
{
	{WM_COMMAND,    Opt_Cmd},
	{WM_INITDIALOG, Opt_Init},
	{WM_DRAWITEM,   Opt_DrawItem}
};

MSGMAPINF mmiOpt={ sizeof(mmOpt)/sizeof(MSGMAP), mmOpt, dwpNone };

/* command map */
LRESULT Opt_OK(HWND, WORD, WORD, HWND);
LRESULT Opt_Cancel(HWND, WORD, WORD, HWND);
LRESULT Opt_Apply(HWND, WORD, WORD, HWND);
LRESULT Opt_Color(HWND, WORD, WORD, HWND);
LRESULT Opt_Tip(HWND, WORD, WORD, HWND);
LRESULT Opt_Key(HWND, WORD, WORD, HWND);

CMDMAP cmOpt[]=
{
	{IDOK,             Opt_OK},
	{IDCANCEL,         Opt_Cancel},
	{IDC_BTN_APPLY,    Opt_Apply},
	{IDC_BTN_RING,     Opt_Color},
	{IDC_BTN_TOWER,    Opt_Color},
	{IDC_BTN_BKGND,    Opt_Color},
	{IDC_CHK_TIP,  Opt_Tip},
	{IDC_CHK_KEY, Opt_Key}
};

CMDMAPINF cmiOpt={ sizeof(cmOpt)/sizeof(CMDMAP), cmOpt, dwpNone };


/* implementation */

LRESULT Frm_Opt(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	DialogBox(App_GetResInst(), MAKEINTRESOURCE(IDD_OPT), hWnd, (DLGPROC)Opt_WndProc);
	return 0;
}

LRESULT CALLBACK Opt_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Dis_Msg(&mmiOpt, hWnd, uMsg, wParam, lParam);
}

LRESULT Opt_Init(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	_colors[COLOR_RING]=g_clrRing;
	_colors[COLOR_TOWER]=g_clrTower;
	_colors[COLOR_BKGND]=g_clrBkgnd;

	SendDlgItemMessage(hWnd, IDC_CHK_TIP, BM_SETCHECK, g_bShowTip?BST_CHECKED:BST_UNCHECKED, 0);
	SendDlgItemMessage(hWnd, IDC_CHK_KEY, BM_SETCHECK, g_bShowKeyInTip?BST_CHECKED:BST_UNCHECKED, 0);
	EnableWindow(GetDlgItem(hWnd, IDC_CHK_KEY), g_bShowTip);

	_bChanged=TRUE;
	Opt_Change(hWnd, FALSE);
	Wnd_MoveToCenter(hWnd, g_hWndMain);
	return TRUE;
}

LRESULT Opt_DrawItem(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UINT uID=(UINT)wParam;
	LPDRAWITEMSTRUCT pdis=(LPDRAWITEMSTRUCT)lParam;

	if(pdis->CtlType==ODT_STATIC)
	{
		HBRUSH hbr=NULL;

		switch(uID)
		{
		case IDC_STC_RING:
		case IDC_STC_TOWER:
		case IDC_STC_BKGND:
			hbr=CreateSolidBrush(_colors[uID-IDC_STC_RING]);
			break;
		}

		if(hbr)
		{
			FillRect(pdis->hDC, &pdis->rcItem, hbr);
			DeleteObject(hbr);
		}
	}

	return TRUE;
}

LRESULT Opt_Cmd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Dis_Cmd(&cmiOpt, hWnd, wParam, lParam);
}

LRESULT Opt_OK(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	if(Opt_Verify(hWnd))
		EndDialog(hWnd, wCmd);

	return TRUE;
}

LRESULT Opt_Cancel(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	EndDialog(hWnd, wCmd);
	return TRUE;
}

LRESULT Opt_Apply(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	Opt_Verify(hWnd);
	return TRUE;
}

LRESULT Opt_Color(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	Opt_ChooseColor(hWnd, _colors+wCmd-IDC_BTN_RING);
	return TRUE;
}

LRESULT Opt_Tip(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	EnableWindow(GetDlgItem(hWnd, IDC_CHK_KEY), SendDlgItemMessage(hWnd, IDC_CHK_TIP, BM_GETCHECK, 0, 0)==BST_CHECKED);
	Opt_Change(hWnd, TRUE);
	return TRUE;
}

LRESULT Opt_Key(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	Opt_Change(hWnd, TRUE);
	return TRUE;
}

BOOL Opt_Verify(HWND hWnd)
{
	if(_bChanged)
	{
		g_clrRing=_colors[COLOR_RING];
		g_clrTower=_colors[COLOR_TOWER];
		g_clrBkgnd=_colors[COLOR_BKGND];

		g_bShowTip=SendDlgItemMessage(hWnd, IDC_CHK_TIP, BM_GETCHECK, 0, 0)==BST_CHECKED;
		g_bShowKeyInTip=SendDlgItemMessage(hWnd, IDC_CHK_KEY, BM_GETCHECK, 0, 0)==BST_CHECKED;

		Opt_Change(hWnd, FALSE);
		InvalidateRect(g_hWndMain, NULL, FALSE);
	}

	return TRUE;
}

void Opt_ChooseColor(HWND hWnd, COLORREF *pColor)
{
	static COLORREF custs[16];

	CHOOSECOLOR cc={0};

	cc.lStructSize=sizeof(cc);
	cc.Flags=CC_ANYCOLOR|CC_FULLOPEN|CC_RGBINIT;
	cc.hwndOwner=hWnd;
	cc.rgbResult=*pColor;
	cc.lpCustColors=custs;

	if(ChooseColor(&cc) && cc.rgbResult!=*pColor)
	{
		*pColor=cc.rgbResult;
		Opt_Change(hWnd, TRUE);
		InvalidateRect(hWnd, NULL, FALSE);
	}
}

void Opt_Change(HWND hWnd, BOOL bNew)
{
	if(bNew!=_bChanged)
	{
		_bChanged=bNew;
		EnableWindow(GetDlgItem(hWnd, IDC_BTN_APPLY), _bChanged);
	}
}

#ifdef __cplusplus
} /* extern "C" */
#endif
