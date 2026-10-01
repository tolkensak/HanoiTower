
#if _MSC_VER>1000
#pragma once
#endif

#ifndef _INC_GLO_H
#define _INC_GLO_H

#include "res.h"

/* defines */
#define STATUSPART_INFO 0

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------
application
----------------------------*/

extern int g_nCmdShow;
extern HINSTANCE g_hInst;

//LPCTSTR App_GetCmdLine();
HINSTANCE App_GetResInst();
int App_GetRegKey(LPTSTR, int, LPCTSTR); /* len include terminated null */

/*----------------------------
main frame
----------------------------*/

extern HWND g_hWndMain;
extern BOOL g_bShowTip;
extern BOOL g_bShowKeyInTip;

HWND CreateMainWnd();
void Frm_ChangeMode();
void Frm_ChangePause(BOOL bPaused);

LRESULT Frm_About(HWND, WORD, WORD, HWND);
LRESULT Frm_Ring(HWND, WORD, WORD, HWND);
LRESULT Frm_Opt(HWND, WORD, WORD, HWND);

/*----------------------------
rebar
----------------------------*/

extern HWND g_hWndRebar;

HWND CreateRebar(HWND, UINT);
BOOL Rebar_InsertBand(HWND, int, HWND);

/*----------------------------
toolbar
----------------------------*/

extern HWND g_hWndToolbar;

HWND CreateToolbar(HWND, UINT);

/*----------------------------
statusbar
----------------------------*/

extern HWND g_hWndStatus;

HWND CreateStatus();
void Status_Init();
void Status_SetResText(UINT, WORD, WORD);
void Status_SetText(LPCTSTR, WORD, WORD);

/*----------------------------
view
----------------------------*/

extern HWND g_hWndView;
extern BOOL g_bManual;

HWND CreateView();
void View_ChangeMode();


/*-------------------------------------------------------------------------
Message and command dispatch infrastructure. The following type
definitions and functions are used by the message and command dispatching
mechanism and do not need to be changed.
-------------------------------------------------------------------------*/

/* For NON-MDI applications, uncomment line 1 below and comment
line 2.  For MDI applications, uncomment line 2 below, comment
line 1, and then define hwndMDIClient as a global variable. */
#define hwndMDIClient NULL        /* (1) Stub for NON-MDI applications. */
/*extern HWND hwndMDIClient;*/     /* (2) For MDI applications.          */

/* Function pointer prototype for message handling functions. */
typedef LRESULT (*PFNMSG)(HWND, UINT, WPARAM, LPARAM);

/* Function pointer prototype for command handling functions. */
typedef LRESULT (*PFNCMD)(HWND, WORD, WORD, HWND);

/* Enumerated type used to determine which default window procedure
should be called by the message- and command-dispatching mechanism
if a message or command is not handled explicitly. */
typedef enum _DefWndProc // Enumeration for Default Window Procedures.
{
	dwpNone,     // Do not call any default procedure.
	dwpWindow,   // Call DefWindowProc.
	dwpDialog,   // Call DefDlgProc (This should be used only for custom dialog boxes - standard dialog box use edwpNone).
	dwpMDIChild, // Call DefMDIChildProc.
	dwpMDIFrame  // Call DefFrameProc.
} DWP;

/* This structure maps messages to message handling functions. */
typedef struct _MsgMap // Message Dispatch structure.
{
	UINT uMsg;
	PFNMSG pfn;
} MSGMAP, *PMSGMAP;

/* This structure contains all of the information that a window
procedure passes to DispMessage in order to define the message
dispatching behavior for the window. */
typedef struct _MsgMapInf // Message Dipatch Information.
{
	int num;     // Number of message dispatch structs.
	PMSGMAP pmm; // Table of message dispatch structures.
	DWP dwp;     // Type of default window handler needed.
} MSGMAPINF, *PMSGMAPINF;

/* This structure maps command IDs to command handling functions. */
typedef struct _CmdMap // Command Dispatch structure.
{
	WORD wCmd;
	PFNCMD pfn;
} CMDMAP, *PCMDMAP;

/* This structure contains all of the information that a command
message procedure passes to DispCommand in order to define the
command dispatching behavior for the window. */
typedef struct _CmdMapInf // Command Dispatch Information.
{
	int num;     // Number of command dispatch structs.
	PCMDMAP pcm; // Table of command dispatch structures.
	DWP dwp;    // Type of default window handler needed.
} CMDMAPINF, *PCMDMAPINF;

/* Message and command dispatching functions.  They look up messages
and commands in the dispatch tables and call the appropriate handler function. */
LRESULT Dis_Msg(LPMSDI, HWND, UINT, WPARAM, LPARAM);
LRESULT Dis_Cmd(LPCMDI, HWND, WPARAM, LPARAM);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _INC_GLO_H */
