#ifndef INCLUDE_ALPHA_H
#define INCLUDE_ALPHA_H
//
//alphaMMX routine 
#define RGBMODE_555		0
#define RGBMODE_565		1 //
#define RGBMODE_16      2
#define RGBMODE_24		3
#define RGBMODE_32		4
#define RGBMODE_NONE	5
#include <windows.h>
#include <ddraw.h>

bool AlphaBlend16(LPDIRECTDRAWSURFACE7 lpDDSDest,RECT RectDest,LPDIRECTDRAWSURFACE7 lpDDSSrc,RECT RectSrc,int alpha);

int BltAlphaFast( LPDIRECTDRAWSURFACE7 lpDDSDest, LPDIRECTDRAWSURFACE7 lpDDSSource, 
			 int iDestX, int iDestY, LPRECT lprcSource, DWORD dwMode );
int BltAlphaFastMMX( LPDIRECTDRAWSURFACE7 lpDDSDest, LPDIRECTDRAWSURFACE7 lpDDSSource, 
			 int iDestX, int iDestY, LPRECT lprcSource, DWORD dwMode );
int BltAlphaMMX( LPDIRECTDRAWSURFACE7 lpDDSDest, LPDIRECTDRAWSURFACE7 lpDDSSource, 
			 int iDestX, int iDestY, LPRECT lprcSource, int iAlpha, DWORD dwMode );

DWORD GetRGBMode( LPDIRECTDRAWSURFACE7 lpDDS );
//
#endif
