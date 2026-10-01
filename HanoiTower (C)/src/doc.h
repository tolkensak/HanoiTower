
#if _MSC_VER>1000
#pragma once
#endif

#define MAX_RING_NUM 64

#ifndef _INC_DOC_H
#define _INC_DOC_H

#ifdef __cplusplus
extern "C" {
#endif

/* variables */

extern COLORREF g_clrBkgnd;
extern COLORREF g_clrRing;
extern COLORREF g_clrTower;


/* functions */

BOOL Doc_Make();
void Doc_Kill();

int Doc_GetRingNum();
BOOL Doc_SetRingNum(int nRings);
void Doc_SetRect(LPCRECT prc);

int Doc_PtOnRing(int x, int y);
int Doc_NearTower(int x, int y);
void Doc_DragRing(int nRing, int x, int y);
void Doc_DropRing(int nTower, int nRing);

void Doc_Start();
void Doc_Pause();
void Doc_Stop();

void Doc_Draw(HDC hDC);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _INC_DOC_H */
