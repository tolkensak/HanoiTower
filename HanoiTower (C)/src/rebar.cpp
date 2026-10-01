
#include "std.h"
#include "glo.h"

#ifdef __cplusplus
extern "C" {
#endif

/* variable */

HWND g_hWndRebar=NULL;


/* function */

HWND CreateRebar(HWND hWndParent, UINT uID)
{
	REBARINFO rbi={0};

	g_hWndRebar=CreateWindowEx(WS_EX_TOOLWINDOW, REBARCLASSNAME, NULL,
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|RBS_VARHEIGHT|CCS_NODIVIDER,
		0, 0, 0, 0, hWndParent, (HMENU)(UINT_PTR)uID, g_hInst, NULL);

	if(!g_hWndRebar)
		return NULL;

	/* Initialize and send the REBARINFO structure. */
	rbi.cbSize=sizeof(REBARINFO);
	if(!SendMessage(g_hWndRebar, RB_SETBARINFO, 0, (LPARAM)&rbi))
		return NULL;

	return g_hWndRebar;
}

BOOL Rebar_InsertBand(HWND hWnd, int nBand, HWND hWndBand)
{
	DWORD dwBtnSize;
	REBARBANDINFO rbBand;

	/* Initialize structure members that both bands will share */
	rbBand.cbSize=sizeof(REBARBANDINFO);
	rbBand.fMask=RBBIM_STYLE|RBBIM_CHILD|RBBIM_CHILDSIZE|RBBIM_SIZE;
	rbBand.fStyle=RBBS_CHILDEDGE|RBBS_GRIPPERALWAYS|RBBS_USECHEVRON;
		
	/* Get the height of the toolbar */
	dwBtnSize=(DWORD)SendMessage(hWndBand, TB_GETBUTTONSIZE, 0,0);

	/* Set values unique to the band with the toolbar */
	rbBand.hwndChild=hWndBand;
	rbBand.cxMinChild=0;
	rbBand.cyMinChild=HIWORD(dwBtnSize);
	rbBand.cx=250;

	/* Add the band that has the toolbar */
	return (BOOL)SendMessage(hWnd, RB_INSERTBAND, (WPARAM)nBand, (LPARAM)&rbBand);
}

#ifdef __cplusplus
} /* extern "C" */
#endif
