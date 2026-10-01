
#include "std.h"
#include "glo.h"
#include "doc.h"

#ifdef __cplusplus
extern "C" {
#endif

/* variable */


/* function */

LRESULT CALLBACK Ring_WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL Ring_Verify(HWND);

/* message map */
LRESULT Ring_Cmd(HWND, UINT, WPARAM, LPARAM);
LRESULT Ring_Init(HWND, UINT, WPARAM, LPARAM);

MSGMAP mmRing[]=
{
	{WM_COMMAND,    Ring_Cmd},
	{WM_INITDIALOG, Ring_Init}
};

MSGMAPINF mmiRing={ sizeof(mmRing)/sizeof(MSGMAP), mmRing, dwpNone };

/* command map */
LRESULT Ring_OK(HWND, WORD, WORD, HWND);
LRESULT Ring_Cancel(HWND, WORD, WORD, HWND);

CMDMAP cmRing[]=
{
	{IDOK,     Ring_OK},
	{IDCANCEL, Ring_Cancel}
};

CMDMAPINF cmiRing={ sizeof(cmRing)/sizeof(CMDMAP), cmRing, dwpNone };


/* implementation */

LRESULT Frm_Ring(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	DialogBox(App_GetResInst(), MAKEINTRESOURCE(IDD_RING), hWnd, (DLGPROC)Ring_WndProc);
	return 0;
}

LRESULT CALLBACK Ring_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Dis_Msg(&mmiRing, hWnd, uMsg, wParam, lParam);
}

LRESULT Ring_Init(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetDlgItemInt(hWnd, IDC_EDT_RING, Doc_GetRingNum(), FALSE);
	SendDlgItemMessage(hWnd, IDC_SPN_RING, UDM_SETRANGE, 0, (LPARAM)MAKELONG(MAX_RING_NUM, 1));
	Wnd_MoveToCenter(hWnd, g_hWndMain);
	return TRUE;
}

LRESULT Ring_Cmd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return Dis_Cmd(&cmiRing, hWnd, wParam, lParam);
}

LRESULT Ring_OK(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	if(Ring_Verify(hWnd))
		EndDialog(hWnd, wCmd);

	return TRUE;
}

LRESULT Ring_Cancel(HWND hWnd, WORD wCmd, WORD wNotify, HWND hWndCtl)
{
	EndDialog(hWnd, wCmd);
	return TRUE;
}

BOOL Ring_Verify(HWND hWnd)
{
	BOOL b;
	int n=GetDlgItemInt(hWnd, IDC_EDT_RING, &b, FALSE);

	if(!b || !Doc_SetRingNum(n))
		return FALSE;

	InvalidateRect(g_hWndMain, NULL, FALSE);
	return TRUE;
}

#ifdef __cplusplus
} /* extern "C" */
#endif
