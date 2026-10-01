
#include "std.h"
#include "glo.h"

#ifdef __cplusplus
extern "C" {
#endif

/* variables */

HINSTANCE g_hInst=NULL;

int g_nCmdShow=SW_SHOWDEFAULT;
LPCTSTR g_lpCmdLine=NULL;

HINSTANCE _hInstRes=NULL;
TCHAR _pchRegKey[MAX_PATH]={0};


/* functions */

//LPCTSTR App_GetCmdLine(){ return g_lpCmdLine; }
HINSTANCE App_GetResInst(){ return _hInstRes?_hInstRes:g_hInst; }

int App_GetRegKey(LPTSTR pcDst, int len, LPCTSTR pcSub) /* len include terminated null */
{
	int n=lstrlen(_pchRegKey);

	if(!n)
		n=App_RootRegKey(g_hInst, _pchRegKey, MAX_PATH);

	if(pcSub)
		n+=lstrlen(pcSub);

	if(!pcDst || len<n)
		return -n;

	if(pcSub)
		wsprintf(pcDst, _T("%s\\%s"), _pchRegKey, pcSub);
	else
		lstrcpy(pcDst, _pchRegKey);

	return n;
}

int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HACCEL hAcl;
	INITCOMMONCONTROLSEX icex;

	/* Ensure that the common control DLL is loaded */
	icex.dwSize=sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC=ICC_COOL_CLASSES|ICC_BAR_CLASSES;

	if(!InitCommonControlsEx(&icex))
		return 0;

	g_hInst=hInst;
	g_nCmdShow=nCmdShow;
	g_lpCmdLine=lpCmdLine;

	if(!CreateMainWnd())
		return 0;

	hAcl=LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_MAIN));

	while(GetMessage(&msg, NULL, 0, 0))
		if(!TranslateAccelerator(msg.hwnd, hAcl, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	return (int)msg.wParam;
}

#ifdef __cplusplus
} /* extern "C" */
#endif
