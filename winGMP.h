#ifndef INCLUDE_WINGMP_H
#define INCLUDE_WINGMP_H
//
#define DDRAW_INIT_STRUCT(ddstruct) {memset(&ddstruct,0,sizeof(ddstruct));ddstruct.dwSize=sizeof(ddstruct);}
// this builds a 16 bit color value in 5.6.5 format (green dominate mode)
#include <windows.h>
#include <ddraw.h>
#include "myengine.h"
#include <math.h>
extern LPDIRECTDRAW7 lpdd;

// pi defines
#define PI         ((float)3.141592654f)
// used to compute the min and max of two expresions
#define MIN(a, b)  (((a) < (b)) ? (a) : (b)) 
#define MAX(a, b)  (((a) > (b)) ? (b) : (a)) 

// storage for our lookup tables
extern float cos_look[361]; // 1 extra so we can store 0-360 inclusive
extern float sin_look[361]; // 1 extra so we can store 0-360 inclusive

#if 0
//d2d framework的碰撞检测定义
/* collision */
#define CL_RIGHT 1
#define CL_LEFT  2
#define CL_DOWN  4
#define CL_UP    8
//d2d framework的一种结构
typedef struct {
    float x, y, w, h;
} Rect;
#endif
LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(LPDIRECTDRAWSURFACE7 lpdds,
                                         int num_rects,
                                         LPRECT clip_list);
int Load_Bitmap_File(HWND hwnd,BITMAP_FILE_PTR bitmap, char *filename);
int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height);
int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap);
LPDIRECTDRAWSURFACE7 Create_Offscreen_Surface(int w,int h,int memflag);
LPDIRECTDRAWSURFACE7 DDraw_Create_Surface(int width, int height, int mem_flags, int color_key);
int DDraw_Draw_Surface(LPDIRECTDRAWSURFACE7 source, // source surface to draw
                      int x, int y,                 // position to draw at
                      int width, int height,        // size of source surface
                      LPDIRECTDRAWSURFACE7 dest,    // surface to draw the surface on
                      int transparent);         // transparency flag
int Scan_Image_Bitmap(BITMAP_FILE_PTR bitmap,     // bitmap file to scan image data from
                      LPDIRECTDRAWSURFACE7 lpdds, // surface to hold data
                      int cx, int cy);            // cell to scan image from

#if 0
//矩形碰撞检测，来自d2d framework
int rectcl (Rect *dest, Rect *src, float oldx, float oldy);
#endif

int Collision_Test(int x1, int y1, int w1, int h1, 
                   int x2, int y2, int w2, int h2); 

//一个三角函数预载函数，这样就可以使用三角函数查找表【索引】来代替cos(索引),sin(索引)得出实际结果
void Build_Sin_Cos_Tables(void);


int Fast_Distance_2D(int x, int y);


//
#endif