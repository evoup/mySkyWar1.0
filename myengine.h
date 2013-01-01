#ifndef INCLUDE_MYENGINE_H
#define INCLUDE_MYENGINE_H
//
#define _RGB16BIT565(r,g,b) ((b%32) + ((g%64) << 6) + ((r%32) << 11))

#include <windows.h>
#include <ddraw.h>
#include <io.h>
#include "alpha.h"
#include "function.h"
#include <vector>
#include <iterator>
using namespace std;
extern DDBLTFX ddbltfx;
//##ModelId=4A1E88FB03A3
typedef struct SPRITE_TYP
{
	LPDIRECTDRAWSURFACE7 frames[100]; // 他/她有几个动画帧,先默认分别100个画面！不可能超过100个动作序列吧！
	float x,y; // 左上角坐标
	int velocity; // 速率
	int current_frame; // 切换到哪一帧
	int counter; // 计时
	int theframe_width;//本帧的宽度，默认是充满的宽度，由于动作导致精灵有时覆盖范围不一样，所以最大就是默认，小了就得查询这个，同意查询一次
	int theframe_height;//本帧的高度
} SPRITE_OBJ, *SPRITE_OBJ_PTR;

// container structure for bitmaps .BMP file
//##ModelId=4A1E88FB03B3
typedef struct BITMAP_FILE_TAG
{
	BITMAPFILEHEADER bitmapfileheader;  // this contains the bitmapfile header
	BITMAPINFOHEADER bitmapinfoheader;  // this is all the info including the palette
	PALETTEENTRY     palette[256];      // we will store the palette here
	UCHAR            *buffer;           // this is a pointer to the data
} BITMAP_FILE, *BITMAP_FILE_PTR;

//##ModelId=4A1E88FB03B7
typedef struct HAPPY_FACE_TYP//定义笑脸数据结构，主要是控制他的坐标
{
int x,y; // position of happy face
int xv, yv; // velocity of happy face
} HAPPY_FACE, *HAPPY_FACE_PTR;
#ifndef INCLUDE_MAPDATA_TYPE
#define INCLUDE_MAPDATA_TYPE
typedef struct MAPDATA_type
{
    int xMax, yMax;              // map size in tiles
    int xCamera, yCamera;        // camera location in pixels
    int xMaxCamera, yMaxCamera;  // max camera coordinates
    //int nNPCCount;               // number of NPCs
    //int nScriptCount;            // number of linked scripts
    //LPNPC lpnpc;                 // linked NPCs
    //LPSCRIPT lpscr;              // linked scripts
} MAPDATA, FAR* LPMAPDATA;
#endif
void mybltclip(HWND hwnd,USHORT * lpsurface,int mempitch);
void mybltclip2(LPDIRECTDRAWSURFACE7 lpddsprimary,
				HWND hwnd,
				int x, 
				int y,
				int width,
				int height,
				UCHAR * bitmap,
				UCHAR * lpsurface,
				int mempitch);

//BltAlphaMMX的操作封装，解决aplha不能用ddrawclipper导致越界出错的问题。
bool myBAMMX(LPDIRECTDRAWSURFACE7 work,LPDIRECTDRAWSURFACE7 back,int startx,int starty,
			 int alpha,SPRITE_OBJ * whichsp,LPDIRECTDRAWSURFACE7 sprite);


//##ModelId=4A1E9C300170
typedef struct bullet
{
  struct bullet* next; //指向下一个子弹【用STL就不用next了】
  int nStyle;          //子弹的种类
  float nSpeed;          //子弹的速度
  int nDirection;      //子弹的方向
  int nDisplayState;   //显示方式，如果击中目标，就转向爆炸状态，以便
                       //Runbullet函数显示爆炸效果，同上边的飞机处理方法一样
  float x;//子弹X坐标
  float y;//子弹Y坐标
  int width;//子弹宽
  int height;//子弹长
  int sfid;//画面id
  int damage;//威力
  int canhit;//可以被打掉(Todo:子弹也有HP)
}BULLET_OBJ,*BULLET_OBJ_PTR;

typedef enum bonus{
	nothing=0,
	power=1,
	bomb=2,
	money500=3,
	money1000=4,
	money2000=5
};

//动画帧数据，主要存放一帧的有效区域，用于修正碰撞检测区域的一些误差
//##ModelId=4A28D60B008C
typedef struct anidata{
int id;//序列id
int width;//当前序列的精灵有效区域宽度，以中心点计算
int height;//但前序列的精灵有效区域高度，以中心点计算
}ANIDATA_OBJ,*ANIDATA;

//主要存放飞机的画面指针，有效区域会针对每帧做改变
//##ModelId=4A1E9F570122
typedef struct planedisplay{
LPDIRECTDRAWSURFACE7 * player_surface;//画面指针
RECT validate_area;//有效区域
}PLANEDISPLAY_OBJ,*PLANEDISPLAY_OBJ_PTR;

//用来返回精确坐标的结构
//##ModelId=4B02B4E40196
typedef struct floatpoint{
float x;
float y;
}*struct_floatpoint;


//
#endif
