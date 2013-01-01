#ifndef INCLUDE_FPS
#define INCLUDE_FPS
#include <ddraw.h>
//#include <windows.h>
#include <mmsystem.h> 
extern LPDIRECTDRAWSURFACE7 lpddsback;
//extern LPDIRECTDRAWSURFACE7 lpddswork;

extern HFONT	g_hFont;
extern HWND globalhwnd;
////////////////////////////////
/*
 * Description:		Displays the current frame rate. For the function to
 *					work properly, it must be called in every frame.
 *
 * Parameters:		None.
 *
 * Return value:	At the moment the function always returns 0.
 *
 */
int ShowFrameRate( int currentfps=0 );
///////////////
#endif