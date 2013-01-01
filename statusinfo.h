#ifndef INCLUDE_STATUSINFO
#define INCLUDE_STATUSINFO
//
#include <ddraw.h>
//#include <windows.h>
#include <mmsystem.h> 
extern LPDIRECTDRAWSURFACE7 lpddsback;
//extern LPDIRECTDRAWSURFACE7 lpddswork;

extern HFONT	status_info_hFont;
extern HWND globalhwnd;
int showHP(int,int);
int showEXP();
int showHITS(int);
//
#endif