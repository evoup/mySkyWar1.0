#ifndef INCLUDE_CLASS_SPRITE_SURFACE_H
#define INCLUDE_CLASS_SPRITE_SURFACE_H
///
#pragma   warning(disable:4786)
#include <windows.h>
#include <ddraw.h>
#include "winGMP.h"
#include "function.h"
#include "myengine.h"
#include <list>
#include<string>
#include "jpeglib.h"
extern HWND globalhwnd;
extern LPDIRECTDRAW7 lpdd;
extern DDBLTFX ddbltfx;//ok?
extern LPDIRECTDRAWSURFACE7 gamemenu;
extern BITMAP_FILE bitmapsp;
using namespace std;
/**该结构作为精灵素材画面的选择句柄
*/

//##ModelId=4A1E88FB03E1
typedef struct SPRITE_SURFACE{
IDirectDrawSurface7 * lpddsrc;//精灵整个素材的画面
LPCSTR srcname;//素材的名字
int sprite_numframe;//精灵的帧数
SPRITE_OBJ sprite;//精灵结构体
LPCSTR spritename;//精灵的名字
int used;//该画面是否使用过

//SPRITE_SURFACE * next;//形成1个链表，之后可以写个lpdds名字查属性的函数,暂时不用链表
//，直接用下标1000数组
}SP_SF,*lp_sprite_surface;

//##ModelId=4A1E88FC0009
typedef struct SURFACE_MGR_STRUCT{//一个画面管理结构，用来保存SPRITE_SURFACE结构的对象
	LPCSTR sprite_group_name;//这批精灵的统一名字，如骷髅兵LV1
	LPCSTR sprite_surface_array_indexes;//精灵句柄索引，分隔符是'|'可供劈开
	LPCSTR source_pic_name;//素材文件名字
	IDirectDrawSurface7 * lpdds;//加载到了哪个素材画面
	int dds_width;//素材长
	int dds_height;//素材宽
}SF_MGR,*surface_mgr;

class cls_sprite_surface  
{
public:
	
	cls_sprite_surface();
	
	virtual ~cls_sprite_surface();
	
	bool test();
	
	bool alert(int word);
	
	bool alert(LPCSTR word);
public:
	
	int get_max_sprite_surface(int debug);/**获取当SPRITE_SURFACE结构对象数量**/
	
	IDirectDrawSurface7 * getSpriteSurface(IDirectDrawSurface7 * lpdds,char * srcname,char * spritename,\
		LPCSTR szBitmap,int frame_num,bool debugflag=0);
	IDirectDrawSurface7 * getSpriteSurfaceJPG(IDirectDrawSurface7 * lpdds,char * srcname,char * spritename,\
		LPCSTR szJpg,int frame_num,bool debugflag=0);
	
	SPRITE_SURFACE sprite_surface[1000];//结构体成员变量，可以匹配到素材画面和对应的精灵动作画面
	
	LPCSTR get_serial_surface(char * sp_gp_name);//根据精灵名字返回一系列精灵画面的句柄"1|2|3",之后
	//调用者再劈开。如参数是"骷髅兵"，会返回若干个“骷髅兵”精灵画面的数组下标
	//std::list <SPRITE_SURFACE *> sprite_surface_list;

	//让外部可以访问本成员的某些函数
	
	friend int friend_cls_sprite_surface_getmax(cls_sprite_surface *);//注：省形参的写法，标准写成这样(cls_sprite_surface *obj)
private:
	
	HWND hwnd;
	
	std::list<SF_MGR * > sprite_handler_list;
	
	SF_MGR  * sfmgrtemp;//用来临时填充sprite_handler_list用结构体
	
	std::list<SF_MGR * >::iterator iter;//迭代sprite_handler_list对象


};
///
#endif 
