#ifndef INC_JPEGLIB
#define INC_JPEGLIB
//////////////////////////////////////////
#include "ijl.h"
#include <ddraw.h>
#pragma comment (lib,"ijl15.lib")
extern HWND globalhwnd;
//Display Mode
#define CLRMODE        16  //bit size
//#define SCREENWIDTH    640 //horizontal screeen pixel count
//#define SCREENHEIGHT   480 //vertical screen pixel count

//16Bit Color conversion macros
#define RGB555(r, g, b) ((r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10))
#define RGB565(r, g, b) ((r >> 3) | ((g >> 2) << 5) | ((b >> 3) << 11))

//16Bit Color Formats
#define PF_555  1
#define PF_565  2

//Color modes
#define C16BIT  16
#define C24BIT  24
#define C32BIT  32

//Where to get JPEG
#define FROM_FILE     0
#define FROM_RESOURCE 1
//16Bit format for video card
const int  PixelFormat = PF_565;
//HRESULT   Result;      //Results from function calls
//LPDIRECTDRAW7		   DDObject       = NULL; // DirectDraw object
//LPDIRECTDRAWSURFACE7	PrimarySurface = NULL; // DirectDraw primary surface
//LPDIRECTDRAWSURFACE7	BackBuffer     = NULL; // DirectDraw back surface
//LPDIRECTDRAWSURFACE7 JPGGraphic     = NULL; // JPG surface

typedef struct JPEGINFO_type{
int width;
int height;
}JPEGINFO,FAR *LP_JPEGINFO;
#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif	/* __cplusplus */
extern HRESULT              DDCopyBitmapx(IDirectDrawSurface7 *pdds, HBITMAP hbm, int x, int y, int dx, int dy);
#ifdef __cplusplus
}
#endif	/* __cplusplus */

bool LoadJPEG(int Location, LPCTSTR Name,  LPDIRECTDRAWSURFACE7* Surface,LPDIRECTDRAW7 dd,JPEGINFO &);
//////////////////////////////////////////
#endif