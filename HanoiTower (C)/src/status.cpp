
#include "std.h"
#include "glo.h"

#ifdef __cplusplus
extern "C" {
#endif

/* variable */

HWND g_hWndStatus=NULL;


/* function */

HWND CreateStatus()
{
	g_hWndStatus=CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD|WS_VISIBLE|SBARS_SIZEGRIP,
		0, 0, 0, 0, g_hWndMain, (HMENU)IDC_STATUS, g_hInst, NULL);

	return g_hWndStatus;
}

void Status_Init()
{
	//const cSpaceInBetween = 8;
	//int   ar[3]; /* Array defining the number of parts/sections the Status bar will display */
	//SIZE  sz;
	//RECT  rc;
	//HDC   hDC;

	//hDC=GetDC(g_hWndMain);
	//GetClientRect(g_hWndMain, &rc);

	//ar[2] = rc.right;

	//if(GetTextExtentPoint(hDC, _T("00:00:00 PM"), 12, &sz))
	//	ar[1]=ar[2]-sz.cx-cSpaceInBetween;
	//else
	//	ar[1]=0;

	//if(GetTextExtentPoint(hDC, _T("Time:"), 5, &sz))
	//	ar[0]=ar[1]-sz.cx-cSpaceInBetween;
	//else
	//	ar[0]=0;

	//ReleaseDC(g_hWndMain, hDC);
	//SendMessage(g_hWndStatus, SB_SETPARTS, sizeof(ar)/sizeof(ar[0]), (LPARAM)ar);

	//Status_SetText(NULL, 0, 0);
	//Status_SetText(_T("Time:"), 1, SBT_POPOUT);
}

void Status_SetResText(UINT uID, WORD wPart, WORD wFlags)
{
	static TCHAR pch[TOL_MAXSTR];
	static TCHAR pch2[TOL_MAXSTR];

	Resource_LoadStr(App_GetResInst(), uID, pch, TOL_MAXSTR, 0);
	Resource_GetInfo(pch, pch2, TOL_MAXSTR);

	SendMessage(g_hWndStatus, SB_SETTEXT, wPart|wFlags, (LPARAM)pch2);
}

void Status_SetText(LPCTSTR pcText, WORD wPart, WORD wFlags)
{
	if(!pcText)
		pcText=_T("");

	SendMessage(g_hWndStatus, SB_SETTEXT, wPart|wFlags, (LPARAM)pcText);
}

#ifdef __cplusplus
} /* extern "C" */
#endif
