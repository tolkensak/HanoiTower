
#include "std.h"
#include "glo.h"

LRESULT Dis_Def(DWP, HWND, UINT, WPARAM, LPARAM);

LRESULT Dis_Msg(PMSGMAPINF pmmi, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int i, n=pmmi->num;
	PMSGMAP pmm=pmmi->pmm;

	for(i=0; i<n; i++)
	{
		if(pmm[i].uMsg==uMsg)
			return pmm[i].pfn(hWnd, uMsg, wParam, lParam);
	}

	return Dis_Def(pmmi->dwp, hWnd, uMsg, wParam, lParam);
}

LRESULT Dis_Cmd(PCMDMAPINF pcmi, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int i, n=pcmi->num;
	PCMDMAP pcm=pcmi->pcm;
	WORD wCmd=GET_WM_COMMAND_ID(wParam, lParam);

	/* Message packing of wparam and lparam have changed for Win32,
	so use the GET_WM_COMMAND macro to unpack the commnad. */

	for(i=0; i<n; i++)
	{
		if(pcm[i].wCmd==wCmd)
			return pcm[i].pfn(hWnd, wCmd, GET_WM_COMMAND_CMD(wParam, lParam), GET_WM_COMMAND_HWND(wParam, lParam));
	}

	return Dis_Def(pcmi->dwp, hWnd, WM_COMMAND, wParam, lParam);
}

LRESULT Dis_Def(DWP dwp, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(dwp)
	{
	case dwpNone: return 0;
	case dwpWindow: return DefWindowProc(hWnd, uMsg, wParam, lParam);
	case dwpDialog: return DefDlgProc(hWnd, uMsg, wParam, lParam);
	case dwpMDIFrame: return DefFrameProc(hWnd, hwndMDIClient, uMsg, wParam, lParam);
	case dwpMDIChild: return DefMDIChildProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}
