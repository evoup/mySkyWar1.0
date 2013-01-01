// DEMO2_3.CPP - A complete windows program
//测试CVS
// INCLUDES ///////////////////////////////////////////////
#define GAME_NAME "毕业设计-C++飞行射击游戏DEMO"
#define WIN32_LEAN_AND_MEAN  // just say no to MFC

//#define USE_GAMEDEV_TILE_1//这是gamedev的tile in directx的TILE引擎(下面这个比较好)
#define USE_GAMEDEV_TILE_2//这是gamedev的Game Programming GenesisPart VIII : Basic Tile Engines 的TILE引擎
#define DDRAW_INIT_STRUCT(ddstruct) {memset(&ddstruct,0,sizeof(ddstruct));ddstruct.dwSize=sizeof(ddstruct);}
// this builds a 16 bit color value in 5.6.5 format (green dominate mode)
#include <windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#include <stdio.h>
#include <math.h>
#include <ddraw.h>
#include <io.h>//_lseek 要包含的
#include <iostream>//除非没有H，是无法用std的！
using namespace std;
#include "winGMP.h"//windows游戏编程大师上的一些低级操作函数（如解锁加锁表面）
#include "myengine.h"//DX和精灵的结构定义
#include "function.h"
#include "jpeglib.h"
#ifdef USE_GAMEDEV_TILE_1
#include "map.h"//地图数据变量和tile函数
#endif
#ifdef USE_GAMEDEV_TILE_2
#include "map2.h"
#endif

#include "fps.h"
#include "alpha.h"
#include "class_sprite_surface.h"
#include "class_player.h"
#include "class_enemy.h"
#include "class_sprite.h"
#include "class_ai.h"
#include "class_boss.h"
#include "statusinfo.h"
#include "class_tank.h"
#include "freshenemy.h"
#include <dsound.h> // include dsound, dmusic
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>

#include "T3DLIB3.H"
//LUA include 
extern "C"
{
#include  "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#pragma comment (lib,"lua51.lib")
#pragma comment (lib,"ddraw.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"winmm.lib")
#pragma comment (lib,"dsound.lib")
// DEFINES ////////////////////////////////////////////////
//##################################################
//设置一个调试标记，把Blt要不要Rect的问题搞清楚！
//7.6主表面的单色填充矩形（无FLIP）
//7.7是像素裁切（FLIP）//作者还没有
//7.8是软件裁切（把位图上的东西裁切到画面，无FLIP）
//7.9是DDCLIPPER(作者的例子没有FLIP，我的有)
//7.10是加载一张8位色位图
//是否推荐使用7.8?是个问题，应为他不是基于双缓冲的，是高级指针lpsurface和mempitch直接操作
//####################
//1是7.6的例子
//2是7.7的例子
//3是同时7.6和7.7的例子
//4是7.6、7.7、7.8、7.9的例子
//5是7.10的例子，作者没有用FLIP,是锁住了主画面操作的ddsd.lpSurface，让他等于bitmap的buffer，然后unlock再Unload_bitmap
//,我是attach surface双缓冲之后FLIP，锁住后备画面之后，操作ddsd.lpSurface,再unlock再Unload_bitmap
//##################################################
#define ECHODEBUG 0//不要显示COM对象创建成功消息
#define DEBUG 6

// defines for windows 
#define WINDOW_CLASS_NAME "WINCLASS1"



#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)


// GLOBALS ////////////////////////////////////////////////
//创建主画面和辅助画面（2倍缓冲）
/** ToDo:把下面全局变量的问题都优化成没有全局变量的方式
*/

HWND globalhwnd=NULL;//全局窗口句柄
HINSTANCE main_instance=NULL; //主实例句柄
LPDIRECTDRAWPALETTE globallpddpal;//全局调色板(所谓的全局调色板其实就是为了封装到gameinit里而不得以的做法，有优化,在游戏编程全接触里讲到过！)
LPDIRECTDRAWSURFACE7 lpddsprimary=NULL;
LPDIRECTDRAWSURFACE7 lpddsback=NULL;
LPDIRECTDRAWSURFACE7 lpddsbackground=NULL;
LPDIRECTDRAWSURFACE7 lpddswork=NULL;
//LPDIRECTDRAWSURFACE7 lpddswork2=NULL;

DDSURFACEDESC2 ddsd;
LPDIRECTDRAW7 lpdd=NULL;

LPDIRECTDRAWCLIPPER   lpddclipper  = NULL;   // dd clipper
DDBLTFX ddbltfx;//重要结构
//BITMAP_FILE bitmap,bitmapsp,bitmapemy,bitmapmenu,bitmapcopyright;
BITMAP_FILE bitmap,bitmapsp,bitmapemy;
PALETTEENTRY          palette[256];          // color palette
//LPDIRECTDRAWPALETTE lpddpal=NULL;
char buf2[50];
int mytransform;//全部变量测试BLT位置变化动画
int COPYRIGHT;//一开始的制作公司版权画面，0是未出现，1是已经出现过了
int START_SELECT; //主菜单选择是什么，0是旧的回忆，1是新的游戏,-1代表还未加载

RECT rcRect;

static int offsety,offsetx;//控制主角的偏移
int pause;//是否暂停标记
int g_iFireInputDelay;//控制主角子弹限制发射
int g_iMissileInputDelay;//控制主角导弹限制发射
DWORD dwStart;
const int timelimit=3000;
int mysm;//简单状态的实现

static int current_fps;//当前的fps
static int max_fps;//最大fps
int already_record_max_fps_time;//给一段时间记录下最大的fps
int key_press_counter;//按键计时器，用来阻止连发，强制拆键盘~
int enter_staff_bg;//进入工作人员背景标记
int enter_option_bg;//进入选项背景标记
int stage_num;//关数标记
int sound_ids[20]; // storage for 4 sound fx ids
//全局精灵链表
list <cls_sprite *> sprite_list;
list <cls_sprite *> sprite_list2;
list <cls_sprite *>::iterator sprite_list_iter,temp_sprite_list_iter;
//全局敌机子弹链表
//list <bullet *> enemy_all_bullet_list;
//list <bullet *>::iterator enemy_all_bullet_list_iter,temp_enemy_all_bullet_list_iter;
//全局击坠数
int hits;
//游戏开始时候的选项
int whichopt;
//开始背景的辉光计数器
int shinecounter;
//boss1死亡标记(用来切换地图)
int boss1die;
//boss2死亡标记
int boss2die;
//按钮选项
typedef enum optx{
	startgameopt=1,
	optionopt=2,
	staffopt=3,
	exitopt=4
};
int option_state_which;//记录游戏进入选项的选择
//进入选项后的状态
typedef enum enter_option_state{
option_state_eazy_shoudong=0,
option_state_normal_shoudong=1,
option_state_hard_shoudong=2,
option_state_eazy_lianfa=3,
option_state_normal_lianfa=4,
option_state_hard_lianfa=5
};
int option_edit_which;//记录定位到选择哪项，是难度还是连发
typedef enum enter_option_edit_which{
edit_difficult=0,
edit_lianfa=1
};
int bossstatecounter;//boss状态计数器(Todo,还是用简单状态机)
//prototype
int gameinit();
LPDIRECTDRAWSURFACE7 reloadmap(LPDIRECTDRAW7,int);//重载地图

void mybltclip(HWND hwnd,USHORT * lpsurface,int mempitch);//一个软件版裁剪函数
void mybltclip2(LPDIRECTDRAWSURFACE7 lpddsprimary,
				HWND hwnd,
				int x, 
				int y,
				int width,
				int height,
				UCHAR * bitmap,
				UCHAR * lpSurface,
				int mempitch);

//int DDraw_Fill_Surface(LPDIRECTDRAWSURFACE4 lpdds,int color);


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
LPDIRECTDRAWSURFACE7 mysprite;//设置一个精灵
LPDIRECTDRAWSURFACE7 gamemenu;//设置游戏主菜单
HFONT 		g_hFont;//显示FPS里用
HFONT		status_info_hFont;//显示状态信息字体

int CreateDDSurface( LPDIRECTDRAWSURFACE7* lplpDDS, LPSIZE lpSize, bool gSysMem );

// FUNCTIONS //////////////////////////////////////////////


// WINMAIN ////////////////////////////////////////////////
int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{

WNDCLASSEX winclass; // this will hold the class we create
HWND	   hwnd;	 // generic window handle
MSG		   msg;		 // generic message

// first fill in the window class stucture
winclass.cbSize         = sizeof(WNDCLASSEX);
winclass.style			= CS_DBLCLKS | CS_OWNDC | 
                          CS_HREDRAW | CS_VREDRAW;
winclass.lpfnWndProc	= WindowProc;
winclass.cbClsExtra		= 0;
winclass.cbWndExtra		= 0;
winclass.hInstance		= hinstance;
winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
winclass.lpszMenuName	= NULL;
winclass.lpszClassName	= WINDOW_CLASS_NAME;
winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

// register the window class
if (!RegisterClassEx(&winclass))
	return(0);

// create the window
if (!(hwnd = CreateWindowEx(NULL, // extended style
                            WINDOW_CLASS_NAME,   // class
						    GAME_NAME, // title
						     WS_VISIBLE |WS_POPUP,
					 	    0,0,	    // initial x,y
						    640,480,  // initial width, height
						    NULL,	    // handle to parent 
						    NULL,	    // handle to menu
						    hinstance,// instance of this application
						    NULL)))	// extra creation parms
return(0);
globalhwnd=hwnd;
main_instance=hinstance;
//char *name="dd";
//alertme(1.1);
//alertme("dd");
gameinit();//初始化游戏
//DSound_Play(sound_ids[8]);
whichopt=startgameopt;
RECT des_rect;
	if(DEBUG==1 || DEBUG==2 || DEBUG==3 ){
	des_rect.left=10;
	des_rect.right=140;
	des_rect.top=0;
	des_rect.bottom=170;
	}
	else{
	des_rect.left=0;
	des_rect.right=600;
	des_rect.top=0;
	des_rect.bottom=480;
	}
//Todo:加GAMEDEV_TILE_2的图片



#if 1
if(!(mysprite=Create_Offscreen_Surface(480,480,DDSCAPS_SYSTEMMEMORY))){
	//echo error
	MessageBox(hwnd,"创建离屏画面sprite失败","信息",MB_OK);
}
else{
	//MessageBox(hwnd,"创建离屏画面Ok","信息",MB_OK);
}
#endif
//创建菜单画面
if(!(gamemenu=Create_Offscreen_Surface(480,480,DDSCAPS_SYSTEMMEMORY))){
	//echo error
	MessageBox(hwnd,"创建离屏画面游戏菜单失败","信息",MB_OK);
}

/*
#if 0
if(!(mysprite = DDraw_Create_Surface(240,480,0,-1))){
	MessageBox(hwnd,"创建精灵失败","info",MB_OK);
}
else{
	//MessageBox(hwnd,"创建精灵OK","info",MB_OK);
}
#endif*/

lpddsbackground = DDraw_Create_Surface(640,480,0,-1);

//
// Create a working surface.
//
SIZE size;
size.cx = 640;
size.cy = 480;
CreateDDSurface( &lpddswork, &size, true );
//CreateDDSurface( &lpddswork2, &size, true );
	//读开始bitmap
	//if(!Load_Bitmap_File(hwnd,&bitmapcopyright,"Resource/bmp/start.bmp")){
	//		MessageBox(hwnd,"读取图像ERROR","信息",MB_OK);
	//}
	//读精灵bitmap
	if(Load_Bitmap_File(hwnd,&bitmapsp,"Resource/bmp/bitmap8_1.bmp")){
		if(ECHODEBUG==1)
			MessageBox(hwnd,"读取图像OK","信息",MB_OK);
	}
	//读地图bitmap
	//if(Load_Bitmap_File(hwnd,&bitmap,"Resource/bmp/testbitmap.bmp")){
	//	if(ECHODEBUG==1)
	//		MessageBox(hwnd,"读取图像OK","信息",MB_OK);
	//}
	//读菜单bitmap
	//if(FAILED(Load_Bitmap_File(hwnd,&bitmapmenu,"Resource/bmp/gamemenu.bmp"))){
	//		MessageBox(hwnd,"读取游戏主菜单失败","信息",MB_OK);
	//}
/*	//读菜单bitmap
	if(FAILED(Load_Bitmap_File(hwnd,&bitmapemy,"soldier_sm.bmp"))){
			MessageBox(hwnd,"读取游戏主菜单失败","信息",MB_OK);
	}*/

	//为lpddsbackground扫描位图bitmap8
#if 0
	lpddsbackground->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	// get video pointer to primary surfce
	UCHAR *image_buffer = (UCHAR *)ddsd.lpSurface;       
	
	// test if memory is linear
	if (ddsd.lPitch == 640)
	{
		// copy memory from double buffer to primary buffer
		memcpy((void *)image_buffer, (void *)bitmap.buffer, 640*480);
	} // end if
	else
	{ // non-linear
		
		// make copy of source and destination addresses
		UCHAR *dest_ptr = image_buffer;
		UCHAR *src_ptr  = bitmap.buffer;
		
		// memory is non-linear, copy line by line
		for (int y=0; y < 480; y++)
		{
			// copy line
			memcpy((void *)dest_ptr, (void *)src_ptr, 640);
			
			// advance pointers to next line
			dest_ptr+=ddsd.lPitch;
			src_ptr +=640;
		} // end for
		
	} // end else
	
	// now unlock the primary surface
	if (FAILED(lpddsbackground->Unlock(NULL)))
		return(0);
#endif

	//为lpddsTileset扫描位图bitmap8,原来的扫描图片到画面的方法
#if 0
	
#ifdef USE_GAMEDEV_TILE_2
//lpddsBack = DDraw_Create_Surface(640,480,0,1);
lpddsTileset = DDraw_Create_Surface(640,480,0,1);//这代替了FAILED(lpdd7->CreateSurface(&ddsd, &lpddsTileset, NULL))部分
//但是例子上是一个32*384的竖向的图片。但是下面的读入画面函数是有问题的，只能读640*480的东西！
//ToDo:下面这个函数封装成可以加载到任意大小的画面！
	lpddsTileset->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	// get video pointer to primary surfce
	UCHAR *image_buffer_tileset = (UCHAR *)ddsd.lpSurface;       
	
	// test if memory is linear
	if (ddsd.lPitch == 640)
	{
		// copy memory from double buffer to primary buffer
		memcpy((void *)image_buffer_tileset, (void *)bitmap.buffer, 640*480);
	} // end if
	else
	{ // non-linear
		
		// make copy of source and destination addresses
		UCHAR *dest_ptr = image_buffer_tileset;
		UCHAR *src_ptr  = bitmap.buffer;
		
		// memory is non-linear, copy line by line
		for (int y=0; y < 480; y++)
		{
			// copy line
			memcpy((void *)dest_ptr, (void *)src_ptr, 640);
			
			// advance pointers to next line
			dest_ptr+=ddsd.lPitch;
			src_ptr +=640;
		} // end for
		
	} // end else
	
	// now unlock the primary surface
	if (FAILED(lpddsTileset->Unlock(NULL)))
		return(0);
#endif

#endif
	
#if 0//新的GDI协作方法V1，未修正载入256色图片

lpddsTileset = DDraw_Create_Surface(6400,480,0,1);
//lpddsTileset->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
//下面这个函数不需要解锁，估计自己封装了！正解！
//The GetDC method creates a GDI-compatible device context for the surface. 
//It uses an internal version of the Lock method to lock the surface,
// and the surface will remain locked until ReleaseDC is called.
// See the description of the Lock method for more information on this behavior.
if (!Load_Bitmap256("Resource/bmp/map.bmp",0,0,lpddsTileset))
MessageBox(hwnd,"载入位图失败！","",NULL);
//lpddsTileset->Unlock(NULL);
#endif
#if 0
lpddsTileset = DDraw_Create_Surface(1298,1446,0,1);
if (!Loadbmp2Surface("big.bmp",0,0,lpddsTileset))
MessageBox(hwnd,"载入位图失败！","",NULL);
#endif

#if 1//现在开始使用这个函数！
//from gamedev new test
//lpddsTileset = DDraw_Create_Surface(6400,480,0,1);
//lpddsTileset = CreateOffScreenSurface(lpdd,640,480);



//lpddsTileset=DDLoadBitmap(lpdd,"Resource/bmp/map.bmp");
//lpddsTileset=DDLoadBitmap(lpdd,"Resource/bmp/map1.bmp");
lpddsTileset=reloadmap(lpdd,0);

//lpddsTileset=reloadmap(lpdd,0);
#endif

//初始化精灵类
cls_sprite_surface *sfobj=new cls_sprite_surface();
LPDIRECTDRAWSURFACE7 girl_swordman_surface_test=NULL;
LPDIRECTDRAWSURFACE7 girl_swordman_surface_test1=NULL;
LPDIRECTDRAWSURFACE7 girl_swordman_surface_test2=NULL;
LPDIRECTDRAWSURFACE7 girl_swordman_surface_red=NULL;
LPDIRECTDRAWSURFACE7 player_plane=NULL;
LPDIRECTDRAWSURFACE7 cloud_surface=NULL;
LPDIRECTDRAWSURFACE7 boss1_surface=NULL;
LPDIRECTDRAWSURFACE7 boss1shadow_surface=NULL;
LPDIRECTDRAWSURFACE7 bullet0_surface=NULL;
LPDIRECTDRAWSURFACE7 bullet1_surface=NULL;
LPDIRECTDRAWSURFACE7 bullet2_surface=NULL;
LPDIRECTDRAWSURFACE7 bullet3_surface=NULL;
//LPDIRECTDRAWSURFACE7 boss2_surface=NULL;
LPDIRECTDRAWSURFACE7 enemy0_surface=NULL;
LPDIRECTDRAWSURFACE7 enemy1_surface=NULL;
LPDIRECTDRAWSURFACE7 enemy2_surface=NULL;
LPDIRECTDRAWSURFACE7 enemy3_surface=NULL;
LPDIRECTDRAWSURFACE7 enemy3_red_surface=NULL;
LPDIRECTDRAWSURFACE7 enemy4_surface=NULL;
LPDIRECTDRAWSURFACE7 expl_surface=NULL;
LPDIRECTDRAWSURFACE7 spark_surface=NULL;
LPDIRECTDRAWSURFACE7 bossspark_surface=NULL;
LPDIRECTDRAWSURFACE7 bigexpl_surface=NULL;
LPDIRECTDRAWSURFACE7 e_bullet_surface=NULL;
LPDIRECTDRAWSURFACE7 big_e_bullet_surface=NULL;
LPDIRECTDRAWSURFACE7 soft_e_bullet_surface=NULL;
LPDIRECTDRAWSURFACE7 leftbar_surface=NULL;
LPDIRECTDRAWSURFACE7 mainloginbg_surface=NULL;
LPDIRECTDRAWSURFACE7 pause_surface=NULL;
LPDIRECTDRAWSURFACE7 rightbar_surface=NULL;
//LPDIRECTDRAWSURFACE7 smallboss_surface=NULL;//Todo bug!为什么不能删掉这句！
LPDIRECTDRAWSURFACE7 ro_bullet_surface=NULL;
LPDIRECTDRAWSURFACE7 gameover_surface=NULL;
LPDIRECTDRAWSURFACE7 mission1_surface=NULL;
LPDIRECTDRAWSURFACE7 mission2_surface=NULL;
LPDIRECTDRAWSURFACE7 tank_surface=NULL;
LPDIRECTDRAWSURFACE7 normal_e_bullet_surface=NULL;
LPDIRECTDRAWSURFACE7 tank_e_bullet_surface=NULL;
LPDIRECTDRAWSURFACE7 alphabg_surface=NULL;
LPDIRECTDRAWSURFACE7 alphabg_pause_surface=NULL;
LPDIRECTDRAWSURFACE7 power_surface=NULL;
LPDIRECTDRAWSURFACE7 staff_surface=NULL;
LPDIRECTDRAWSURFACE7 product_surface=NULL;
LPDIRECTDRAWSURFACE7 underline_surface=NULL;
LPDIRECTDRAWSURFACE7 opt1_surface=NULL;
LPDIRECTDRAWSURFACE7 opt2_surface=NULL;
LPDIRECTDRAWSURFACE7 opt3_surface=NULL;
LPDIRECTDRAWSURFACE7 opt4_surface=NULL;
LPDIRECTDRAWSURFACE7 option_surface=NULL;
LPDIRECTDRAWSURFACE7 option_difficult_surface=NULL;
LPDIRECTDRAWSURFACE7 option_repeatfire_surface=NULL;
LPDIRECTDRAWSURFACE7 missile_surface=NULL;
LPDIRECTDRAWSURFACE7 missile_surface0=NULL;
LPDIRECTDRAWSURFACE7 missile_surface1=NULL;
LPDIRECTDRAWSURFACE7 missile_surface2=NULL;
LPDIRECTDRAWSURFACE7 missile_surface3=NULL;
LPDIRECTDRAWSURFACE7 missile_surface4=NULL;
LPDIRECTDRAWSURFACE7 missile_surface5=NULL;
LPDIRECTDRAWSURFACE7 missile_surface6=NULL;
LPDIRECTDRAWSURFACE7 missile_surface7=NULL;
LPDIRECTDRAWSURFACE7 missile_surface8=NULL;
LPDIRECTDRAWSURFACE7 missile_surface9=NULL;
LPDIRECTDRAWSURFACE7 missile_surface10=NULL;
LPDIRECTDRAWSURFACE7 missile_surface11=NULL;
LPDIRECTDRAWSURFACE7 missile_surface12=NULL;
LPDIRECTDRAWSURFACE7 missile_surface13=NULL;
LPDIRECTDRAWSURFACE7 missile_surface14=NULL;
LPDIRECTDRAWSURFACE7 missile_surface15=NULL;
LPDIRECTDRAWSURFACE7 option_eazy_surface=NULL;
LPDIRECTDRAWSURFACE7 option_normal_surface=NULL;
LPDIRECTDRAWSURFACE7 option_hard_surface=NULL;
LPDIRECTDRAWSURFACE7 option_on_surface=NULL;
LPDIRECTDRAWSURFACE7 option_off_surface=NULL;
LPDIRECTDRAWSURFACE7 option_eazy1_surface=NULL;
LPDIRECTDRAWSURFACE7 option_normal1_surface=NULL;
LPDIRECTDRAWSURFACE7 option_hard1_surface=NULL;
LPDIRECTDRAWSURFACE7 option_on1_surface=NULL;
LPDIRECTDRAWSURFACE7 option_off1_surface=NULL;
LPDIRECTDRAWSURFACE7 boss2_surface=NULL;
LPDIRECTDRAWSURFACE7 boss2spark_surface=NULL;
//设计思路：对一个素材画面直接能够创建1组精灵，然后操纵这组精灵
#if 0
girl_swordman_surface_test=sfobj->getSpriteSurface(girl_swordman_surface_test,"蓝色女剑士素材",\
"蓝衣女剑士","Resource/bmp/swordwoman.bmp",4);
//下面这个是多余的调用，仅仅返回指针
girl_swordman_surface_test1=sfobj->getSpriteSurface(girl_swordman_surface_test1,"蓝色女剑士素材",\
"蓝衣女剑士","Resource/bmp/swordwoman.bmp",4);
girl_swordman_surface_test2=sfobj->getSpriteSurface(girl_swordman_surface_test2,"紫色女剑士素材",\
"紫衣女剑士","Resource/bmp/swordwoman2.bmp",4);
girl_swordman_surface_red=sfobj->getSpriteSurface(girl_swordman_surface_red,"红色女剑士素材",\
"红衣女剑士","Resource/bmp/swordwomanred.bmp",4);
#endif
int player_seqnum=6;
player_plane=sfobj->getSpriteSurface(player_plane,"玩家飞机素材","玩家飞机","Resource/bmp/playerplane.bmp",player_seqnum);
int player_sfid=friend_cls_sprite_surface_getmax(sfobj);//获取当前素材画面管理器里id

//int player_sfid=sfobj->get_max_sprite_surface(1);效果一样的！
//cloud_surface=sfobj->getSpriteSurface(cloud_surface,"云雾素材","云雾","Resource/bmp/cloud.bmp",1);
boss1_surface=sfobj->getSpriteSurface(boss1_surface,"绿色改boss素材","绿色改boss","Resource/bmp/boss1.bmp",6);
int boss1_sfid=sfobj->get_max_sprite_surface(0);

//boss1shadow_surface=sfobj->getSpriteSurface(boss1shadow_surface,"绿色改boss影子素材","绿色改boss影子","Resource/bmp/boss1_shadow.bmp",1);
//int boss1_shadow_sfid=sfobj->get_max_sprite_surface(0);

bullet0_surface=sfobj->getSpriteSurface(bullet0_surface,"等级1子弹素材","等级1子弹","Resource/bmp/bullet0.bmp",1);
int bullet0_sfid=sfobj->get_max_sprite_surface(0);
bullet1_surface=sfobj->getSpriteSurface(bullet1_surface,"等级2子弹素材","等级2子弹","Resource/bmp/bullet1.bmp",1);
int bullet1_sfid=sfobj->get_max_sprite_surface(0);
bullet2_surface=sfobj->getSpriteSurface(bullet2_surface,"等级3子弹素材","等级3子弹","Resource/bmp/bullet2.bmp",1);
int bullet2_sfid=sfobj->get_max_sprite_surface(0);
bullet3_surface=sfobj->getSpriteSurface(bullet3_surface,"等级4子弹素材","等级4子弹","Resource/bmp/bullet3.bmp",1);
int bullet3_sfid=sfobj->get_max_sprite_surface(0);
sfobj->getSpriteSurface(e_bullet_surface,"敌机子弹素材","敌机子弹","Resource/bmp/e_bullet.bmp",1);
int e_bullet_sfid=sfobj->get_max_sprite_surface(0);
sfobj->getSpriteSurface(big_e_bullet_surface,"大敌机子弹素材","大敌机子弹","Resource/bmp/big_e_bullet.bmp",6);
int big_e_bullet_sfid=sfobj->get_max_sprite_surface(0);
sfobj->getSpriteSurface(soft_e_bullet_surface,"可打掉敌机子弹素材","可打掉敌机子弹","Resource/bmp/soft_e_bullet.bmp",6);
int soft_e_bullet_sfid=sfobj->get_max_sprite_surface(0);

enemy0_surface=sfobj->getSpriteSurface(enemy0_surface,"敌机0素材","敌机0","Resource/bmp/enemy0.bmp",6);
int enemy0_sfid=sfobj->get_max_sprite_surface(0);
//alertme("调试了");
enemy1_surface=sfobj->getSpriteSurface(enemy1_surface,"敌机1素材","敌机1","Resource/bmp/enemy1.bmp",6);
int enemy1_sfid=sfobj->get_max_sprite_surface(0);
enemy2_surface=sfobj->getSpriteSurface(enemy2_surface,"敌机2素材","敌机2","Resource/bmp/enemy2.bmp",6);
int enemy2_sfid=sfobj->get_max_sprite_surface(0);
enemy3_surface=sfobj->getSpriteSurface(enemy3_surface,"敌机3素材","敌机3","Resource/bmp/enemy3.bmp",6);
int enemy3_sfid=sfobj->get_max_sprite_surface(0);
enemy3_red_surface=sfobj->getSpriteSurface(enemy3_red_surface,"敌机3红素材","敌机3红","Resource/bmp/enemy3_red.bmp",6);
int enemy3_red_sfid=sfobj->get_max_sprite_surface(0);

enemy4_surface=sfobj->getSpriteSurface(enemy4_surface,"敌机4素材","敌机4","Resource/bmp/enemy4.bmp",6);
int enemy4_sfid=sfobj->get_max_sprite_surface(0);

expl_surface=sfobj->getSpriteSurface(expl_surface,"爆炸素材","爆炸","Resource/bmp/expl.bmp",14,false);
int expl_sfid=sfobj->get_max_sprite_surface(0);


bossspark_surface=sfobj->getSpriteSurface(bossspark_surface,"boss火花素材","boss火花","Resource/bmp/bossspark.bmp",2);
int bossspark_sfid=sfobj->get_max_sprite_surface(0);
boss2spark_surface=sfobj->getSpriteSurface(boss2spark_surface,"boss2火花素材","boss2火花","Resource/bmp/boss2spark.bmp",2);
int boss2spark_sfid=sfobj->get_max_sprite_surface(0);
spark_surface=sfobj->getSpriteSurface(spark_surface,"火花素材","火花","Resource/bmp/spark.bmp",2);
int spark_sfid=sfobj->get_max_sprite_surface(0);

bigexpl_surface=sfobj->getSpriteSurface(bigexpl_surface,"大爆炸素材","大爆炸","Resource/bmp/bigexpl.bmp",28);
int bigexpl_sfid=sfobj->get_max_sprite_surface(0);
//leftbar_surface=sfobj->getSpriteSurface(leftbar_surface,"左侧边栏素材","左侧边栏","Resource/bmp/leftbar.bmp",1);
JPEGINFO jpgf;
LoadJPEG(FROM_FILE,"Resource/bmp/leftbar.jpg",&leftbar_surface,lpdd,jpgf);
//MessageBeep(MB_ICONASTERISK);
//int leftbar_sfid=sfobj->get_max_sprite_surface(0);
//rightbar_surface=sfobj->getSpriteSurface(rightbar_surface,"右侧边栏素材","右侧边栏","Resource/bmp/rightbar.bmp",1);
//int rightbar_sfid=sfobj->get_max_sprite_surface(0);
LoadJPEG(FROM_FILE,"Resource/bmp/rightbar.jpg",&rightbar_surface,lpdd,jpgf);
mainloginbg_surface=sfobj->getSpriteSurface(mainloginbg_surface,"主登陆界面素材","主登陆界面","Resource/bmp/startbg.bmp",9);
int mainloginbg_sfid=sfobj->get_max_sprite_surface(0);
pause_surface=sfobj->getSpriteSurface(pause_surface,"暂停素材","暂停","Resource/bmp/pause.bmp",1);
int pause_sfid=sfobj->get_max_sprite_surface(0);
//smallboss_surface=sfobj->getSpriteSurface(smallboss_surface,"小boss素材","小boss","Resource/bmp/smallboss.bmp",2);
//int smallboss_sfid=sfobj->get_max_sprite_surface(0);
ro_bullet_surface=sfobj->getSpriteSurface(ro_bullet_surface,"旋转子弹素材","旋转子弹","Resource/bmp/ro_bullet.bmp",12);
int ro_bullet_sfid=sfobj->get_max_sprite_surface(0);
gameover_surface=sfobj->getSpriteSurface(gameover_surface,"gameover素材","gameover","Resource/bmp/gameover.bmp",1);
int gameover_sfid=sfobj->get_max_sprite_surface(0);
mission1_surface=sfobj->getSpriteSurface(mission1_surface,"mission1素材","mission1","Resource/bmp/mission1.bmp",1);
int mission1_sfid=sfobj->get_max_sprite_surface(0);

mission2_surface=sfobj->getSpriteSurface(mission2_surface,"mission2素材","mission2","Resource/bmp/mission2.bmp",1);
int mission2_sfid=sfobj->get_max_sprite_surface(0);

tank_surface=sfobj->getSpriteSurface(tank_surface,"tank素材","tank","Resource/bmp/tank.bmp",8);
int tank_sfid=sfobj->get_max_sprite_surface(0);
normal_e_bullet_surface=sfobj->getSpriteSurface(normal_e_bullet_surface,"普通旋转子弹素材","普通旋转子弹","Resource/bmp/normal_e_bullet.bmp",8);
int normal_e_bullet_sfid=sfobj->get_max_sprite_surface(0);
tank_e_bullet_surface=sfobj->getSpriteSurface(tank_e_bullet_surface,"坦克子弹素材","坦克子弹","Resource/bmp/tank_e_bullet.bmp",8);
int tank_e_bullet_sfid=sfobj->get_max_sprite_surface(0);
//alphabg_surface=sfobj->getSpriteSurface(alphabg_surface,"alpha背景素材","alpha背景","Resource/bmp/alphabg.bmp",1);
alphabg_surface=sfobj->getSpriteSurfaceJPG(alphabg_surface,"alpha背景素材","alpha背景","Resource/bmp/alphabg.jpg",1);
int alphabg_sfid=sfobj->get_max_sprite_surface(0);
//alphabg_pause_surface=sfobj->getSpriteSurfaceJPG(alphabg_pause_surface,"alpha暂停背景素材","alpha暂停背景","Resource/bmp/alphabg_pause.jpg",1);
//int alphabg_pause_sfid=sfobj->get_max_sprite_surface(0);
power_surface=sfobj->getSpriteSurface(power_surface,"power背景素材","power背景","Resource/bmp/power.bmp",2);
int power_sfid=sfobj->get_max_sprite_surface(0);
//staff_surface=sfobj->getSpriteSurface(staff_surface,"staff素材","staff素材","Resource/bmp/staff.bmp",1);
staff_surface=sfobj->getSpriteSurfaceJPG(staff_surface,"staff素材","staff素材","Resource/bmp/staff.jpg",1);
int staff_sfid=sfobj->get_max_sprite_surface(0);
//option_surface=sfobj->getSpriteSurface(option_surface,"option素材","option素材","Resource/bmp/option.bmp",1);
option_surface=sfobj->getSpriteSurfaceJPG(option_surface,"option素材","option素材","Resource/bmp/option.jpg",1);
//LPDIRECTDRAWSURFACE7 ppps=NULL;
///ppps=DDLoadBitmap(lpdd,"Resource/bmp/product.bmp");
//LPDIRECTDRAWSURFACE7 girl_swordman_surface;
//	girl_swordman_surface=DDLoadBitmap(lpdd,"Resource/bmp/bitmap8_1.bmp");
//JPEGINFO ji;
//LoadJPEG(FROM_FILE,"Resource/bmp/example1s.jpg",&ppps,lpdd,ji);

int option_sfid=sfobj->get_max_sprite_surface(0);
product_surface=sfobj->getSpriteSurface(product_surface,"出品素材","出品素材","Resource/bmp/product.bmp",1);
int product_sfid=sfobj->get_max_sprite_surface(0);
underline_surface=sfobj->getSpriteSurface(underline_surface,"下划线素材","下划线素材","Resource/bmp/underline.bmp",1);
int underline_sfid=sfobj->get_max_sprite_surface(0);
opt1_surface=sfobj->getSpriteSurface(opt1_surface,"菜单项1素材","菜单项1素材","Resource/bmp/opt1.bmp",1);
int opt1_sfid=sfobj->get_max_sprite_surface(0);
opt2_surface=sfobj->getSpriteSurface(opt2_surface,"菜单项2素材","菜单项2素材","Resource/bmp/opt2.bmp",1);
int opt2_sfid=sfobj->get_max_sprite_surface(0);
opt3_surface=sfobj->getSpriteSurface(opt3_surface,"菜单项3素材","菜单项3素材","Resource/bmp/opt3.bmp",1);
int opt3_sfid=sfobj->get_max_sprite_surface(0);
opt4_surface=sfobj->getSpriteSurface(opt4_surface,"菜单项4素材","菜单项4素材","Resource/bmp/opt4.bmp",1);
int opt4_sfid=sfobj->get_max_sprite_surface(0);
option_difficult_surface=sfobj->getSpriteSurface(option_difficult_surface,"难度选项素材","难度素材","Resource/bmp/option_difficult.bmp",1);
int option_difficult_sfid=sfobj->get_max_sprite_surface(0);
option_repeatfire_surface=sfobj->getSpriteSurface(option_repeatfire_surface,"连发选项素材","连发素材","Resource/bmp/option_repeatfire.bmp",1);
int option_repeatfire_sfid=sfobj->get_max_sprite_surface(0);
missile_surface=sfobj->getSpriteSurface(missile_surface,"导弹素材","导弹素材","Resource/bmp/missile.bmp",16);
int missile_sfid=sfobj->get_max_sprite_surface(0);
missile_surface0=sfobj->getSpriteSurface(missile_surface0,"导弹0素材","导弹0素材","Resource/bmp/missile0.bmp",1);
int missile0_sfid=sfobj->get_max_sprite_surface(0);
missile_surface1=sfobj->getSpriteSurface(missile_surface1,"导弹1素材","导弹1素材","Resource/bmp/missile1.bmp",1);
int missile1_sfid=sfobj->get_max_sprite_surface(0);
missile_surface2=sfobj->getSpriteSurface(missile_surface2,"导弹2素材","导弹2素材","Resource/bmp/missile2.bmp",1);
int missile2_sfid=sfobj->get_max_sprite_surface(0);
missile_surface3=sfobj->getSpriteSurface(missile_surface3,"导弹3素材","导弹3素材","Resource/bmp/missile3.bmp",1);
int missile3_sfid=sfobj->get_max_sprite_surface(0);
missile_surface4=sfobj->getSpriteSurface(missile_surface4,"导弹4素材","导弹4素材","Resource/bmp/missile4.bmp",1);
int missile4_sfid=sfobj->get_max_sprite_surface(0);
missile_surface5=sfobj->getSpriteSurface(missile_surface5,"导弹5素材","导弹5素材","Resource/bmp/missile5.bmp",1);
int missile5_sfid=sfobj->get_max_sprite_surface(0);
missile_surface6=sfobj->getSpriteSurface(missile_surface6,"导弹6素材","导弹6素材","Resource/bmp/missile6.bmp",1);
int missile6_sfid=sfobj->get_max_sprite_surface(0);
missile_surface7=sfobj->getSpriteSurface(missile_surface7,"导弹7素材","导弹7素材","Resource/bmp/missile7.bmp",1);
int missile7_sfid=sfobj->get_max_sprite_surface(0);
missile_surface8=sfobj->getSpriteSurface(missile_surface8,"导弹8素材","导弹8素材","Resource/bmp/missile8.bmp",1);
int missile8_sfid=sfobj->get_max_sprite_surface(0);
missile_surface9=sfobj->getSpriteSurface(missile_surface9,"导弹9素材","导弹9素材","Resource/bmp/missile9.bmp",1);
int missile9_sfid=sfobj->get_max_sprite_surface(0);
missile_surface10=sfobj->getSpriteSurface(missile_surface10,"导弹10素材","导弹10素材","Resource/bmp/missile10.bmp",1);
int missile10_sfid=sfobj->get_max_sprite_surface(0);
missile_surface11=sfobj->getSpriteSurface(missile_surface11,"导弹11素材","导弹11素材","Resource/bmp/missile11.bmp",1);
int missile11_sfid=sfobj->get_max_sprite_surface(0);
missile_surface12=sfobj->getSpriteSurface(missile_surface12,"导弹12素材","导弹12素材","Resource/bmp/missile12.bmp",1);
int missile12_sfid=sfobj->get_max_sprite_surface(0);
missile_surface13=sfobj->getSpriteSurface(missile_surface13,"导弹13素材","导弹13素材","Resource/bmp/missile13.bmp",1);
int missile13_sfid=sfobj->get_max_sprite_surface(0);
missile_surface14=sfobj->getSpriteSurface(missile_surface14,"导弹14素材","导弹14素材","Resource/bmp/missile14.bmp",1);
int missile14_sfid=sfobj->get_max_sprite_surface(0);
missile_surface15=sfobj->getSpriteSurface(missile_surface15,"导弹15素材","导弹15素材","Resource/bmp/missile15.bmp",1);
int missile15_sfid=sfobj->get_max_sprite_surface(0);
option_eazy_surface=sfobj->getSpriteSurface(option_eazy_surface,"选项简单素材","选项简单素材","Resource/bmp/option_eazy.bmp",1);
int option_eazy_sfid=sfobj->get_max_sprite_surface(0);
option_eazy1_surface=sfobj->getSpriteSurface(option_eazy1_surface,"选项简单1素材","选项简单1素材","Resource/bmp/option_eazy1.bmp",1);
int option_eazy1_sfid=sfobj->get_max_sprite_surface(0);
option_normal_surface=sfobj->getSpriteSurface(option_normal_surface,"选项一般素材","选项一般素材","Resource/bmp/option_normal.bmp",1);
int option_normal_sfid=sfobj->get_max_sprite_surface(0);
option_normal1_surface=sfobj->getSpriteSurface(option_normal1_surface,"选项一般1素材","选项一般1素材","Resource/bmp/option_normal1.bmp",1);
int option_normal1_sfid=sfobj->get_max_sprite_surface(0);
option_hard_surface=sfobj->getSpriteSurface(option_hard_surface,"选项困难素材","选项困难素材","Resource/bmp/option_hard.bmp",1);
int option_hard_sfid=sfobj->get_max_sprite_surface(0);
option_hard1_surface=sfobj->getSpriteSurface(option_hard1_surface,"选项困难1素材","选项困难1素材","Resource/bmp/option_hard1.bmp",1);
int option_hard1_sfid=sfobj->get_max_sprite_surface(0);
option_on_surface=sfobj->getSpriteSurface(option_on_surface,"选项打开素材","选项打开素材","Resource/bmp/option_on.bmp",1);
int option_on_sfid=sfobj->get_max_sprite_surface(0);
option_on1_surface=sfobj->getSpriteSurface(option_on1_surface,"选项打开1素材","选项打开1素材","Resource/bmp/option_on1.bmp",1);
int option_on1_sfid=sfobj->get_max_sprite_surface(0);
option_off_surface=sfobj->getSpriteSurface(option_off_surface,"选项关闭素材","选项关闭素材","Resource/bmp/option_off.bmp",1);
int option_off_sfid=sfobj->get_max_sprite_surface(0);
option_off1_surface=sfobj->getSpriteSurface(option_off1_surface,"选项关闭1素材","选项关闭1素材","Resource/bmp/option_off1.bmp",1);
int option_off1_sfid=sfobj->get_max_sprite_surface(0);
boss2_surface=sfobj->getSpriteSurface(boss2_surface,"boss2素材","boss2素材","Resource/bmp/boss2.bmp",6);
int boss2_sfid=sfobj->get_max_sprite_surface(0);
//init player
//初始化主角（战机）
cls_player *player=new cls_player();
POINT pt_startpoint;
pt_startpoint.x=300;
pt_startpoint.y=480;
//设置主角初始位置
player->SetPos(pt_startpoint);
player->sprite_surface_id=player_sfid;
player->SetAnimateSequence("0,1,2,3,4,5");
//player->SetAnimateData("23|33,23|33,23|33,14|33,23|33,14|33");
player->SetAnimateData("33|33,33|33,33|33,24|33,33|33,24|33");
player->current_frame=0;
player->SetVelocity(4);
player->SetHp(4);
player->SetState(starting);


list<cls_enemy *> enemy_list;
list<cls_enemy * >::iterator enemy_iter,temp_enemy_iter;
#if 0
//初始化第一批敌机，共6个
for (int i=0;i<=8;i++){
	enemy_list.push_back((cls_enemy *)new cls_enemy());
//}
//pt_startpoint.x=130;
//pt_startpoint.y=230;

//list<cls_enemy * >::iterator   temp_enemy_iter;//用来替换删除的临时迭代器
//enemy_iter=enemy_list.begin();
//while(enemy_iter!=enemy_list.end()){
	floatpoint ptf_startpoint;
	ptf_startpoint.x=rand()%640;
	ptf_startpoint.y=rand()%480-480;
	enemy_list.back()->SetPos(ptf_startpoint);
	enemy_list.back()->sprite_surface_id=enemy3_sfid;
	enemy_list.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
	enemy_list.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
// 	(*enemy_iter)->sprite_surface_id=enemy4_sfid;
// 	(*enemy_iter)->SetAnimateSequence("0,1");
// 	(*enemy_iter)->SetAnimateData("167|152,167|152");
	enemy_list.back()->current_frame=(rand()%1);
	enemy_list.back()->SetVelocity(4);
	enemy_list.back()->SetAiState(1);
	enemy_list.back()->iFireInputDelay=23;
	if (i==4)//带一个奖励
	{
		enemy_list.back()->sprite_surface_id=enemy3_red_sfid;
		enum bonus bs=power;//加enum可以适应c编译器！
		enemy_list.back()->SetBonus(bs);
		enemy_list.back()->SetHP(9);

	}
//	++enemy_iter;
}
#endif


//boss1
cls_boss * boss1,*boss2=NULL;
floatpoint ptf_startpoint;

#if 0
cls_boss *boss1=new cls_boss();
ptf_startpoint.x=500;
ptf_startpoint.y=0;
boss1->SetPos(ptf_startpoint);
boss1->sprite_surface_id=boss1_sfid;
boss1->SetAnimateSequence("0,1,2,3,4,5");
boss1->SetAnimateData("216|226,216|226,216|226,216|226,216|226,216|226");
boss1->current_frame=1;
boss1->SetVelocity(3);
boss1->SetHP(650);
boss1->SetState(boss_normalattack);
#endif
//爆炸效果
cls_sprite *expl=new cls_sprite();
//POINT pt_startpoint;
//pt_startpoint.x=300;
//pt_startpoint.y=300;
//expl->SetPos(pt_startpoint);
/*expl->sprite_surface_id=expl_sfid;
expl->SetAnimateSequence("0,1,2,3,4,5,6,7,8");
expl->SetAnimateData("23|33,23|33,23|33,14|33,23|33,14|33");
expl->SetSeqNum(9);
expl->current_frame=0;
expl->SetVelocity(0.2);*/

//大爆炸效果
cls_sprite *bigexpl=new cls_sprite();

//火花效果
cls_sprite *spark=new cls_sprite();
//POINT pt_startpoint;
//pt_startpoint.x=300;
//pt_startpoint.y=300;
//expl->SetPos(pt_startpoint);
spark->sprite_surface_id=spark_sfid;
spark->SetAnimateSequence("0,1,0,1,0,0,1,1,1,1,0,0,0");
//expl->SetAnimateData("23|33,23|33,23|33,14|33,23|33,14|33");
spark->SetSeqNum(13);
spark->current_frame=0;
spark->SetVelocity(5);

//boss火花效果
cls_sprite *bossspark=new cls_sprite();
//POINT pt_startpoint;
//pt_startpoint.x=300;
//pt_startpoint.y=300;
//expl->SetPos(pt_startpoint);
bossspark->sprite_surface_id=bossspark_sfid;
bossspark->SetAnimateSequence("0,1");
//expl->SetAnimateData("23|33,23|33,23|33,14|33,23|33,14|33");
bossspark->SetSeqNum(2);
bossspark->current_frame=0;
bossspark->SetVelocity(.3);

//boss2火花效果
cls_sprite *boss2spark=new cls_sprite();
//POINT pt_startpoint;
//pt_startpoint.x=300;
//pt_startpoint.y=300;
//expl->SetPos(pt_startpoint);
boss2spark->sprite_surface_id=bossspark_sfid;
boss2spark->SetAnimateSequence("0");
//expl->SetAnimateData("23|33,23|33,23|33,14|33,23|33,14|33");
boss2spark->SetSeqNum(2);
bossspark->current_frame=0;
bossspark->SetVelocity(.3);


cls_ai * ai=new cls_ai();
//ai->Set_AI_File_Dir("Script");//Todo:可以一次性设置路径的实现

ai->Open_AI_File("Script/enemy_move.lua");


cls_ai * boss1ai=new cls_ai();
boss1ai->Open_AI_File("Script/boss1_move.lua");

cls_ai * powerai=new cls_ai();
powerai->Open_AI_File("Script/power_move.lua");
//static player_plane_aniseq[]={0,1};//空战游戏主角的动画2帧

//为mysprite扫描位图bitmap8_1，主角（暂时是女剑士）
#if 1
mysprite->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
// get video pointer to primary surfce
UCHAR *image_buffersp = (UCHAR *)ddsd.lpSurface;       

// test if memory is linear
if (ddsd.lPitch == 640)
   {
   // copy memory from double buffer to primary buffer
   memcpy((void *)image_buffersp, (void *)bitmapsp.buffer, 640*480);
   } // end if
else
   { // non-linear

   // make copy of source and destination addresses
   UCHAR *dest_ptr = image_buffersp;
   UCHAR *src_ptr  = bitmapsp.buffer;

   // memory is non-linear, copy line by line
   for (int y=0; y < 480; y++)
       {
       // copy line
       memcpy((void *)dest_ptr, (void *)src_ptr, 640);

       // advance pointers to next line
       dest_ptr+=ddsd.lPitch;
       src_ptr +=640;
       } // end for

   } // end else
if (FAILED(mysprite->Unlock(NULL)))
   return(0);
#endif


//初始化好这个精灵的数据结构
static girl_swordman_stand_aniseq[]={0,1,2,3};//站立姿势4帧
static girl_swordman_run_aniseq[]={0,1,2,3,4,5,6,7};//走路姿势8帧
static girl_swordman_attach_aniseq[]={0,1,2,3,4,5};//攻击姿势6帧
static enemy_1_aniseq[]={0,1};
static int player_plane_aniseq[]={0,1,2,3,4,5};//空战游戏主角的动画2帧
static enemy_plane_aniseq[]={0,1};//一般敌机的动画1帧
//static av_boss_aniseq[]={0,1};




//boss2_surface=sfobj->getSpriteSurface(boss2_surface,"AVboss素材","AVboss","boss2.bmp",2);
SPRITE_OBJ girl_swordman;//栈方法的结构对象定义！
SPRITE_OBJ enemy_1;

girl_swordman.frames[0]=DDraw_Create_Surface(239,239,3,1);//创建好四个画面
girl_swordman.frames[1]=DDraw_Create_Surface(239,239,3,1);//创建好四个画面
girl_swordman.frames[2]=DDraw_Create_Surface(239,239,3,1);//创建好四个画面
girl_swordman.frames[3]=DDraw_Create_Surface(239,239,3,1);//创建好四个画面


girl_swordman.velocity=4;
girl_swordman.counter=0;
girl_swordman.x=300;
girl_swordman.y=100;
enemy_1.frames[0]=DDraw_Create_Surface(32,32,0,1);
enemy_1.frames[1]=DDraw_Create_Surface(32,32,0,1);
enemy_1.velocity=1;
enemy_1.counter=0;
enemy_1.x=98;
enemy_1.y=177;

//为女剑士对象加载图片
#if 0//windows GMP的老方法
//scan_bitmap函数
    Scan_Image_Bitmap(&bitmapsp,                 // bitmap file to scan image data from
                      girl_swordman.frames[0], // surface to hold data
                      0, 0);               // cell to scan image from    

	Scan_Image_Bitmap(&bitmapsp,                 // bitmap file to scan image data from
                      girl_swordman.frames[1], // surface to hold data
                      0, 1);               // cell to scan image from   
	Scan_Image_Bitmap(&bitmapsp,                 // bitmap file to scan image data from
                      girl_swordman.frames[2], // surface to hold data
                      1,0);               // cell to scan image from   
	Scan_Image_Bitmap(&bitmapsp,                 // bitmap file to scan image data from
                      girl_swordman.frames[3], // surface to hold data
                      1, 1);               // cell to scan image from    
#endif
//	DDBLTFX ddbltfx;
DDRAW_INIT_STRUCT(ddbltfx);//初始化fx结构
ddbltfx.dwFillColor = _RGB16BIT565(0,0,0);//给后备缓冲填充颜色
#if 1//新方法，256和16色兼容的新函数调用，还未封装读取精灵函数
	LPDIRECTDRAWSURFACE7 girl_swordman_surface;
	girl_swordman_surface=DDLoadBitmap(lpdd,"Resource/bmp/bitmap8_1.bmp");
	//bmpinfo *bmp_info=(bmpinfo *)malloc(sizeof(bmpinfo));
	//girl_swordman_surface=DDLoadBitmap2(lpdd,"big.bmp",*bmp_info);

	/*
	char buf[200];
	sprintf(buf,"%d",bmp_info->bm.bmHeight);
	MessageBox(hwnd,buf,"",MB_OK);*/


	RECT DestRect;
	DestRect.top=0;
	DestRect.left=0;
	DestRect.right=239;
	DestRect.bottom=239;
	RECT SrcRect;
	SrcRect.top=0;
	SrcRect.left=0;
	SrcRect.right=239;
	SrcRect.bottom=239;
	/*girl_swordman.frames[0]->Blt(
		&DestRect,
		girl_swordman_surface,
		&SrcRect,
		DDBLT_COLORFILL | DDBLT_WAIT,
		&ddbltfx
		);*/

	
	if (FAILED(girl_swordman.frames[0]->Blt(&DestRect, // pointer to dest RECT
		girl_swordman_surface, // pointer to source surface
		&SrcRect, // pointer to source RECT
		DDBLT_WAIT, 
		&ddbltfx))) // pointer to DDBLTFX holding info
	{}
	SrcRect.top=0;
	SrcRect.left=0+239+1;
	SrcRect.right=239+239+1;
	SrcRect.bottom=239;
	if (FAILED(girl_swordman.frames[1]->Blt(&DestRect, // pointer to dest RECT
		girl_swordman_surface, // pointer to source surface
		&SrcRect, // pointer to source RECT
		DDBLT_WAIT, 
		&ddbltfx))) // pointer to DDBLTFX holding info
	{}
	SrcRect.top=239+1;
	SrcRect.left=0;
	SrcRect.right=239;
	SrcRect.bottom=239+239+1;
	if (FAILED(girl_swordman.frames[2]->Blt(&DestRect, // pointer to dest RECT
		girl_swordman_surface, // pointer to source surface
		&SrcRect, // pointer to source RECT
		DDBLT_WAIT, 
		&ddbltfx))) // pointer to DDBLTFX holding info
	{}
	SrcRect.top=239+1;
	SrcRect.left=0+239+1;
	SrcRect.right=239+239+1;
	SrcRect.bottom=239+239+1;
	if (FAILED(girl_swordman.frames[3]->Blt(&DestRect, // pointer to dest RECT
		girl_swordman_surface, // pointer to source surface
		&SrcRect, // pointer to source RECT
		DDBLT_WAIT, 
		&ddbltfx))) // pointer to DDBLTFX holding info
	{}
	


	//if(!DeleteObject(bmp_info->hbmp)){
	//	MessageBox(hwnd,"删除位图对象失败","",MB_OK);
	//}
	//free(bmp_info);
#endif

girl_swordman.current_frame=0;

/*Scan_Image_Bitmap(&bitmapemy,enemy_1.frames[0],0,0);

Scan_Image_Bitmap(&bitmapemy,enemy_1.frames[1],1,0);
enemy_1.current_frame=0;*/






//为gamemenu扫描位图bitmapmenu
#if 0
gamemenu->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
// get video pointer to primary surfce
UCHAR *image_buffermn = (UCHAR *)ddsd.lpSurface;       

// test if memory is linear
if (ddsd.lPitch == 640)
   {
   // copy memory from double buffer to primary buffer
   memcpy((void *)image_buffermn, (void *)bitmapmenu.buffer, 640*480);
   } // end if
else
   { // non-linear

   // make copy of source and destination addresses
   UCHAR *dest_ptr = image_buffermn;
   UCHAR *src_ptr  = bitmapmenu.buffer;

   // memory is non-linear, copy line by line
   for (int y=0; y < 480; y++)
       {
       // copy line
       memcpy((void *)dest_ptr, (void *)src_ptr, 640);

       // advance pointers to next line
       dest_ptr+=ddsd.lPitch;
       src_ptr +=640;
       } // end for

   } // end else
if (FAILED(gamemenu->Unlock(NULL)))
   return(0);
#endif

// unload the bitmap file, we no longer need it
//Unload_Bitmap_File(&bitmap);
Unload_Bitmap_File(&bitmapsp);
//Unload_Bitmap_File(&bitmapmenu);
/*Unload_Bitmap_File(&bitmapemy);*/
//都使用bitmap8的调色板
//	if (FAILED(globallpddpal->SetEntries(0,0,256,bitmap.palette)))
//	   return(0);


//附加后备画面到主画面
ddsd.ddsCaps.dwCaps=DDSCAPS_BACKBUFFER;
if(FAILED(lpddsprimary->GetAttachedSurface(&ddsd.ddsCaps,&lpddsback))){
//echo error
	MessageBox(hwnd,"附加后备缓冲失败","信息",MB_OK);
}
//
//进行主画面操作lpSurface指针直接操作，要求先Lock后备缓冲画面
	UCHAR happy_bitmap[64] = {0,0,0,0,0,0,0,0,//定义一个笑脸，这种方法是直接操作主画面
	0,0,1,1,1,1,0,0,
	0,1,0,1,1,0,1,0,
	0,1,1,1,1,1,1,0,
	1,1,0,1,1,0,1,0,
	0,1,1,0,0,1,1,0,
	0,0,1,1,1,1,0,0,
	0,0,0,0,0,0,0,0};
	typedef struct HAPPY_FACE_TYP//定义笑脸数据结构，主要是控制他的坐标
{
int x,y; // position of happy face
int xv, yv; // velocity of happy face
} HAPPY_FACE, *HAPPY_FACE_PTR;

HAPPY_FACE hp[100];
// initialize all the happy faces
for (int face = 0; face < 100; face++)
    {
    // set random position
    hp[face].x = rand()%640;
    hp[face].y = rand()%480;    

    // set random velocity (-2,+2)
    hp[face].xv = -2 + rand()%5;
    hp[face].yv = -2 + rand()%5;

    } // end for face




//释放多余老接口
/*if (lpdd){
lpdd->Release();
lpdd=NULL;
}*/

// enter main event loop
//while(GetMessage(&msg,NULL,0,0))
// 	 { 
	 // translate any accelerator keys
//	 TranslateMessage(&msg);

	 // send the message to the window proc
//	 DispatchMessage(&msg);
//	 } // end while
HDC hdc=GetDC(hwnd);

#ifdef USE_GAMEDEV_TILE_2
// now set up the map
//Todo 注意还有1个tilesets画面要先装载位图，位图函数例子上是24位的！
//stage_num=1;
//if (!Init_Map(stage_num))//
//MessageBox(globalhwnd,"地图初始化失败","",MB_OK);
//return(FALSE);

// all set!
#endif
//MessageBox(hwnd,"准备进入主游戏循环","caution!!",MB_OK)
Build_Sin_Cos_Tables();
while(1){//主循环
	dwStart = GetTickCount();
	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
		if (msg.message == WM_QUIT)//检测是否退出
			break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

if (KEYDOWN(VK_ESCAPE))
   {
	if (pause!=1)
		PostMessage(hwnd,WM_CLOSE,0,0);
   //window_closed = 1;
   } // end if
if(pause!=1){


	//
	//7.6的例子2个矩形，主画面Blt指定区域到后备缓冲的区域，后备缓冲再切换到主画面。
	//我猜其实实际游戏里应该不是这么操作的！
	static RECT source_rect;
	source_rect.top=70;
	source_rect.bottom=120;
	source_rect.left=100;
	source_rect.right=200;
	//DDRAW_INIT_STRUCT(ddsd);
ddbltfx.dwFillColor = _RGB16BIT565(0,0,0);//给离屏画面填充颜色,先来个黑色好了
// make the blitter call清屏
if (FAILED(lpddsback->Blt(NULL, // pointer to dest RECT, NULL for whole thing
                          NULL, // pointer to source surface
                          NULL, // pointer to source RECT
                          DDBLT_COLORFILL | DDBLT_WAIT, 
                          // do a color fill and wait if you have to
                          &ddbltfx))) // pointer to DDBLTFX holding info
{
	MessageBox(hwnd,"以矩形填充后备缓冲失败","信息",MB_OK);
	return(0);
}
else{
	//MessageBox(hwnd,"以矩形填充后备缓冲OK","信息",MB_OK);
}
#if 0
ddbltfx.dwFillColor = _RGB16BIT565(0,120,250);//给离屏画面填充颜色
// make the blitter call
if (FAILED(mysprite->Blt(NULL, // pointer to dest RECT, NULL for whole thing
                          NULL, // pointer to source surface
                          NULL, // pointer to source RECT
                          DDBLT_COLORFILL | DDBLT_WAIT, 
                          // do a color fill and wait if you have to
                          &ddbltfx))) // pointer to DDBLTFX holding info
{
	MessageBox(hwnd,"以矩形填充离屏画面失败","信息",MB_OK);
	return(0);
}
else{
	if (ECHODEBUG==1)
		MessageBox(hwnd,"以矩形填充离屏画面OK","信息",MB_OK);
}
#endif


#if 0
		//锁住后备表面
	if(FAILED(lpddsback->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL))){
	//echo error
	}

	

	HDC xdc;//dx的兼容设备描述上下文
	if (FAILED(lpddsprimary->GetDC(&xdc))){
	//echo error
		MessageBox(hwnd,"在主画面获取设备描述符失败","信息",MB_OK);
	}
	SetTextColor(xdc,RGB(0,255,0));
	SetBkColor(xdc,RGB(0,0,0));
	SetBkMode(xdc,TRANSPARENT);
	if (DEBUG==1){
		char x[]="Directx加GDI混合使用的例子";
		TextOut(xdc,300,200,(char *)x,strlen(x));
	}
	else if (DEBUG>1 && DEBUG<6){
		char x[]="Directx的flip加GDI混合使用的例子";
		TextOut(xdc,300,200,(char *)x,strlen(x));
	}
	
	lpddsprimary->ReleaseDC(xdc);

	//用一种8位色填充画面区域
	

	// for now test if user is hitting ESC and send WM_CLOSE
if (KEYDOWN(VK_ESCAPE))
   {
   PostMessage(hwnd,WM_CLOSE,0,0);
   //window_closed = 1;
   } // end if
	
	//ddbltfx.dwFillColor=rand()%256;//选择一种颜色
	ddbltfx.dwFillColor = _RGB16BIT565(120, 120, 120);
	//ddbltfx.dwFillColor = 0;



	
mybltclip(hwnd,(USHORT *)ddsd.lpSurface,(int)ddsd.lPitch);//绘制16bit像素

if (DEBUG==4){
			for (int face=0; face < 100; face++){//绘制bitmap
		mybltclip2(lpddsprimary,
				   hwnd,
				   hp[face].x, 
				   hp[face].y,
				   8,
				   8,
				   happy_bitmap,
				   (UCHAR *)ddsd.lpSurface,
				   (int)ddsd.lPitch
				   );
		}


		// move all happy faces
		for (face=0; face < 100; face++)
			{
			// move
			hp[face].x+=hp[face].xv;
			hp[face].y+=hp[face].yv;

			// check for off screen, if so wrap
			if (hp[face].x > 640)
				 hp[face].x = -8;
			else
			if (hp[face].x < -8)
				hp[face].x = 640;

			if (hp[face].y > 480)
				 hp[face].y = -8;
			else
			if (hp[face].y < -8)
				hp[face].y = 480;

			} // end face
}





//###################################################################
if (DEBUG==5){
	//读birmap
	if(Load_Bitmap_File(hwnd,&bitmap,"bitmap8.bmp")){
	//MessageBox(hwnd,"读取图像OK","信息",MB_OK);
	}

	//读bitmap的调色板
	// load it's palette into directdraw
	if (FAILED(globallpddpal->SetEntries(0,0,256,bitmap.palette)))
	   return(0);

	//这就是所谓的读了
	// get video pointer to primary surfce
	UCHAR *primary_buffer = (UCHAR *)ddsd.lpSurface;       

	// test if memory is linear检查内存是不是线性的
	if (ddsd.lPitch == 640){
	//	MessageBox(hwnd,"内存是线性的","信息",MB_OK);
	   // copy memory from double buffer to primary buffer
	   memcpy((void *)primary_buffer, (void *)bitmap.buffer, 640*480);
	} // end if
	else
	   { 
	//	MessageBox(hwnd,"内存不是线性的","信息",MB_OK);
	   // make copy of source and destination addresses
	   UCHAR *dest_ptr = primary_buffer;
	   UCHAR *src_ptr  = bitmap.buffer;

	   // memory is non-linear, copy line by line
	   for (int y=0; y < 480; y++)
		   {
		   // 逐行拷贝
		   memcpy((void *)dest_ptr, (void *)src_ptr, 640);
		   // advance pointers to next line
		   dest_ptr+=ddsd.lPitch;
		   src_ptr +=640;
		   } // end for
	   } // end else
}

//###################################################################



	//解锁后备表面
	if(FAILED(lpddsback->Unlock(NULL))){
	//echo error
	}
	else{
		if(ECHODEBUG==1)
			MessageBox(hwnd,"解锁后备表面OK","信息",NULL);
	}
	if (DEBUG==5)
		Unload_Bitmap_File(&bitmap);

#endif

#if 0

	if (DEBUG==1){
		// make the blitter call
		if (FAILED(lpddsprimary->Blt(&des_rect, // pointer to dest RECT
			NULL, // pointer to source surface
			NULL, // pointer to source RECT
			DDBLT_COLORFILL | DDBLT_WAIT, 
			// do a color fill and wait if you have to
			&ddbltfx))) // pointer to DDBLTFX holding info
			
		{
			MessageBox(hwnd,"Blt失败","信息",NULL);
			return(0);
		}
	}//ebnd debug1
	else if (DEBUG==2){//这里例子不太明显，反正就是拷贝
		if (FAILED(lpddsprimary->Blt(&des_rect, // pointer to dest RECT
			lpddsback, // pointer to source surface
			&source_rect, // pointer to source RECT
			DDBLT_WAIT, //这里无法使用DDBLT_COLORFILL了？
			// do a color fill and wait if you have to
			&ddbltfx))) // pointer to DDBLTFX holding info
			
		{
			MessageBox(hwnd,"Blt失败","信息",NULL);
			return(0);
		}
	}//end debug2
	else if (DEBUG==3){//7.6和7.7结合，看效果，发现如果要出现填色矩形，只有把Blt
		//里有DDBLT_COLORFILL的放到最后，否则倒过来会被新的无DDBLT_COLORFILL的BLT相同区域覆盖掉的！
		if (FAILED(lpddsprimary->Blt(&des_rect, // pointer to dest RECT
			lpddsback, // pointer to source surface
			&source_rect, // pointer to source RECT
			DDBLT_WAIT, //这里无法使用DDBLT_COLORFILL了？
			// do a color fill and wait if you have to
			&ddbltfx))) // pointer to DDBLTFX holding info
			
		{
			MessageBox(hwnd,"Blt失败","信息",NULL);
			return(0);
		}//end 7.7
		if (FAILED(lpddsprimary->Blt(&des_rect, // pointer to dest RECT
			NULL, // pointer to source surface
			NULL, // pointer to source RECT
			DDBLT_COLORFILL | DDBLT_WAIT, 
			// do a color fill and wait if you have to
			&ddbltfx))) // pointer to DDBLTFX holding info
			
		{
			MessageBox(hwnd,"Blt失败","信息",NULL);
			return(0);
		}	//end7.6
	}//end debug3
	
	if(DEBUG==4){
		//里有DDBLT_COLORFILL的放到最后，否则倒过来会被新的无DDBLT_COLORFILL的BLT相同区域覆盖掉的！
		if (FAILED(lpddsprimary->Blt(&des_rect, // pointer to dest RECT
			lpddsback, // pointer to source surface
			&source_rect, // pointer to source RECT
			DDBLT_WAIT, //这里无法使用DDBLT_COLORFILL了？
			// do a color fill and wait if you have to
			&ddbltfx))) // pointer to DDBLTFX holding info
			
		{
			MessageBox(hwnd,"Blt失败","信息",NULL);
			return(0);
		}//end 7.7
		if (FAILED(lpddsprimary->Blt(&des_rect, // pointer to dest RECT
			NULL, // pointer to source surface
			NULL, // pointer to source RECT
			DDBLT_COLORFILL | DDBLT_WAIT, 
			// do a color fill and wait if you have to
			&ddbltfx))) // pointer to DDBLTFX holding info
			
		{
			MessageBox(hwnd,"Blt失败","信息",NULL);
			return(0);
		}	//end7.6
		
		
		
		//DDraw_Draw_Surface(lpddsbackground,0,0, 640,480, lpddsback,0);    
		//////////////////////////////////////////////////////////////////////////
		//把离屏画面Blt到后备表面
		RECT srcrect,desrect;
		srcrect.left=srcrect.top=0;
		srcrect.right=240-1;
		srcrect.bottom=480-1;
		desrect.left=0;
		desrect.top=0;
		desrect.right=desrect.left+srcrect.right-1;
		desrect.bottom=desrect.top+srcrect.bottom-1;
		if(FAILED(lpddsback->Blt(&desrect,mysprite,&srcrect,(DDBLT_WAIT),NULL))){
			MessageBox(hwnd,"mysprite离屏画面Blt到后备缓冲失败","信息",MB_OK);
		}
		//////////////////////////////////////////////////////////////////////////
		// copy background to back buffer
		
		//DDraw_Draw_Surface(mysprite,rand()%2,0, 240,480, lpddsback,0);    
		//
		// now create and attach clipper
		RECT rect_list[3] = {{10,10,50,50},
							 {20,20,60,70},
							 {300,300, 500, 450}};
		
		if (FAILED(lpddclipper = DDraw_Attach_Clipper(lpddsprimary,3,rect_list)))
			return(0);
		//end7.9
	}//end debug4
#endif

	if(DEBUG==6){//现在从这里开始
static int singleton_num;//以后改成单件模式
//先扫描开始的制作版权画面
///
//为lpddsbackground扫描位图bitmap8,这部分应该放在游戏初始化！！但是我不知道中途切换的时候是怎么处理，
//理论上肯定是在game_main里的
if (COPYRIGHT==0){
#ifdef USE_GAMEDEV_TILE_2
	// now create and attach clipper
	
	if (singleton_num<=1){
	//	MessageBox(hwnd,"应用了一次裁切器","debug",MB_OK);
	 RECT rect_list[1] = {{0,0,640,480}};
	 if (FAILED(lpddclipper = DDraw_Attach_Clipper(lpddsback,1,rect_list)))
		 return(0);
	// if (FAILED(lpddclipper = DDraw_Attach_Clipper(lpddswork,1,rect_list)))
	//	 return(0);
	 singleton_num++;
	}
#endif

#if 0
	lpddsbackground->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	// get video pointer to primary surfce
	UCHAR *image_buffer = (UCHAR *)ddsd.lpSurface;       

	// test if memory is linear
	if (ddsd.lPitch == 640)
	   {
	   // copy memory from double buffer to primary buffer
	   memcpy((void *)image_buffer, (void *)bitmapcopyright.buffer, 640*480);
	   } // end if
	else
	   { // non-linear

	   // make copy of source and destination addresses
	   UCHAR *dest_ptr = image_buffer;
	   UCHAR *src_ptr  = bitmapcopyright.buffer;

	   // memory is non-linear, copy line by line
	   for (int y=0; y < 480; y++)
		   {
		   // copy line
		   memcpy((void *)dest_ptr, (void *)src_ptr, 640);

		   // advance pointers to next line
		   dest_ptr+=ddsd.lPitch;
		   src_ptr +=640;
		   } // end for

	   } // end else

	// now unlock the primary surface
	if (FAILED(lpddsbackground->Unlock(NULL)))
	   return(0);
#endif
}
#if 1//性能瓶颈,发现其实没有


if (COPYRIGHT==1 && START_SELECT==-1){//如果START屏幕出现过了，就加载另外一个背景
	DSound_Stop_All_Sounds();
	DSound_Play(sound_ids[0]);
	START_SELECT=0;//赋值，代表已经开始菜单加载了
#if 0//注释下面的可能的性能问题代码,用之后的代替
	lpddsbackground->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
	// get video pointer to primary surfce
	UCHAR *image_buffer = (UCHAR *)ddsd.lpSurface;       
	
	// test if memory is linear
	if (ddsd.lPitch == 640)
	{
		// copy memory from double buffer to primary buffer
		memcpy((void *)image_buffer, (void *)bitmap.buffer, 640*480);
	} // end if
	else
	{ // non-linear
		Unload_Bitmap_File(&bitmapcopyright);//进入菜单选择画面，删除之前的制作公司版权画面
		// make copy of source and destination addresses
		UCHAR *dest_ptr = image_buffer;
		UCHAR *src_ptr  = bitmap.buffer;
		
		// memory is non-linear, copy line by line
		for (int y=0; y < 480; y++)
		{
			// copy line
			memcpy((void *)dest_ptr, (void *)src_ptr, 640);
			
			// advance pointers to next line
			dest_ptr+=ddsd.lPitch;
			src_ptr +=640;
		} // end for
		
	} // end else
	
	// now unlock the primary surface
	if (FAILED(lpddsbackground->Unlock(NULL)))
		return(0);
#endif//注释上面的可能的性能问题代码,用下面的代替，其实问题不在这里，排除了
	lpddsbackground=DDLoadBitmap(lpdd,"tilemap.bmp");
}//end if (COPYRIGHT==1 && START_SELECT==-1)


#endif
///
#ifdef USE_GAMEDEV_TILE_1
//draw_tiles(lpddsbackground,lpddsback);
#endif
#if 1
#if 0
#ifdef USE_GAMEDEV_TILE_2//检测用户输入，应该放在主消息循环里，而不是消息回调函数里！
	//To do:把上面的检测输入放到这里！
	// check user input
	if (KEYSTATE(VK_UP) && (mapdata.yCamera > 4))
		mapdata.yCamera -= 5;
	if (KEYSTATE(VK_DOWN) && (mapdata.yCamera < (mapdata.yMaxCamera - 4)))
		mapdata.yCamera += 5;
	if (KEYSTATE(VK_LEFT) && (mapdata.xCamera > 4))
		mapdata.xCamera -= 5;
	if (KEYSTATE(VK_RIGHT) && (mapdata.xCamera < (mapdata.xMaxCamera - 4)))
		mapdata.xCamera += 5;
	// draw and show frame
	Render_Map(lpddsTileset,lpddswork);
#endif
	//动画的部分
	for (int x=0; x<NUM_TILES; x++)
	{
		// update each frame counter
		if (lptile[x]->nAnimSpeed > 0)
		{
			// advance frame counter
			lptile[x]->uiFrames++;			
			// check if we're finished
			if (lptile[x]->uiFrames == (UINT)lptile[x]->nAnimSpeed)
			{
				lptile[x]->uiFrames = 0;
				lptile[x] = lptile[x]->lpNext;
			}
		}
	}
#endif
#endif
//DDraw_Draw_Surface(lpddsbackground,0,0, 640,480, lpddsback,0);    

	//////////////////////////////////////////////////////////////////////////
	//把离屏画面Blt到后备表面,其实就是书上的DDRAW_DRAW_SURFACE
	static RECT srcrect,desrect;//一个简陋的“动画序列”实现
	if (mytransform==0){
	srcrect.left=0;
	srcrect.top=0;
	srcrect.right=240-1;
	srcrect.bottom=240-1;
	mytransform++;
	}
	else if(mytransform==1*5){
	srcrect.left=0;
	srcrect.top=240;
	srcrect.right=240-1;
	srcrect.bottom=480-1;
	mytransform++;
	}
	else if(mytransform==2*5){
	srcrect.left=240;
	srcrect.top=0;
	srcrect.right=480-1;
	srcrect.bottom=240-1;
	mytransform++;
	}
	else if(mytransform==3*5){
	srcrect.left=240;
	srcrect.top=240;
	srcrect.right=480-1;
	srcrect.bottom=480-1;
	mytransform=0;
	}
	else{
	mytransform++;
	}
	//BLT目的地RECT
	desrect.left=190;
	desrect.top=60;
	desrect.right=0+430-1;
	desrect.bottom=0+300-1;
	//加KEYSRC来透明
	if(START_SELECT!=-1){//START_SELECT出现之后就有问题
		//愚笨的动画方法,上方的
		//if(FAILED(lpddswork->Blt(&desrect,mysprite,&srcrect,(DDBLT_WAIT|DDBLT_KEYSRC ),NULL))){
		//	MessageBox(hwnd,"mysprite离屏画面Blt到后备缓冲失败","信息",MB_OK);
		//}
		//加强版的绘制动画精灵，没有问题
		if (++girl_swordman.counter >= (8 - girl_swordman.velocity))
        {
			// reset counter
			girl_swordman.counter = 0;
			
			// advance to next frame
			if (++girl_swordman.current_frame > 3)
				girl_swordman.current_frame = 0;
			
        } // end if
		//封装后的更新操作
		player->UpdateFrame();

		//move it!
		//if (girl_swordman.x<400)
		//	girl_swordman.x++;		
#ifdef USE_GAMEDEV_2
		if (KEYSTATE(VK_UP))
			girl_swordman.y-=2;
		if (KEYSTATE(VK_DOWN))
			girl_swordman.y+=2;
		if (KEYSTATE(VK_LEFT))
			girl_swordman.x-=2;
		if (KEYSTATE(VK_RIGHT))
			girl_swordman.x+=2;
#endif


	}
	



//////////////主菜单绘制//////////////////////////////////////////////
#if 1//先绘制底色
	RECT srcrect_mn,desrect_mn;
	/*
	srcrect_mn.left=0;
		srcrect_mn.top=0;
		srcrect_mn.right=200-1;
		srcrect_mn.bottom=160-1;*/
	

	srcrect_mn.left=0;
	srcrect_mn.top=166;
	srcrect_mn.right=200-1;
	srcrect_mn.bottom=326-1;
	//Blt目的地矩形区域
	desrect_mn.left=210;
	desrect_mn.top=240;
	desrect_mn.right=0+410-1;
	desrect_mn.bottom=0+400-1;
	
	if(START_SELECT!=-1){//如果进入了主菜单
		//if(FAILED(lpddsback->Blt(&desrect_mn,gamemenu,&srcrect_mn,(DDBLT_WAIT|DDBLT_KEYSRC ),NULL))){
		//	MessageBox(hwnd,"mysprite离屏画面Blt到后备缓冲失败","信息",MB_OK);
		//}
	}
#endif
//如果没选择那么默认是上面一个菜单文字{旧的回忆}是深色的。
#if 1
	if (START_SELECT==0){
		srcrect_mn.left=0;
		srcrect_mn.top=0;
		srcrect_mn.right=200-1;
		srcrect_mn.bottom=80-1;
		//Blt目的地矩形区域
		desrect_mn.left=210;
		desrect_mn.top=251;
		desrect_mn.right=0+410-1;
		desrect_mn.bottom=0+331-1;
		//if(FAILED(lpddsback->Blt(&desrect_mn,gamemenu,&srcrect_mn,(DDBLT_WAIT|DDBLT_KEYSRC ),NULL))){
		//	MessageBox(hwnd,"mysprite离屏画面Blt到后备缓冲失败","信息",MB_OK);
		//}
	}
#endif
	//如果是新的游戏
#if 1
	if (START_SELECT==1){
		srcrect_mn.left=0;
		srcrect_mn.top=100;
		srcrect_mn.right=200-1;
		srcrect_mn.bottom=160-1;
		//Blt目的地矩形区域
		desrect_mn.left=210;
		desrect_mn.top=351;
		desrect_mn.right=0+410-1;
		desrect_mn.bottom=0+411-1;
		//if(FAILED(lpddsback->Blt(&desrect_mn,gamemenu,&srcrect_mn,(DDBLT_WAIT|DDBLT_KEYSRC ),NULL))){
		//	MessageBox(hwnd,"mysprite离屏画面Blt到后备缓冲失败","信息",MB_OK);
		//}
	}
#endif
//////////////////////////////////////////////////////////////////////////


	//锁住离屏画面
	if(FAILED(mysprite->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL))){
	//echo error
		MessageBox(hwnd,"锁定离屏画面失败","信息",MB_OK);
	}
	else{
	//MessageBox(hwnd,"锁定离屏画面OK","信息",MB_OK);
	}
	//这里做些离屏画面Blt,GDI等等
	HDC xdc0;
	if (FAILED(mysprite->GetDC(&xdc0))){
		MessageBox(hwnd,"在离屏画面获取设备描述符失败","信息",MB_OK);
	}
	SetTextColor(xdc0,1);
	SetBkColor(xdc0,0);
	SetBkMode(xdc0,TRANSPARENT);
	//char x0[]="离屏画面里GDI,BLT到后备缓冲！";
	char x0[]="离屏画面上的另外一个字";
	TextOut(xdc0,20,10,(char *)buf2,strlen(buf2));
	mysprite->ReleaseDC(xdc0);
	if(FAILED(mysprite->Unlock(NULL))){
		//echo error
	}
	else{
		if(ECHODEBUG==1)
			MessageBox(hwnd,"解锁离屏画面OK","信息",NULL);
	}
	//
	}//end mygame
#if 0
		rcRect.top=0;
		rcRect.left=0;
		rcRect.right=239;
		rcRect.bottom=239;	
//AlphaBlend16(lpddswork,rcRect,girl_swordman.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],rcRect,5555);
		BltAlphaFastMMX( lpddswork, girl_swordman.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]], 0, 0, NULL, 1 );
#endif

		rcRect.top=0;
		rcRect.left=0;
		rcRect.right=640;
		rcRect.bottom=480;	
/*	if (FAILED(lpddsback->Blt(//工作画面BLT到后备缓冲
		&rcRect,
		lpddswork,
		&rcRect,
		(DDBLT_WAIT|DDBLT_KEYSRC ),
		NULL
		))){
	MessageBox(hwnd,"","",MB_OK);
	}*/
	lpddsback->BltFast( 0, 0, lpddswork, NULL, DDBLTFAST_WAIT );//优化！

#if 1
#ifdef USE_GAMEDEV_TILE_2//检测用户输入，应该放在主消息循环里，而不是消息回调函数里！
		//To do:把上面的检测输入放到这里！
		// check user input
/*		if (KEYSTATE(VK_UP) && (mapdata.yCamera > 4))
			mapdata.yCamera -= 5;
		if (KEYSTATE(VK_DOWN) && (mapdata.yCamera < (mapdata.yMaxCamera - 4)))
			mapdata.yCamera += 5;
		if (KEYSTATE(VK_LEFT) && (mapdata.xCamera > 4))
			mapdata.xCamera -= 5;
		if (KEYSTATE(VK_RIGHT) && (mapdata.xCamera < (mapdata.xMaxCamera - 4)))
			mapdata.xCamera += 5;*/
		// draw and show frame
		Render_Map(lpddsTileset,lpddsback);
		//动画的部分
		for (int x=0; x<NUM_TILES; x++)
		{
			// update each frame counter
			if (lptile[x]->nAnimSpeed > 0)
			{
				// advance frame counter
				lptile[x]->uiFrames++;			
				// check if we're finished
				if (lptile[x]->uiFrames == (UINT)lptile[x]->nAnimSpeed)
				{
					lptile[x]->uiFrames = 0;
					lptile[x] = lptile[x]->lpNext;
				}
			}
		}
#endif
#endif

//空战游戏自己卷屏
		/*if (mapdata.yCamera<20)
mapdata.yCamera =800;*/
		if ((mapdata.yCamera > 4)){
			//如果boss1出现了，如果还没被消灭的话，不滚屏，否则继续滚
			//if(!(mapdata.yCamera<2000 && boss1!=NULL)){
			//if(boss1!=NULL){
				if(COPYRIGHT!=0)
				mapdata.yCamera -= 3;
				if (stage_num==1 && mapdata.yCamera<=3000)//靠近boss的背景再快一点
				{
					mapdata.yCamera -= 4;
				}
				if (stage_num==0 && mapdata.yCamera<=2000)//靠近boss的背景再快一点
				{
					mapdata.yCamera -= 2;
				}
			//}
		}
		// Display the frame rate.
		current_fps=ShowFrameRate(max_fps);
		if (max_fps<current_fps) 
			max_fps=current_fps;

//敌机链表垃圾回收
		enemy_iter=enemy_list.begin();
		while(enemy_iter!=enemy_list.end()){
			if((*enemy_iter)->GetPos().x>1000 || (*enemy_iter)->GetPos().x<-1000
				||(*enemy_iter)->GetPos().y>1000 || (*enemy_iter)->GetPos().y<-1000
				){
				temp_enemy_iter=enemy_iter;
				++enemy_iter;
				delete(*temp_enemy_iter);
				enemy_list.erase(temp_enemy_iter);
			}
			else
				++enemy_iter;
		}

		//起飞和飞离和隐身简单有限状态机
		if (player->GetState()==starting)
		{
			if (player->GoStarting())
			{
				player->SetState(flikering);
			}
			
		}
		if (player->GetState()==exitareaing)
		{
			if (player->GoLeaving())
			{

				player->SetState(normaling);
				POINT pt_startpoint;
				pt_startpoint.x=300;
				pt_startpoint.y=480;
				//设置主角初始位置
				player->SetPos(pt_startpoint);
				stage_num+=1;
			}
			
		}
		if (player->GetState()==flikering)
		{
			if (player->GoFlikering())
			{
				player->SetState(normaling);
			}
			
		}


//测试一下能量
#if 0
		if(mapdata.yCamera<13000){
			static int suplly_power=0;
			if (suplly_power==0)
			{
			//	MessageBox(globalhwnd,"","",MB_OK);
				floatpoint pt;
				pt.x=300;
				pt.y=150;
#if 1
				sprite_list2.push_back(new cls_sprite());
				sprite_list2.back()->Bind(sfobj);
				sprite_list2.back()->sprite_surface_id=power_sfid;
				sprite_list2.back()->SetAnimateSequence("0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1");
				sprite_list2.back()->SetAnimateData("74|31,74|31,74|31,74|31,74|31,74|31,74|31,74|31,74|31,74|31,74|31,74|31");
				sprite_list2.back()->SetSeqNum(12);
				sprite_list2.back()->current_frame=0;
				//改进过的速度，改成最大是60
				sprite_list2.back()->SetVelocity(55);
				sprite_list2.back()->SetNspeed(7);
				sprite_list2.back()->SetPos(pt);
				sprite_list2.back()->sprite_type=1;
				sprite_list2.back()->SetAngle(315);
#endif
				suplly_power=1;
			}
		}
#endif
// if (sprite_list.size()!=0)
// {
// 	sprite_list.back()->UpdateFrame();
// 	//bossspark->UpdateFrame();
// 	//bossspark->DrawMe(sfobj,bossspark_sfid,0,0);
// 	sprite_list.back()->DrawMe(sfobj,sprite_list.back()->sprite_surface_id,0,0);
// }
/*	sprite_list_iter=sprite_list.begin();
	while (sprite_list_iter!=sprite_list.end())
	{
		(*sprite_list_iter)->UpdateFrame();
		if((*sprite_list_iter)->sprite_type==1){
			(*sprite_list_iter)->DrawMe(sfobj,(*sprite_list_iter)->sprite_surface_id,0,0);
		}
		++sprite_list_iter;
	}*/

//刷怪
#if 0
//////////////////////////////////////////////////////////////////////////
		if(mapdata.yCamera<10800){
			
			static test_tank=0;
			if(test_tank==0){
				//alertme("差不多到了");
// 				for (int i=0;i<=8;i++){
// 					enemy_list.push_back((cls_enemy *)new cls_enemy());
// 					pt_startpoint.x=rand()%640-rand()%14;
// 					pt_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
// 					enemy_list.back()->SetPos(ptf_startpoint);
// 					enemy_list.back()->sprite_surface_id=enemy0_sfid;
// 					enemy_list.back()->SetAnimateSequence("0,1,2,3,4,5");
// 					enemy_list.back()->SetAnimateData("30|29,30|29,30|29,30|29,30|29,30|29");
// 					enemy_list.back()->current_frame=(rand()%1);
// 					enemy_list.back()->SetVelocity(5);
// 				}
				for (int i=0;i<=6;i++){
					enemy_list.push_back((cls_enemy *)new cls_tank());
					ptf_startpoint.x=rand()%640-rand()%14;
					ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
					enemy_list.back()->SetPos(ptf_startpoint);
					enemy_list.back()->sprite_surface_id=tank_sfid;
					enemy_list.back()->SetAnimateSequence("0,1,2,3,4,5,6,7");
					enemy_list.back()->SetAnimateData("34|35,34|35,34|35,34|35,34|35,34|35,34|35,34|35");
					enemy_list.back()->current_frame=(rand()%1);
					enemy_list.back()->SetVelocity(rand()%4);
					//enemy_list.back()->SetFireState(enemy_normalattack);
					enemy_list.back()->SetHP(15);
					//enemy_list.back()->use_fire_fsm=1;
					//dynamic_cast<cls_tank *>(enemy_list.back())->AimTarget(player,jiajiao);
				}
				//enemy_iter=enemy_list.begin();
				//while(enemy_iter!=enemy_list.end()){

				//	++enemy_iter;
				//}
				test_tank=1;
			}
		}
#endif
//////////////////////////////////////////////////////////////////////////
#if 0
		if(mapdata.yCamera<9500){
			static int the_2nd_enemy=0;
			if (the_2nd_enemy==0)
			{
				//初始化第一批敌机，共6个
				for (int i=0;i<=8;i++){
					enemy_list.push_back((cls_enemy *)new cls_enemy());
					//}
					//pt_startpoint.x=130;
					//pt_startpoint.y=230;
					
					//list<cls_enemy * >::iterator   temp_enemy_iter;//用来替换删除的临时迭代器
					//enemy_iter=enemy_list.begin();
					//while(enemy_iter!=enemy_list.end()){
					floatpoint ptf_startpoint;
					ptf_startpoint.x=rand()%640;
					ptf_startpoint.y=rand()%480-480;
					enemy_list.back()->SetPos(ptf_startpoint);
					enemy_list.back()->sprite_surface_id=enemy3_sfid;
					enemy_list.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
					enemy_list.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
					// 	(*enemy_iter)->sprite_surface_id=enemy4_sfid;
					// 	(*enemy_iter)->SetAnimateSequence("0,1");
					// 	(*enemy_iter)->SetAnimateData("167|152,167|152");
					enemy_list.back()->current_frame=(rand()%1);
					enemy_list.back()->SetVelocity(4);
					enemy_list.back()->SetAiState(1);
					enemy_list.back()->iFireInputDelay=23;
					if (i==4)//带一个奖励
					{
						enemy_list.back()->sprite_surface_id=enemy3_red_sfid;
						enum bonus bs=power;//加enum可以适应c编译器！
						enemy_list.back()->SetBonus(bs);
						enemy_list.back()->SetHP(9);
						
					}
					//	++enemy_iter;
				}
				the_2nd_enemy=1;
			}
			
		}
#endif	
		int lv;
		if (option_state_which==option_state_eazy_lianfa||option_state_which==option_state_eazy_shoudong)
		{	
			lv=0;
		}
		else if (option_state_which==option_state_normal_lianfa||option_state_which==option_state_normal_shoudong)
		{	
			lv=1;
		}
		else if (option_state_which==option_state_hard_lianfa||option_state_which==option_state_hard_shoudong)
		{	
			lv=2;
		}
		if (stage_num==0)
		{
				enemygroup_1_1(mapdata,7000,enemy_list,enemy3_sfid,enemy3_red_sfid,lv);
				enemygroup_1_2t(mapdata,4000,enemy_list,tank_sfid,enemy3_red_sfid,lv);
				enemygroup_1_3t(mapdata,10000,enemy_list,tank_sfid,enemy3_red_sfid,lv);
				enemygroup_1_3(mapdata,10800,enemy_list,enemy4_sfid,0,lv);
				enemygroup_1_2(mapdata,10000,enemy_list,enemy3_sfid,enemy3_red_sfid,lv);
				enemygroup_1_4(mapdata,8500,enemy_list,enemy3_sfid,enemy3_red_sfid,lv);
				enemygroup_1_5(mapdata,7500,enemy_list,enemy3_sfid,enemy3_red_sfid,lv);
				enemygroup_1_6(mapdata,7000,enemy_list,enemy3_sfid,enemy3_red_sfid,lv);
				enemygroup_1_7(mapdata,6700,enemy_list,enemy3_sfid,enemy3_red_sfid,lv);
				enemygroup_1_8(mapdata,5800,enemy_list,enemy3_sfid,enemy3_red_sfid,lv);
				enemygroup_1_9(mapdata,5200,enemy_list,enemy3_sfid,enemy3_red_sfid,lv);
				enemygroup_1_10(mapdata,4900,enemy_list,enemy4_sfid,lv);

		}
		else if (stage_num==1)
		{
			enemygroup_2_1(mapdata,11000,enemy_list,enemy2_sfid,enemy3_red_sfid,lv);
			enemygroup_2_2(mapdata,10500,enemy_list,enemy2_sfid,enemy3_red_sfid,lv);
			enemygroup_2_3(mapdata,10000,enemy_list,enemy2_sfid,enemy3_red_sfid,lv);
			enemygroup_2_4(mapdata,9500,enemy_list,enemy2_sfid,enemy3_red_sfid,lv);
			enemygroup_2_4b(mapdata,9200,enemy_list,enemy2_sfid,enemy3_red_sfid,lv);
			enemygroup_2_4c(mapdata,8900,enemy_list,enemy2_sfid,enemy3_red_sfid,lv);
			enemygroup_2_5(mapdata,8500,enemy_list,enemy2_sfid,enemy3_red_sfid,lv);
			enemygroup_2_6(mapdata,8000,enemy_list,enemy2_sfid,enemy3_red_sfid,lv);
			enemygroup_2_7(mapdata,7600,enemy_list,enemy1_sfid,enemy3_red_sfid,lv);
			enemygroup_2_8(mapdata,7200,enemy_list,enemy1_sfid,enemy3_red_sfid,lv);
			enemygroup_2_9(mapdata,6800,enemy_list,enemy2_sfid,enemy3_red_sfid,lv);
			enemygroup_2_10(mapdata,6300,enemy_list,enemy1_sfid,enemy3_red_sfid,lv);
			enemygroup_2_11(mapdata,6000,enemy_list,enemy2_sfid,enemy3_red_sfid,lv);
			enemygroup_2_12(mapdata,5700,enemy_list,enemy1_sfid,enemy3_red_sfid,lv);
			enemygroup_2_13(mapdata,5400,enemy_list,enemy2_sfid,enemy3_red_sfid,lv);
			enemygroup_2_14(mapdata,5200,enemy_list,enemy1_sfid,enemy3_red_sfid,lv);
			enemygroup_2_15(mapdata,4700,enemy_list,enemy2_sfid,enemy3_red_sfid,lv);
			enemygroup_t1(mapdata,6000,enemy_list,tank_sfid,enemy3_red_sfid,lv);
			enemygroup_t4(mapdata,4200,enemy_list,tank_sfid,enemy3_red_sfid,lv);
			enemygroup_t3(mapdata,11500,enemy_list,tank_sfid,enemy3_red_sfid,lv);
			enemygroup_t2(mapdata,8000,enemy_list,tank_sfid,enemy3_red_sfid,lv);
			enemygroup_t5(mapdata,3900,enemy_list,tank_sfid,enemy3_red_sfid,lv);

		}

#if 0
//第二b批
		if(mapdata.yCamera<7500){
			
			static the_2bnd_enemy=0;
			if(the_2bnd_enemy==0){
				//alertme("差不多到了");
				MessageBeep(MB_ICONASTERISK);
				for (int i=0;i<=8;i++){
					enemy_list.push_back((cls_enemy *)new cls_enemy());
					pt_startpoint.x=rand()%640-rand()%14;
					pt_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
					enemy_list.back()->SetPos(ptf_startpoint);
					enemy_list.back()->sprite_surface_id=enemy0_sfid;
					enemy_list.back()->SetAnimateSequence("0,1,2,3,4,5");
					enemy_list.back()->SetAnimateData("30|29,30|29,30|29,30|29,30|29,30|29");
					enemy_list.back()->current_frame=(rand()%1);
					enemy_list.back()->SetVelocity(5);
				}
				//enemy_iter=enemy_list.begin();
				//while(enemy_iter!=enemy_list.end()){

				//	++enemy_iter;
				//}
				the_2bnd_enemy=1;
			}
		}
#endif
#if 0
//第三批
		if(mapdata.yCamera<7000){
			static the_3rd0_enemy=0;
			if(the_3rd0_enemy==0){
		//	MessageBeep(MB_DEFBUTTON1);
				for (int i=0;i<=2;i++){
					enemy_list.push_back((cls_enemy *)new cls_enemy());
				
				//enemy_iter=enemy_list.begin();
				//while(enemy_iter!=enemy_list.end()){
					ptf_startpoint.x=rand()%640-rand()%14;
					ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
					enemy_list.back()->SetPos(ptf_startpoint);
					enemy_list.back()->sprite_surface_id=enemy1_sfid;
					enemy_list.back()->SetAnimateSequence("0,0,0,0,0,0");
					enemy_list.back()->SetAnimateData("36|25,36|25,36|25,36|25,36|25,36|25");
					enemy_list.back()->current_frame=(rand()%1);
					enemy_list.back()->SetVelocity(rand()%4);
				//	++enemy_iter;
				}
				the_3rd0_enemy=1;
			//}
				}
		}
#endif
//////////////////////循环刷怪////////////////////////////////////
		//
#if 0
		if(mapdata.yCamera%500==0 && mapdata.yCamera<10000){
			
				for (int i=0;i<=3;i++){
					enemy_list.push_back((cls_enemy *)new cls_enemy());
				}
				enemy_iter=enemy_list.begin();
				while(enemy_iter!=enemy_list.end()){
					ptf_startpoint.x=rand()%640-rand()%14;
					ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
					(*enemy_iter)->SetPos(ptf_startpoint);
					(*enemy_iter)->sprite_surface_id=enemy4_sfid;
					(*enemy_iter)->SetAnimateSequence("0,1,2,3,4,5");
					(*enemy_iter)->SetAnimateData("100|112,100|112,100|112,100|112,100|112,100|112");
					//(*enemy_iter)->sprite_surface_id=smallboss_sfid;
					//(*enemy_iter)->SetAnimateSequence("0");
					//(*enemy_iter)->SetAnimateData("164|152");
					(*enemy_iter)->current_frame=(rand()%1);
					(*enemy_iter)->SetVelocity(7);
					(*enemy_iter)->SetHP(11);
					(*enemy_iter)->b_type=1;
					++enemy_iter;//Todo这些对象什么时候被销毁
				}

		//	
		}
#endif
//////////////////////////////////////////////////////////////////////////
#if 0
//第四批
		if(mapdata.yCamera<6700){
			
			static the_4th_enemy=0;
			if(the_4th_enemy==0){
				//alertme("快到了");
// 				for (int x=0;x<enemy_list.size();x++)
// 				{
// 					MessageBeep(MB_ICONASTERISK);
// 					Sleep(300);
// 				}
				
				for (int i=0;i<=12;i++){
					enemy_list.push_back((cls_enemy *)new cls_enemy());
					ptf_startpoint.x=rand()%640-rand()%14;
					ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
					enemy_list.back()->SetPos(ptf_startpoint);
					enemy_list.back()->sprite_surface_id=enemy2_sfid;
					enemy_list.back()->SetAnimateSequence("0,1,2,3,4,5");
					enemy_list.back()->SetAnimateData("44|34,44|34,44|34,44|34,44|34,44|34");
					enemy_list.back()->current_frame=(rand()%1);
					enemy_list.back()->SetVelocity(rand()%4);
				}
// 				enemy_iter=enemy_list.begin();
// 				while(enemy_iter!=enemy_list.end()){
// 					ptf_startpoint.x=rand()%640-rand()%14;
// 					ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
// 					(*enemy_iter)->SetPos(ptf_startpoint);
// 					(*enemy_iter)->sprite_surface_id=enemy2_sfid;
// 					(*enemy_iter)->SetAnimateSequence("0,1,2,3,4,5");
// 					(*enemy_iter)->SetAnimateData("44|34,44|34,44|34,44|34,44|34,44|34");
// 					(*enemy_iter)->current_frame=(rand()%1);
// 					(*enemy_iter)->SetVelocity(rand()%4);
// 					++enemy_iter;
// 				}
				the_4th_enemy=1;
			}
		}
#endif

		//char buf[20];
		//sprintf(buf,"%d",GetRGBMode(lpddsback));
		//MessageBox(hwnd,buf,"",MB_OK);
		//RGB565模式
		
		//无关的就不要放到工作画面里，否则性能下降严重！
		//一个老函数,支持任何位数的BLT！推荐
		/*DDraw_Draw_Surface(sfobj->sprite_surface[2].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]], 
			girl_swordman.x, girl_swordman.y,
			sfobj->sprite_surface[2].sprite.theframe_width,sfobj->sprite_surface[2].sprite.theframe_height,
			lpddsback,1);*/
	/*	DDraw_Draw_Surface(sfobj->sprite_surface[6].sprite.frames[0], 
			girl_swordman.x+20, girl_swordman.y+70,
			sfobj->sprite_surface[6].sprite.theframe_width,sfobj->sprite_surface[6].sprite.theframe_height,
			lpddsback,1);*/
#if 0
//第五批
		static the_5th_enemy=0;
				if(mapdata.yCamera<6670){
			
			
			if(the_5th_enemy==0){
// 				for (int x=0;x<enemy_list.size();x++)
// 				{
// 					MessageBeep(MB_DEFBUTTON1);
// 					Sleep(300);
// 				}
				//alertme("差不多到了");
				for (int i=0;i<=29;i++){
					enemy_list.push_back((cls_enemy *)new cls_enemy());
					ptf_startpoint.x=rand()%640-rand()%14;
					ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
					enemy_list.back()->SetPos(ptf_startpoint);
					enemy_list.back()->sprite_surface_id=enemy0_sfid;
					enemy_list.back()->SetAnimateSequence("0,1,2,3,4,5");
					enemy_list.back()->SetAnimateData("30|29,30|29,30|29,30|29,30|29,30|29");
					enemy_list.back()->current_frame=(rand()%1);
					enemy_list.back()->SetVelocity(5);
				}
// 				enemy_iter=enemy_list.begin();
// 				while(enemy_iter!=enemy_list.end()){
// 					ptf_startpoint.x=rand()%640-rand()%14;
// 					ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
// 					(*enemy_iter)->SetPos(ptf_startpoint);
// 					(*enemy_iter)->sprite_surface_id=enemy0_sfid;
// 					(*enemy_iter)->SetAnimateSequence("0,1,2,3,4,5");
// 					(*enemy_iter)->SetAnimateData("30|29,30|29,30|29,30|29,30|29,30|29");
// 					(*enemy_iter)->current_frame=(rand()%1);
// 					(*enemy_iter)->SetVelocity(5);
// 					++enemy_iter;
// 				}
				the_5th_enemy=1;
			}
		}
#endif

#if 0
//第六批
				if(mapdata.yCamera<2000){
			
			static the_6th_enemy=0;
			if(the_6th_enemy==0){
				//alertme("差不多到了");
				for (int i=0;i<=8;i++){
					enemy_list.push_back((cls_enemy *)new cls_enemy());
					ptf_startpoint.x=rand()%640-rand()%14;
					ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
					enemy_list.back()->SetPos(ptf_startpoint);
					enemy_list.back()->sprite_surface_id=enemy0_sfid;
					enemy_list.back()->SetAnimateSequence("0,1,2,3,4,5");
					enemy_list.back()->SetAnimateData("30|29,30|29,30|29,30|29,30|29,30|29");
					enemy_list.back()->current_frame=(rand()%1);
					enemy_list.back()->SetVelocity(5);
				}
// 				enemy_iter=enemy_list.begin();
// 				while(enemy_iter!=enemy_list.end()){
// 					ptf_startpoint.x=rand()%640-rand()%14;
// 					ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
// 					(*enemy_iter)->SetPos(ptf_startpoint);
// 					(*enemy_iter)->sprite_surface_id=enemy0_sfid;
// 					(*enemy_iter)->SetAnimateSequence("0,1,2,3,4,5");
// 					(*enemy_iter)->SetAnimateData("30|29,30|29,30|29,30|29,30|29,30|29");
// 					(*enemy_iter)->current_frame=(rand()%1);
// 					(*enemy_iter)->SetVelocity(5);
// 					++enemy_iter;
// 				}
				the_6th_enemy=1;
			}
		}
#endif
//boss死，更新地图
				if(boss1die && player->GetPos().y<-1000){
					lpddsTileset=reloadmap(lpdd,1);
					DSound_Stop_All_Sounds();
					DSound_Play(sound_ids[9]);
					boss1die=0;
				}

				if(mapdata.yCamera<2000 && stage_num==0){
					static initboss1=0;
					if (initboss1==0)
					{
						DSound_Stop_All_Sounds();
						DSound_Play(sound_ids[11]);
						DSound_Play(sound_ids[10]);
						boss1=new cls_boss();
						ptf_startpoint.x=300;
						ptf_startpoint.y=0;
						boss1->SetPos(ptf_startpoint);
						boss1->sprite_surface_id=boss1_sfid;
						boss1->SetAnimateSequence("0,1,2,3,4,5");
						boss1->SetAnimateData("216|226,216|226,216|226,216|226,216|226,216|226");
						boss1->current_frame=1;
						boss1->SetVelocity(3);
						boss1->SetHP(1280);
						MOVESTATE ms;
						ms.aimoveid=11;
						ms.move=11;
						ms.movetime=150;
						boss1->PushMoveState(ms);
						ms.aimoveid=12;
						ms.move=12;
						ms.movetime=150;
						boss1->PushMoveState(ms);
						boss1->use_move_fsm=1;
						boss1->loopmove=1;
					boss1->SetState(boss_normalattack);
					initboss1=1;
					}
				}


				if(mapdata.yCamera<2000 && stage_num==1){
					static initboss2=0;
					if (initboss2==0)
					{
						DSound_Stop_All_Sounds();
						DSound_Play(sound_ids[11]);
						DSound_Play(sound_ids[10]);
						boss2=new cls_boss();
						ptf_startpoint.x=270;
						ptf_startpoint.y=0;
						boss2->SetPos(ptf_startpoint);
						boss2->sprite_surface_id=boss2_sfid;
						boss2->SetAnimateSequence("0,1,2,3,4,5");
						boss2->SetAnimateData("139|142,139|142,139|142,139|142,139|142,139|142");
						boss2->current_frame=1;
						boss2->SetVelocity(3);
						boss2->SetHP(1580);
						MOVESTATE ms;
						ms.aimoveid=11;
						ms.move=11;
						ms.movetime=150;
						boss2->PushMoveState(ms);
						ms.aimoveid=12;
						ms.move=12;
						ms.movetime=150;
						boss2->PushMoveState(ms);
						boss2->use_move_fsm=1;
						boss2->loopmove=1;
						boss2->SetState(boss_normalattack3);
						initboss2=1;
					}
				}


if(mapdata.yCamera<2000){
		//先绘制阴影，ab效果ddraw真彩色吃不消！不行就改成黑色拉到
		/*if (boss1!=NULL)
		myBAMMX(lpddswork,lpddsback,boss1ai->GetNewPos(boss1->GetPos(),1).x,boss1ai->GetNewPos(boss1->GetPos(),1).y+20,
			120,&(sfobj->sprite_surface[boss1_shadow_sfid].sprite),sfobj->sprite_surface[boss1_shadow_sfid].sprite.frames[0]);
debugshow(0+offsetx,0+offsety);*/
}
	/*	DDraw_Draw_Surface(sfobj->sprite_surface[boss1_sfid].sprite.frames[0], 
			girl_swordman.x, girl_swordman.y,
			sfobj->sprite_surface[boss1_sfid].sprite.theframe_width,sfobj->sprite_surface[boss1_sfid].sprite.theframe_height,
			lpddsback,1);*/
#if 0
//玩家飞机,改成封装版本的
			DDraw_Draw_Surface(sfobj->sprite_surface[player_sfid].sprite.frames[player_plane_aniseq[player->GetCurrentFrame()]], 
			girl_swordman.x+offsetx*5, 480+offsety*4,
			sfobj->sprite_surface[player_sfid].sprite.theframe_width,sfobj->sprite_surface[player_sfid].sprite.theframe_height,
			lpddsback,1);
#endif


#if 1
//等级0子弹
/*			DDraw_Draw_Surface(sfobj->sprite_surface[bullet1_sfid].sprite.frames[0], 
				300+offsetx*5, 300+offsety*4,
				sfobj->sprite_surface[bullet1_sfid].sprite.theframe_width,sfobj->sprite_surface[bullet1_sfid].sprite.theframe_height,
				lpddsback,1);
			*/
			//enemy0
			
			/*DDraw_Draw_Surface(sfobj->sprite_surface[enemy0_sfid].sprite.frames[enemy_plane_aniseq[player->GetCurrentFrame()]], 
			100, 100,
			sfobj->sprite_surface[enemy0_sfid].sprite.theframe_width,sfobj->sprite_surface[enemy0_sfid].sprite.theframe_height,
			lpddsback,1);
			*/
			
			POINT ptpl;
			//更新主角坐标
			ptpl.x=player->GetPos().x+offsetx;ptpl.y=player->GetPos().y+offsety;
			if (player->GetState()!=starting && player->GetState()!=exitareaing)
			{
				//direction一个方向按下
				//左
				if (!KEYSTATE(VK_UP) && KEYSTATE(VK_LEFT)  && !KEYSTATE(VK_DOWN) ){
					//offsetx=-2;
					ptpl.x-=4;
				}
				//右
				if (!KEYSTATE(VK_UP) && KEYSTATE(VK_RIGHT)  && !KEYSTATE(VK_DOWN) ){
					//offsetx=2;
					ptpl.x+=4;
				}
				//上
				if (KEYSTATE(VK_UP) && !KEYSTATE(VK_RIGHT)  && !KEYSTATE(VK_LEFT) ){
					//offsety=-2;
					ptpl.y-=4;
				}
				//下
				if (KEYSTATE(VK_DOWN) && !KEYSTATE(VK_RIGHT)  && !KEYSTATE(VK_LEFT) ){
					//offsety=2;
					ptpl.y+=4;
				}
				//按下一个方向键的同时按下了SPACE
				//左control
				if (!KEYSTATE(VK_UP) && KEYSTATE(VK_LEFT)  && !KEYSTATE(VK_DOWN) && KEYSTATE(VK_CONTROL) ){
					//offsetx=-2;
					//pt.y-=2;
				}
				//右control
				if (!KEYSTATE(VK_UP) && KEYSTATE(VK_RIGHT)  && !KEYSTATE(VK_DOWN) && KEYSTATE(VK_CONTROL) ){
					//offsetx=2;
					//pt.x+=2;
				}
				//上control
				if (KEYSTATE(VK_UP) && !KEYSTATE(VK_RIGHT)  && !KEYSTATE(VK_LEFT) && KEYSTATE(VK_CONTROL) ){
					//offsety=-2;
					//pt.y-=2;
				}
				//下control
				if (KEYSTATE(VK_DOWN) && !KEYSTATE(VK_RIGHT)  && !KEYSTATE(VK_LEFT) && KEYSTATE(VK_CONTROL) ){
					//offsety=2;
					//pt.y+=2;
				}
				
				
				//direction二键同时按下
				//上左
				if (KEYSTATE(VK_UP)  && KEYSTATE(VK_LEFT) ){
					//MessageBox(hwnd,"test","info",MB_OK);
					//offsetx=-2;
					//offsety=-2;
					ptpl.x-=4;
					ptpl.y-=4;
				}
				//上右
				if (KEYSTATE(VK_UP)  && KEYSTATE(VK_RIGHT)   ){
					//MessageBox(hwnd,"test","info",MB_OK);
					//offsetx=2;
					//offsety=-2;
					ptpl.x+=4;
					ptpl.y-=4;
				}
				//下右
				if (KEYSTATE(VK_DOWN)  && KEYSTATE(VK_RIGHT)   ){
					//MessageBox(hwnd,"test","info",MB_OK);
					//offsetx=2;
					//offsety=2;
					ptpl.x+=4;
					ptpl.y+=4;
				}
				//下左
				if (KEYSTATE(VK_DOWN)  && KEYSTATE(VK_LEFT)   ){
					//MessageBox(hwnd,"test","info",MB_OK);
					//offsetx=-2;
					//offsety=2;
					ptpl.x-=4;
					ptpl.y+=4;
				}
				//限制主角移动范围
				if (player->GetState()==normaling)
				{
				if(ptpl.x<=115)
				ptpl.x=115;
				else if(ptpl.x>500)
					ptpl.x=500;
				if(ptpl.y<0)
					ptpl.y=0;
				else if(ptpl.y>447)
					ptpl.y=447;
				}

#if 1
				//按下2个方向的时候按下了SPACE
				//上左control
				if (KEYSTATE(VK_UP)  && KEYSTATE(VK_LEFT) && KEYSTATE(VK_CONTROL) ){
					//【三键检测是可以的！keystate是检测中断信号的，所以有可能某些
					//按键同时会检测不到，就比如上左空格，改成上左control就可以了】
					//MessageBox(hwnd,"test","info",MB_OK);Sleep(6);
					//offsetx=-2;
					//offsety=-2;
					//pt.x-=2;
					//pt.y-=2;
				}
				//上右control
				if (KEYSTATE(VK_UP)  && KEYSTATE(VK_RIGHT) && KEYSTATE(VK_CONTROL) ){
					//MessageBox(hwnd,"test","info",MB_OK);
					//offsetx=2;
					//offsety=-2;
					//pt.x+=2;
					//pt.y-=2;
				}
				//下右control
				if (KEYSTATE(VK_DOWN)  && KEYSTATE(VK_RIGHT) && KEYSTATE(VK_CONTROL) ){
					//MessageBox(hwnd,"test","info",MB_OK);
					//offsetx=2;
					//offsety=2;
					//pt.x+=2;
					//pt.y+=2;
				}
				//下左control
				if (KEYSTATE(VK_DOWN)  && KEYSTATE(VK_LEFT) && KEYSTATE(VK_CONTROL) ){
					//MessageBox(hwnd,"test","info",MB_OK);
					//offsetx=-2;
					//offsety=2;
					//pt.x-=2;
					//pt.y+=2;
				}
#endif
}

		if (player->GetHp()>0)
		{
		
			player->SetPos(ptpl);
// 			//画出飞机
// 			player->DrawMe(sfobj,player_sfid);
			
			//主角开火
			//fire!!!
			/*if(KEYSTATE(VK_SPACE))
			{
				player->AddBullet(pt);
			}*/
			//按键松开检测
			if (KEYSTATE(VK_CONTROL))
			{
				//DSound_Play(sound_ids[1]);
				++key_press_counter;
					if (option_state_which==option_state_eazy_lianfa || \
						option_state_which==option_state_normal_lianfa || \
						option_state_which==option_state_hard_lianfa
						)
					{
						key_press_counter=0;
				}
			}
			else{
				key_press_counter=0;
			}


			
			//子弹
			if(++g_iFireInputDelay>2 && KEYSTATE(VK_CONTROL) && key_press_counter<10)
			{
				//选择子弹等级，然后添加子弹
				int b_type=player->GetLevel() , b_sfid,b_firedelay;
				switch (b_type)
				{
				case 0:
					b_sfid=bullet0_sfid;
					b_firedelay=-3;
					break;
				case 1:
					b_sfid=bullet1_sfid;
					b_firedelay=-2;
					break;
				case 2:
					b_sfid=bullet2_sfid;
					b_firedelay=-1;
					break;
				case 3:
					b_sfid=bullet3_sfid;
					b_firedelay=0;
					break;
				}

				if (player->GetState()==exitareaing)
				{
				}
				else if (player->GetState()==normaling||flikering )
				{
					DSound_Play(sound_ids[1]);
					player->AddBullet(ptpl,b_type,b_sfid);
					ptpl.y-=40;
					DSound_Play(sound_ids[1]);
					player->AddBullet(ptpl,b_type,b_sfid);
				}
				
				//g_iFireInputDelay =0;
				g_iFireInputDelay =b_firedelay;
			}
			//end 子弹
			
			//导弹
			if(++g_iMissileInputDelay>30 && KEYSTATE(VK_CONTROL) && key_press_counter<10)
			{
				//选择子弹等级，然后添加子弹
				int b_type=player->GetLevel() , m_sfid,m_firedelay;
				switch (b_type)
				{
				case 0:
					m_sfid=-1;
					m_firedelay=-2;
					break;
				case 1:
					m_sfid=missile1_sfid;
					m_firedelay=-2;
					break;
				case 2:
					m_sfid=missile2_sfid;
					m_firedelay=-1;
					break;
				case 3:
					m_sfid=missile3_sfid;
					m_firedelay=-1;
					break;
				}
				
				if (player->GetState()==exitareaing)
				{
				}
				else if (player->GetState()==normaling||flikering )
				{
					if (m_sfid!=-1)
					{POINT ptm;
					ptm.x=ptpl.x+40;
					ptm.y=ptpl.y+30;
					player->AddBullet(ptm,10,missile6_sfid);
					//ptm.x=pt.x+40;
					//ptm.y=pt.y;
					player->AddBullet(ptm,11,missile3_sfid);
					}
				}
				
				//g_iFireInputDelay =0;
				g_iMissileInputDelay =m_firedelay;
			}
			//end 导弹

		}
			//移动子弹,死掉了子弹还是要运动的
			player->RunBullet(enemy_list);

			//敌人1
			enemy_iter=enemy_list.begin();
			while(enemy_iter!=enemy_list.end()){
				//敌人移动AI实现
				floatpoint ptf;
				ptf.x=(*enemy_iter)->GetPos().x;
				ptf.y=(*enemy_iter)->GetPos().y;
				//pt.x=ai->GetNewPos(pt,1).x;//Todo，改成定时切换的！
				//pt.y=ai->GetNewPos(pt,1).y;
				//if ((*enemy_iter)->use_move_fsm!=1)
				//{
				ptf.x=ai->GetNewPos(ptf,(*enemy_iter)->GetAiState()).x;
				ptf.y=ai->GetNewPos(ptf,(*enemy_iter)->GetAiState()).y;
				//}
				(*enemy_iter)->SetPos(ptf);
				(*enemy_iter)->UpdateFrame();
#if 1
				int jiajiao0;
				//计算和主角的夹角开始
				if (player->GetPos().y>(*enemy_iter)->GetPos().y){//主角在敌机下方
					if(player->GetPos().x!=(*enemy_iter)->GetPos().x){
						jiajiao0=atan(fabs(\
							((float)player->GetPos().y-(*enemy_iter)->GetPos().y)/\
							((float)player->GetPos().x-(*enemy_iter)->GetPos().x)\
							)\
							)/PI*180;
						//atan是弧度，弧度要转角度！
						//http://zhidao.baidu.com/question/42746372.html
//  						if(player->GetPos().x<(*enemy_iter)->GetPos().x)
//  							jiajiao0+=180;//主角在敌机左翼
//  						else
// 							jiajiao0+=270;//主角在敌机右翼
					}
// 					else
// 						jiajiao0=270;//主角和敌机垂直
						}
				//如果是坦克炮管指向主角
				dynamic_cast<cls_tank *>(*enemy_iter)->AimTarget(player,jiajiao0);
#endif
				//(*enemy_iter)->DrawMe(sfobj,enemy2_sfid);
				if((*enemy_iter)->HP!=0){
					(*enemy_iter)->DrawMe(sfobj,(*enemy_iter)->sprite_surface_id);
				}
				
				//敌机开火
				if(Fast_Distance_2D( (player->GetPos().x-(*enemy_iter)->GetPos().x),
					(player->GetPos().y-(*enemy_iter)->GetPos().y)
					)<600){//检测有效范围是
					//alertme("caution ,under attack!");
					if( ++((*enemy_iter)->b_counter)==(*enemy_iter)->iFireInputDelay){
						int jiajiao;
						//计算和主角的夹角开始
						if (player->GetPos().y>(*enemy_iter)->GetPos().y){//主角在敌机下方
							if(player->GetPos().x!=(*enemy_iter)->GetPos().x){
								jiajiao=atan(fabs(\
								((float)player->GetPos().y-(*enemy_iter)->GetPos().y)/\
								((float)player->GetPos().x-(*enemy_iter)->GetPos().x)\
								)\
								)/PI*180;
								//atan是弧度，弧度要转角度！
								//http://zhidao.baidu.com/question/42746372.html
								if(player->GetPos().x<(*enemy_iter)->GetPos().x)
									jiajiao+=180;//主角在敌机左翼
								else
									jiajiao+=270;//主角在敌机右翼
							}
							else
								jiajiao=270;//主角和敌机垂直
						}
						else
							jiajiao=-1;//如果主角在敌机上方就不发了。
						//计算和主角的夹角结束
						if (jiajiao!=-1){//下面改成enemy类的addbullet函数里引用就可以了
							if((*enemy_iter)->HP!=0){								
								dynamic_cast<cls_tank *>(*enemy_iter)->AimTarget(player,0);
								if((*enemy_iter)->GetPos().x<118 || 
									(*enemy_iter)->GetPos().x>552 ||
									(*enemy_iter)->GetPos().y>600){
									//如果离开屏幕太远，就不要发子弹了。
								}
								else{
									if ((*enemy_iter)->b_type==0 && (*enemy_iter)->use_fire_fsm!=1)
									{
										(*enemy_iter)->AddBullet(ptf,5,0,normal_e_bullet_sfid,jiajiao);//Todo此处检查泄漏。
									}
									else if ((*enemy_iter)->b_type==1 && (*enemy_iter)->use_fire_fsm!=1)
									{
										(*enemy_iter)->AddBullet(ptf,4,0,normal_e_bullet_sfid,225);
										(*enemy_iter)->AddBullet(ptf,4,0,normal_e_bullet_sfid,270);
										(*enemy_iter)->AddBullet(ptf,4,0,normal_e_bullet_sfid,315);
									//	(*enemy_iter)->AddBullet(pt,0);
									//	(*enemy_iter)->AddBullet(pt,45);
									//	(*enemy_iter)->AddBullet(pt,90);
									//	(*enemy_iter)->AddBullet(pt,135);
									//	(*enemy_iter)->AddBullet(pt,180);
									//Todo改成6向子弹，看看是不是垃圾回收提前回收了子弹
									}
									//2009-11-18新的敌机状态
									if ((*enemy_iter)->GetFireState()==enemy_normalattack && (*enemy_iter)->use_fire_fsm)
									{
										(*enemy_iter)->attack(tank_e_bullet_sfid);
									}
									else if ((*enemy_iter)->GetFireState()==enemy_normalattack1 && (*enemy_iter)->use_fire_fsm)
									{
										(*enemy_iter)->attack1(tank_e_bullet_sfid);
									}
									else if ((*enemy_iter)->GetFireState()==enemy_normalattack2 && (*enemy_iter)->use_fire_fsm)
									{
										(*enemy_iter)->attack2(tank_e_bullet_sfid);
									}
									else if ((*enemy_iter)->GetFireState()==enemy_normalattack3 && (*enemy_iter)->use_fire_fsm)
									{
										(*enemy_iter)->attack3(tank_e_bullet_sfid);
									}
								}
							}
							//没有泄露!stl自己会销毁类方法的malloc

						}
						(*enemy_iter)->b_counter=0;
					}
				}
				

				//改成判断敌机类子弹链表成员是不是为空并且越界。
				(*enemy_iter)->RunBullet();
				//死掉（HP和子弹都没有时）的画就删掉对象，但是HP为0的时候仅仅不画敌机
// 				if((*enemy_iter)->isdie){
// 					temp_enemy_iter=enemy_iter;
// 					delete(*temp_enemy_iter);
// 					++enemy_iter;
// 					enemy_list.erase(temp_enemy_iter);
// 				}

				(*enemy_iter)->DrawBullet(sfobj);
				//是否被敌机击中的检测

				if (player->GetState()==normaling)
				{

					if ((*enemy_iter)->IsAttack(player))
					{
						if (player->HP>=0)
						{
							DSound_Play(sound_ids[3]);
							ptf.x=player->GetPos().x;
							ptf.y=player->GetPos().y;
							sprite_list.push_back(new cls_sprite());
							sprite_list.back()->Bind(sfobj);
							sprite_list.back()->sprite_surface_id=expl_sfid;
							sprite_list.back()->SetAnimateSequence("0,1,2,3,4,5,6,7,8,9,10,11,12,13");
							sprite_list.back()->SetAnimateData("23|33,23|33,23|33,14|33,23|33,14|33");
							sprite_list.back()->SetSeqNum(14);
							sprite_list.back()->current_frame=0;
							//改进过的速度，改成最大是60
							sprite_list.back()->SetVelocity(57);
							sprite_list.back()->SetPos(ptf);
						}
						POINT ptdie;
						ptdie.x=300;
						ptdie.y=480;
						player->SetPos(ptdie);
						player->SetState(starting);
						//	MessageBox(NULL,"我死了","",MB_OK);
					}
				}



				//是否击中敌机的检测
				if(player->IsAttack(*enemy_iter)){
					DSound_Play(sound_ids[5]);//集中火花
					ptf.x=(*enemy_iter)->GetPos().x;
					ptf.y=(*enemy_iter)->GetPos().y;
					/*expl->SetPos(pt);
					expl->UpdateFrame();
					expl->DrawMe(sfobj,expl_sfid);*/
					spark->SetPos(ptf);
					spark->UpdateFrame();
					spark->DrawMe(sfobj,spark_sfid,0,-35);

					//
					temp_enemy_iter=enemy_iter;//临时迭代器
					if((*temp_enemy_iter)->HP<0)
						(*temp_enemy_iter)->HP=0;
					if((*temp_enemy_iter)->HP==0){//改成子弹消失时候销毁敌机对象
						DSound_Play(sound_ids[2]);
						//delete(*temp_enemy_iter);
						hits+=1;//击坠数+1
						//是否有奖励的检测
						if ((*enemy_iter)->GetBonus()==power)
						{
							//MessageBeep(MB_ICONASTERISK);
							//////////////////////////////////////////////////////////////////////////
							floatpoint pt;
							pt.x=(*enemy_iter)->GetPos().x;
							pt.y=(*enemy_iter)->GetPos().y;
							sprite_list2.push_back(new cls_sprite());
							sprite_list2.back()->Bind(sfobj);
							sprite_list2.back()->sprite_surface_id=power_sfid;
							sprite_list2.back()->SetAnimateSequence("0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1");
							sprite_list2.back()->SetAnimateData("74|31,74|31,74|31,74|31,74|31,74|31,74|31,74|31,74|31,74|31,74|31,74|31");
							sprite_list2.back()->SetSeqNum(12);
							sprite_list2.back()->current_frame=0;
							//改进过的速度，改成最大是60
							sprite_list2.back()->SetVelocity(55);
							sprite_list2.back()->SetNspeed(7);
							sprite_list2.back()->SetPos(pt);
							sprite_list2.back()->sprite_type=1;
							sprite_list2.back()->SetAngle(315);
							//////////////////////////////////////////////////////////////////////////
						}
						++enemy_iter;
						//expl->PlayOnce(sfobj,expl_sfid);
						//精灵封装类测试，压入1个爆炸精灵对象
#if 1
						sprite_list.push_back(new cls_sprite());
						sprite_list.back()->Bind(sfobj);
						sprite_list.back()->sprite_surface_id=expl_sfid;
						sprite_list.back()->SetAnimateSequence("0,1,2,3,4,5,6,7,8,9,10,11,12,13");
						sprite_list.back()->SetAnimateData("23|33,23|33,23|33,14|33,23|33,14|33");
						sprite_list.back()->SetSeqNum(14);
						sprite_list.back()->current_frame=0;
						//改进过的速度，改成最大是60
						sprite_list.back()->SetVelocity(57);
						sprite_list.back()->SetPos(ptf);

						

#endif			
						//enemy_list.erase(temp_enemy_iter);
					}
					else {
						++enemy_iter;
					}//end if hp==0
				}//end if player->isattack
				else{//如果没有攻击到的话，就放爆炸图，如果有爆炸的话。	(移到后面处理了)					
//					sprite_list_iter=sprite_list.begin();
//					while(sprite_list_iter!=sprite_list.end()){
// 						(*sprite_list_iter)->UpdateFrame();
// 						//如果是爆炸和其他无效果精灵，那么放完就可以了
// 						if((*sprite_list_iter)->sprite_type==0 && \
// 							(*sprite_list_iter)->PlayTime(1,(*sprite_list_iter)->sprite_surface_id)){
// 							temp_sprite_list_iter=sprite_list_iter;
// 							delete(*temp_sprite_list_iter); 
// 							++sprite_list_iter;
// 							sprite_list.erase(temp_sprite_list_iter);
// 						}
// 						//如果是能量
// 						else if((*sprite_list_iter)->sprite_type==1 && \
// 							(*sprite_list_iter)->PlayTime(12,power_sfid)){
// 								temp_sprite_list_iter=sprite_list_iter;
// 								delete(*temp_sprite_list_iter); 
// 								++sprite_list_iter;
// 								sprite_list.erase(temp_sprite_list_iter);
// 						}
// 						else
//							++sprite_list_iter;
//					}
					++enemy_iter;
				}
			}


			///////////////是否被boss1攻击的检测/////////////////////
			if (player->GetState()==normaling && boss1!=NULL)
			{
				
				if (boss1->IsAttack(player))
				{
					if (player->HP>=0)
					{
						floatpoint ptf;
						ptf.x=player->GetPos().x;
						ptf.y=player->GetPos().y;
						sprite_list.push_back(new cls_sprite());
						sprite_list.back()->Bind(sfobj);
						sprite_list.back()->sprite_surface_id=expl_sfid;
						sprite_list.back()->SetAnimateSequence("0,1,2,3,4,5,6,7,8,9,10,11,12,13");
						sprite_list.back()->SetAnimateData("23|33,23|33,23|33,14|33,23|33,14|33");
						sprite_list.back()->SetSeqNum(14);
						sprite_list.back()->current_frame=0;
						//改进过的速度，改成最大是60
						sprite_list.back()->SetVelocity(57);
						sprite_list.back()->SetPos(ptf);
					}
					POINT ptdie;
					ptdie.x=300;
					ptdie.y=480;
					player->SetPos(ptdie);
					player->SetState(starting);
					//	MessageBox(NULL,"我死了","",MB_OK);
				}
			}
			///////////////是否被boss2攻击的检测/////////////////////
			if (player->GetState()==normaling && boss2!=NULL)
			{
				
				if (boss2->IsAttack(player))
				{
					if (player->HP>=0)
					{
						floatpoint ptf;
						ptf.x=player->GetPos().x;
						ptf.y=player->GetPos().y;
						sprite_list.push_back(new cls_sprite());
						sprite_list.back()->Bind(sfobj);
						sprite_list.back()->sprite_surface_id=expl_sfid;
						sprite_list.back()->SetAnimateSequence("0,1,2,3,4,5,6,7,8,9,10,11,12,13");
						sprite_list.back()->SetAnimateData("23|33,23|33,23|33,14|33,23|33,14|33");
						sprite_list.back()->SetSeqNum(14);
						sprite_list.back()->current_frame=0;
						//改进过的速度，改成最大是60
						sprite_list.back()->SetVelocity(57);
						sprite_list.back()->SetPos(ptf);
					}
					POINT ptdie;
					ptdie.x=300;
					ptdie.y=480;
					player->SetPos(ptdie);
					player->SetState(starting);
					//	MessageBox(NULL,"我死了","",MB_OK);
				}
			}
			//////////////////////////////////////////////////////////////////////////

			//已经死亡敌机对象回收s
			enemy_iter=enemy_list.begin();
			while(enemy_iter!=enemy_list.end()){
// 				if (the_5th_enemy)
// 				{
// 					MessageBeep(MB_ICONASTERISK);
// 				Sleep(400);
// 				}
// 				if (enemy_iter==enemy_list.end() && the_5th_enemy)
// 				{return 0;
// 				}

				temp_enemy_iter=enemy_iter;
				//死掉（HP和子弹都没有时）的画就删掉对象，但是HP为0的时候仅仅不画敌机
				if((*enemy_iter)->isdie){
					delete(*temp_enemy_iter);
					++enemy_iter;
					enemy_list.erase(temp_enemy_iter);
				}
				//越界的视为死亡，也删掉对象
				else if((*enemy_iter)->GetPos().x<-2000 || 
					(*enemy_iter)->GetPos().x>2000 ||
					(*enemy_iter)->GetPos().y>2000
					){
					(*enemy_iter)->HP=0;
					//delete(*temp_enemy_iter);
					++enemy_iter;
					//enemy_list.erase(temp_enemy_iter);
				}
				//否则也迭代下去
				else{
					++enemy_iter;
				}
			}
			//已经死亡敌机对象回收e
			
			//Boss action
#if 1
			if (mapdata.yCamera<=1100 && stage_num==0)
			{
				mapdata.yCamera=2000;
			}
			else if (mapdata.yCamera<=50 && stage_num==1)
			{
				mapdata.yCamera=1000;
			}

			floatpoint ptf;
			if(mapdata.yCamera<11000){
				//
				if (boss1!=NULL && boss1->HP<0)
				{
					boss1->HP=0;
				}
				if(boss1!=NULL && boss1->HP==0){
					DSound_Stop_All_Sounds();
					DSound_Play(sound_ids[13]);
					DSound_Play(sound_ids[12]);
					player->SetState(exitareaing);
					for (int i=0;i<3;i++)
					{
						if (i==0){
							ptf.x=boss1->GetPos().x+80;
							ptf.y=boss1->GetPos().y+120;
						}
						else if (i==1){
							ptf.x=boss1->GetPos().x-60;
							ptf.y=boss1->GetPos().y-80;
						}
						else if (i==2){
							ptf.x=boss1->GetPos().x+110;
							ptf.y=boss1->GetPos().y+130;
						}
						sprite_list.push_back(new cls_sprite());
						sprite_list.back()->Bind(sfobj);
						sprite_list.back()->sprite_surface_id=bigexpl_sfid;
						sprite_list.back()->SetAnimateSequence("0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27");
						sprite_list.back()->SetAnimateData("300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,\
																																			  300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,\
																																			  300|228,300|228,300|228,300|228");
						sprite_list.back()->SetSeqNum(28);
						sprite_list.back()->current_frame=0;
						sprite_list.back()->SetVelocity(57);
						sprite_list.back()->SetPos(ptf);
					}
					//delete point
					delete boss1;
					boss1=NULL;
					boss1die=1;
				}
				else{
					if(boss1!=NULL){
						ptf.x=boss1->GetPos().x;
						ptf.y=boss1->GetPos().y;
						//ptf.x=ai->GetNewPos(ptf,boss1->GetAiState()).x;
						//ptf.y=ai->GetNewPos(ptf,boss1->GetAiState()).y;
						if (++bossstatecounter<600)
						{ptf.x=boss1ai->GetNewPos(ptf,1).x;//Todo，改成定时切换的！
						ptf.y=boss1ai->GetNewPos(ptf,1).y;
						}
						else if (++bossstatecounter>=500 && bossstatecounter<=800)
						{ptf.x=boss1ai->GetNewPos(ptf,2).x;//Todo，改成定时切换的！
						ptf.y=boss1ai->GetNewPos(ptf,2).y;
						}
						else
							bossstatecounter=0;

						boss1->SetPos(ptf);
						boss1->UpdateFrame();//Todo每帧其实都一样没有解决好子弹和图片的分离。
						boss1->DrawMe(sfobj,boss1_sfid);
						//Todo:这里改成一定几率发射激光
						boss1->FireLaser();
						if (boss1->GetState()==boss_normalattack)
						{
							boss1->normal_attack(big_e_bullet_sfid);
						}
						if (boss1->GetState()==boss_normalattack1)
						{
							boss1->normal_attack1(big_e_bullet_sfid);
						}
						if (boss1->GetState()==boss_normalattack2)
						{
							boss1->normal_attack2(soft_e_bullet_sfid);
						}
						boss1->RunBullet();
						boss1->DrawBullet(sfobj);
						if(player->IsAttack(boss1)){
							ptf.x=boss1->GetPos().x;
							ptf.y=boss1->GetPos().y;
							/*expl->SetPos(pt);
							expl->UpdateFrame();
							expl->DrawMe(sfobj,expl_sfid);*/
							bossspark->SetPos(ptf);
							bossspark->UpdateFrame();
							bossspark->DrawMe(sfobj,bossspark_sfid,0,0);
						}//endif isattack
					}//endif boss1 is null
				}

								//
				if (boss2!=NULL && boss2->HP<0)
				{
					boss2->HP=0;
				}
				if(boss2!=NULL && boss2->HP==0){
					DSound_Stop_All_Sounds();
					DSound_Play(sound_ids[13]);
					DSound_Play(sound_ids[12]);
					player->SetState(exitareaing);
					for (int i=0;i<3;i++)
					{
						if (i==0){
							ptf.x=boss2->GetPos().x+80;
							ptf.y=boss2->GetPos().y+120;
						}
						else if (i==1){
							ptf.x=boss2->GetPos().x-60;
							ptf.y=boss2->GetPos().y-80;
						}
						else if (i==2){
							ptf.x=boss2->GetPos().x+110;
							ptf.y=boss2->GetPos().y+130;
						}
						sprite_list.push_back(new cls_sprite());
						sprite_list.back()->Bind(sfobj);
						sprite_list.back()->sprite_surface_id=bigexpl_sfid;
						sprite_list.back()->SetAnimateSequence("0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27");
						sprite_list.back()->SetAnimateData("300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,\
																																			  300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,300|228,\
																																			  300|228,300|228,300|228,300|228");
						sprite_list.back()->SetSeqNum(28);
						sprite_list.back()->current_frame=0;
						sprite_list.back()->SetVelocity(57);
						sprite_list.back()->SetPos(ptf);
					}
					//delete point
					delete boss2;
					boss2=NULL;
					boss2die=1;
				}
				else{
					if(boss2!=NULL){
						ptf.x=boss2->GetPos().x;
						ptf.y=boss2->GetPos().y;
						//ptf.x=ai->GetNewPos(ptf,boss2->GetAiState()).x;
						//ptf.y=ai->GetNewPos(ptf,boss2->GetAiState()).y;
						if (++bossstatecounter<400)
						{
						ptf.x=boss1ai->GetNewPos(ptf,1).x;//Todo，改成定时切换的！
						ptf.y=boss1ai->GetNewPos(ptf,1).y;
						}
						else if (++bossstatecounter>=400 && bossstatecounter<=800)
						{
						ptf.x=boss1ai->GetNewPos(ptf,2).x;//Todo，改成定时切换的！
						ptf.y=boss1ai->GetNewPos(ptf,2).y;
						}
						else if (bossstatecounter>=800 && bossstatecounter<=1000)
						{
							ptf.x=boss1ai->GetNewPos(ptf,5).x;//Todo，改成定时切换的！
							ptf.y=boss1ai->GetNewPos(ptf,5).y;
						}
						else
							bossstatecounter=0;
						
						boss2->SetPos(ptf);
						boss2->UpdateFrame();//Todo每帧其实都一样没有解决好子弹和图片的分离。
						boss2->DrawMe(sfobj,boss2_sfid);
						//Todo:这里改成一定几率发射激光
						//boss2->FireLaser();
						if (boss2->GetState()==boss_normalattack)
						{
							boss2->normal_attack(big_e_bullet_sfid);
						}
						if (boss2->GetState()==boss_normalattack1)
						{
							boss2->normal_attack1(big_e_bullet_sfid);
						}
						if (boss2->GetState()==boss_normalattack2)
						{
							boss2->normal_attack2(soft_e_bullet_sfid);
						}
						if (boss2->GetState()==boss_normalattack3)
						{
							boss2->normal_attack3(normal_e_bullet_sfid);
						}
						if (boss2->GetState()==boss_normalattack4)
						{
							boss2->normal_attack4(big_e_bullet_sfid);
						}
						boss2->RunBullet();
						boss2->DrawBullet(sfobj);
						if(player->IsAttack(boss2)){
							ptf.x=boss2->GetPos().x;
							ptf.y=boss2->GetPos().y;
							/*expl->SetPos(pt);
							expl->UpdateFrame();
							expl->DrawMe(sfobj,expl_sfid);*/
							bossspark->SetPos(ptf);
							bossspark->UpdateFrame();
							bossspark->DrawMe(sfobj,boss2spark_sfid,0,0);
						}//endif isattack
					}//endif boss1 is null
				}				
}
#endif
//更新导弹画面
MISSILE_SURFACE mss;
mss.surface0=missile0_sfid;
mss.surface1=missile1_sfid;
mss.surface2=missile2_sfid;
mss.surface3=missile3_sfid;
mss.surface4=missile4_sfid;
mss.surface5=missile5_sfid;
mss.surface6=missile6_sfid;
mss.surface7=missile7_sfid;
mss.surface8=missile8_sfid;
mss.surface9=missile9_sfid;
mss.surface10=missile10_sfid;
mss.surface11=missile11_sfid;
mss.surface12=missile12_sfid;
mss.surface13=missile13_sfid;
mss.surface14=missile14_sfid;
mss.surface15=missile15_sfid;
player->UpdateMissileSurface(&mss);
//画出子弹和导弹（用精灵类做）
player->DrawBullet(sfobj);
#endif

	//	DDraw_Draw_Surface(sfobj->sprite_surface[6].sprite.frames[av_boss_aniseq[girl_swordman.current_frame]], 
	//		200, 0,
	//		sfobj->sprite_surface[6].sprite.theframe_width,sfobj->sprite_surface[6].sprite.theframe_height,
	//		lpddsback,1);


		rcRect.top=0;
		rcRect.left=0;
		rcRect.right=239;
		rcRect.bottom=239;	
//AlphaBlend16性能不怎么样
//AlphaBlend16(lpddswork,rcRect,girl_swordman.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],rcRect,5555);
		//		BltAlphaFastMMX( lpddsback, girl_swordman.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]], 0, 0, NULL, 1 );
		//好了，只要设置iAlpha为255就可以了，不要去用DDraw_Draw_Surface函数了，不适合新载入的画面算法。
		static float iAlpha;static int goinverse;
#if 0
		myBAMMX(lpddswork,lpddsback,0+offsetx,0+offsety,
			iAlpha,&(sfobj->sprite_surface[1].sprite),sfobj->sprite_surface[1].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]]);
debugshow(0+offsetx,0+offsety);
#endif


sprite_list_iter=sprite_list2.begin();
while(sprite_list_iter!=sprite_list2.end()){
	(*sprite_list_iter)->UpdateFrame();
	floatpoint d;
// 	static int powerd=0;
// 	if (powerd==0)
// 	{
// 		d.x=200.12f;d.y=50.00f;
// 		powerd=1;
// 	}
	d.x=(*sprite_list_iter)->GetPos().x;
	d.y=(*sprite_list_iter)->GetPos().y;
//	d.x=powerai->GetNewPos(d,2,(*sprite_list_iter)->GetAngle()).x;
	//d.x=powerai->GetNewPos(d,2).x;
	//d.y=powerai->GetNewPos(d,2).y;
/*	d.x=powerai->GetNewAnglePos(d,(*sprite_list_iter)->GetNspeed(),\
		(*sprite_list_iter)->GetAngle()).x;
	d.y=powerai->GetNewAnglePos(d,(*sprite_list_iter)->GetNspeed(),\
		(*sprite_list_iter)->GetAngle()).y;
(*sprite_list_iter)->SetPos(d);
(*sprite_list_iter)->SetAngle(powerai->GetNewAnglePos(d,(*sprite_list_iter)->GetNspeed(),\
		(*sprite_list_iter)->GetAngle()).angle);*/
	movedata md;
	md=powerai->GetNewAnglePos(d,(*sprite_list_iter)->GetNspeed(),\
		0,(*sprite_list_iter)->GetAngle());
	d.x=md.x;
	d.y=md.y;
	(*sprite_list_iter)->SetPos(d);
	(*sprite_list_iter)->SetAngle(md.angle);
	//d.x=powerai->GetNewPos(d,2)
	(*sprite_list_iter)->PlayTime(12,power_sfid);
	//主角是否获取能量
	int getit;
getit = Collision_Test(player->GetPos().x,player->GetPos().y,player->anidata_list[player->GetCurrentFrame()]->width,\
					   player->anidata_list[player->GetCurrentFrame()]->height,\
					   (*sprite_list_iter)->GetPos().x,(*sprite_list_iter)->GetPos().y,\
					   37,\
					   31);
if (getit==1)
{
	//MessageBeep(MB_ICONASTERISK);
	DSound_Play(sound_ids[4]);
	player->LevelUp();
	temp_sprite_list_iter=sprite_list_iter;
	delete(*sprite_list_iter);
	++sprite_list_iter;
			sprite_list2.erase(temp_sprite_list_iter);
}
else
	++sprite_list_iter;
}

sprite_list_iter=sprite_list.begin();
while(sprite_list_iter!=sprite_list.end()){
(*sprite_list_iter)->UpdateFrame();
//如果是爆炸和其他无效果精灵，那么放完就可以了
if((*sprite_list_iter)->sprite_type==0 && \
   (*sprite_list_iter)->PlayTime(1,(*sprite_list_iter)->sprite_surface_id)){
	temp_sprite_list_iter=sprite_list_iter;
	delete(*temp_sprite_list_iter); 
	++sprite_list_iter;
	sprite_list.erase(temp_sprite_list_iter);
}
////如果是能量
// else if((*sprite_list_iter)->sprite_type==1 && \
// 		(*sprite_list_iter)->PlayTime(12,power_sfid)){
// 	temp_sprite_list_iter=sprite_list_iter;
// 	delete(*temp_sprite_list_iter); 
// 	++sprite_list_iter;
// 	sprite_list.erase(temp_sprite_list_iter);
// }
else
++sprite_list_iter;
}


//
// enemy_iter=enemy_list.begin();
// 			while(enemy_iter!=enemy_list.end()){
// 				//如果是坦克炮管指向主角
// 				dynamic_cast<cls_tank *>(*enemy_iter)->AimTarget(player,0);
// 				++enemy_iter;
// 			}
		//BltAlphaMMX( lpddsback, sfobj->sprite_surface[2].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],200, 0, NULL, ( int ) iAlpha, 1 );
		//BltAlphaMMX( lpddsback, sfobj->sprite_surface[0].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],200, 200, NULL, ( int ) 255, 1 );
		//BltAlphaMMX( lpddsback, sfobj->sprite_surface[0].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],300, 0, NULL, ( int ) iAlpha, 1 );
		//飞机
		//BltAlphaMMX( lpddsback, sfobj->sprite_surface[3].sprite.frames[player_plane_aniseq[sfobj->sprite_surface[3].sprite.current_frame]],sfobj->sprite_surface[3].sprite.x, sfobj->sprite_surface[3].sprite.y, NULL, ( int ) iAlpha, 1 );
		//绿色改boss
		//BltAlphaMMX( lpddsback, sfobj->sprite_surface[boss1_sfid].sprite.frames[0],300, 0, NULL, ( int ) 30, 1 );
		//云雾效果
		//BltAlphaMMX( lpddsback, sfobj->sprite_surface[4].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],340, 230, NULL, ( int ) iAlpha, 1 );
		/*							BltAlphaMMX( lpddsback, sfobj->sprite_surface[0].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],\
		400, 120, NULL, ( int ) iAlpha, 1 );
		BltAlphaMMX( lpddsback, sfobj->sprite_surface[0].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],\
		400, 210, NULL, ( int ) iAlpha, 1 );
		BltAlphaMMX( lpddsback, sfobj->sprite_surface[0].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],\
		400, 40, NULL, ( int ) iAlpha, 1 );
		BltAlphaMMX( lpddsback, sfobj->sprite_surface[0].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],\
		300, 60, NULL, ( int ) iAlpha, 1 );
		BltAlphaMMX( lpddsback, sfobj->sprite_surface[0].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],\
		260, 121, NULL, ( int ) iAlpha, 1 );*/
		/*if(FAILED(lpddswork2->BltFast(0,0,
	                 lpddswork,lprcrect,
					 DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY)))
		{
			MessageBox(globalhwnd,"failed in blt","DRAWENEMY",NULL);
		    return FALSE;
		}*/
		if (iAlpha<255 && !goinverse)
		iAlpha++;
		if (iAlpha==255){
			iAlpha--;goinverse=1;
		}
		if(iAlpha<255 && goinverse)
			iAlpha--;
		if (iAlpha==0){
			iAlpha++;goinverse=0;
		}

		//画出飞机
		if (player->GetHp()>0)
		{
				player->DrawMe(sfobj,player_sfid);
		}
		

		//显示第一关
		static int zimu_offset=0;static int zimu_offset1=0;
		if (stage_num==0)
		{
			if (mapdata.yCamera < 11000 && mapdata.yCamera >10250 && player->HP>0)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[mission1_sfid].sprite.frames[0], 
					140+zimu_offset, 100,
					229,144,
					lpddsback,1);
				if(mapdata.yCamera < 10940 && mapdata.yCamera >10700){
					//字母停留一下
				}
				else{
					zimu_offset+=1;
				}
			}
		}
		else if (stage_num==1)
		{
			if (mapdata.yCamera < 11000 && mapdata.yCamera >10250 && player->HP>0)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[mission2_sfid].sprite.frames[0], 
					140+zimu_offset1, 100,
					229,144,
					lpddsback,1);
				if(mapdata.yCamera < 10940 && mapdata.yCamera >10700){
					//字母停留一下
				}
				else{
					zimu_offset1+=1;
				}
			}
		}

		

//左侧边栏
		DDraw_Draw_Surface(leftbar_surface, 
			0, 0,
			118,481,
			lpddsback,0);
//右侧边栏
		//DDraw_Draw_Surface(sfobj->sprite_surface[rightbar_sfid].sprite.frames[0], 
		DDraw_Draw_Surface(rightbar_surface, 
			523, 0,
			118,481,
			lpddsback,0);
		//显示HITS
		showHITS(hits);
		//显示HP
		showHP(player->GetHp(),20);
//出品
// 		DDraw_Draw_Surface(sfobj->sprite_surface[product_sfid].sprite.frames[0], 
// 			223, 300,
// 			200,36,
// 			lpddsback,1);
		RECT pdrc;
		pdrc.top=0;
		pdrc.left=0;
		pdrc.right=102;
		pdrc.bottom=57;
		BltAlphaMMX( lpddsback,sfobj->sprite_surface[product_sfid].sprite.frames[0] ,\
		419, 420, &pdrc, ( int ) 66, 1 );
		//显示HITS
		showHITS(hits);
		//显示HP
		showHP(player->GetHp(),20);
//临时boss2
// 		DDraw_Draw_Surface(sfobj->sprite_surface[boss2_sfid].sprite.frames[0], 
// 			210, 110,
// 			139,142,
// 			lpddsback,1);
		
//如果结束游戏显示挂了
		if(player==NULL || player->GetHp()<=0){
			//if (iAlpha<255)
			//{
			
				BltAlphaMMX( lpddsback,sfobj->sprite_surface[alphabg_sfid].sprite.frames[0] ,\
		0, 0, NULL, ( int ) iAlpha, 1 );
				
			//}
		DDraw_Draw_Surface(sfobj->sprite_surface[gameover_sfid].sprite.frames[0], 
			110, 100,
			422,258,
			lpddsback,1);
		}
//主界面一共9帧辉光效果
		if(COPYRIGHT==0){
			if (++shinecounter<10)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[mainloginbg_sfid].sprite.frames[0], 
					0, 0,
					640,481,
					lpddsback,1);
			}
			else if(++shinecounter<10)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[mainloginbg_sfid].sprite.frames[1], 
					0, 0,
					640,481,
					lpddsback,1);
			}
			else if(++shinecounter<17)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[mainloginbg_sfid].sprite.frames[2], 
					0, 0,
					640,481,
					lpddsback,1);
			}
			else if(++shinecounter<23)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[mainloginbg_sfid].sprite.frames[3], 
					0, 0,
					640,481,
					lpddsback,1);
			}
			else if(++shinecounter<30)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[mainloginbg_sfid].sprite.frames[4], 
					0, 0,
					640,481,
					lpddsback,1);
			}
			else if(++shinecounter<37)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[mainloginbg_sfid].sprite.frames[5], 
					0, 0,
					640,481,
					lpddsback,1);
			}
			else if(++shinecounter<44)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[mainloginbg_sfid].sprite.frames[6], 
					0, 0,
					640,481,
					lpddsback,1);
			}
			else if(++shinecounter<49)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[mainloginbg_sfid].sprite.frames[7], 
					0, 0,
					640,481,
					lpddsback,1);
			}
			else if(++shinecounter<55)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[mainloginbg_sfid].sprite.frames[8], 
					0, 0,
					640,481,
					lpddsback,1);
			}
			else if (++shinecounter<800)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[mainloginbg_sfid].sprite.frames[0], 
					0, 0,
					640,481,
					lpddsback,1);
				
			}
			else{				DDraw_Draw_Surface(sfobj->sprite_surface[mainloginbg_sfid].sprite.frames[0], 
				0,0 ,
				640,481,
				lpddsback,1);
			shinecounter=0;
			}
		

		if (whichopt==startgameopt){//start按钮
			DDraw_Draw_Surface(sfobj->sprite_surface[opt1_sfid].sprite.frames[0], 
				72, 443,
				132,38,
			lpddsback,1);
		}
		if (whichopt==optionopt){//option按钮
			DDraw_Draw_Surface(sfobj->sprite_surface[opt2_sfid].sprite.frames[0], 
				254, 443,
				78,38,
				lpddsback,1);
		}
		if (whichopt==staffopt){//staff按钮
			DDraw_Draw_Surface(sfobj->sprite_surface[opt3_sfid].sprite.frames[0], 
				399, 443,
				64,38,
				lpddsback,1);
		}
		if (whichopt==exitopt){//exit按钮
			DDraw_Draw_Surface(sfobj->sprite_surface[opt4_sfid].sprite.frames[0], 
				544, 443,
				58,38,
				lpddsback,1);
		}
		if(enter_staff_bg){
		DDraw_Draw_Surface(sfobj->sprite_surface[staff_sfid].sprite.frames[0], 
			0, 0,
			640,480,
			lpddsback,1);
		}
		else if(enter_option_bg){
			DDraw_Draw_Surface(sfobj->sprite_surface[option_sfid].sprite.frames[0], 
				0, 0,
				640,480,
				lpddsback,1);
			//选项文字
			if (option_state_which==option_state_eazy_shoudong)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[option_eazy_sfid].sprite.frames[0], 
					320, 110,
					165,58,
					lpddsback,1);
				DDraw_Draw_Surface(sfobj->sprite_surface[option_off_sfid].sprite.frames[0], 
					320, 260,
					103,58,
					lpddsback,1);
				if (option_edit_which==edit_difficult)//高亮
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_eazy1_sfid].sprite.frames[0], 
						320, 110,
						165,58,
					lpddsback,1);
				}
				else if (option_edit_which==edit_lianfa)//高亮
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_off1_sfid].sprite.frames[0], 
						320, 260,
						103,58,
						lpddsback,1);
				}
			}
			else if (option_state_which==option_state_normal_shoudong)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[option_normal_sfid].sprite.frames[0], 
					320, 110,
					165,58,
					lpddsback,1);
				DDraw_Draw_Surface(sfobj->sprite_surface[option_off_sfid].sprite.frames[0], 
					320, 260,
					103,58,
					lpddsback,1);
				if (option_edit_which==edit_difficult)//高亮
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_normal1_sfid].sprite.frames[0], 
						320, 110,
						165,58,
						lpddsback,1);
				}
				else if (option_edit_which==edit_lianfa)//高亮
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_off1_sfid].sprite.frames[0], 
						320, 260,
						103,58,
						lpddsback,1);
				}
			}
			else if (option_state_which==option_state_hard_shoudong)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[option_hard_sfid].sprite.frames[0], 
					320, 110,
					165,58,
					lpddsback,1);
				DDraw_Draw_Surface(sfobj->sprite_surface[option_off_sfid].sprite.frames[0], 
					320, 260,
					103,58,
					lpddsback,1);
				if (option_edit_which==edit_difficult)//高亮
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_hard1_sfid].sprite.frames[0], 
						320, 110,
						165,58,
						lpddsback,1);
				}
				else if (option_edit_which==edit_lianfa)//高亮
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_off1_sfid].sprite.frames[0], 
						320, 260,
						103,58,
						lpddsback,1);
				}
			}	
			else if (option_state_which==option_state_eazy_lianfa)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[option_eazy_sfid].sprite.frames[0], 
					320, 110,
					165,58,
					lpddsback,1);
				DDraw_Draw_Surface(sfobj->sprite_surface[option_on_sfid].sprite.frames[0], 
					320, 260,
					103,58,
					lpddsback,1);
				if (option_edit_which==edit_difficult)//高亮
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_eazy1_sfid].sprite.frames[0], 
						320, 110,
						165,58,
						lpddsback,1);
				}
				else if (option_edit_which==edit_lianfa)//高亮
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_on1_sfid].sprite.frames[0], 
						320, 260,
						103,58,
						lpddsback,1);
				}
			}
			else if (option_state_which==option_state_normal_lianfa)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[option_normal_sfid].sprite.frames[0], 
					320, 110,
					165,58,
					lpddsback,1);
				DDraw_Draw_Surface(sfobj->sprite_surface[option_on_sfid].sprite.frames[0], 
					320, 260,
					103,58,
					lpddsback,1);
				if (option_edit_which==edit_difficult)//高亮
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_normal1_sfid].sprite.frames[0], 
						320, 110,
						165,58,
						lpddsback,1);
				}
				else if (option_edit_which==edit_lianfa)//高亮
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_on1_sfid].sprite.frames[0], 
						320, 260,
						103,58,
						lpddsback,1);
				}
			}
			else if (option_state_which==option_state_hard_lianfa)
			{
				DDraw_Draw_Surface(sfobj->sprite_surface[option_hard_sfid].sprite.frames[0], 
					320, 110,
					165,58,
					lpddsback,1);
				DDraw_Draw_Surface(sfobj->sprite_surface[option_on_sfid].sprite.frames[0], 
					320, 260,
					103,58,
					lpddsback,1);
				if (option_edit_which==edit_difficult)//高亮
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_hard1_sfid].sprite.frames[0], 
						320, 110,
						165,58,
						lpddsback,1);
				}
				else if (option_edit_which==edit_lianfa)//高亮
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_on1_sfid].sprite.frames[0], 
						320, 260,
						103,58,
						lpddsback,1);
				}
			}
			//end 选项文字
		}
}
}//end if pause
else{
	RECT thegamewindow;
	thegamewindow.top=0;
	thegamewindow.right=522;
	thegamewindow.bottom=480;
			thegamewindow.left=118;
	BltAlphaMMX( lpddsback,sfobj->sprite_surface[alphabg_sfid].sprite.frames[0] ,\
		119, 0, &thegamewindow, ( int ) 150, 1 );
			DDraw_Draw_Surface(sfobj->sprite_surface[pause_sfid].sprite.frames[0], 
				270,210,
				102,42,
				lpddsback,1);

}
	if (DEBUG==1){
		//这个例子7.6不要求FLIP!
	}
	else{
		// lock to 30 FPS (33ms per frame)
		int wantfps=0;
		if (wantfps>70 && wantfps<=80)
			wantfps=22;//22下面while的实际效果是max75fps的话是32
		else if (wantfps>60 && wantfps<=70)
			wantfps=32;
		else
			wantfps=22;

// 		if (already_record_max_fps_time<300){
// 			already_record_max_fps_time+=1;
// 		}
// 		else{
// 			while ((GetTickCount() - dwStart) < wantfps);
// 			
// 		}
		/*if (FAILED(DDraw_Draw_Surface(option_surface, 
			0,0,
			640,480,
				lpddsback,0)))
		{
			MessageBox(globalhwnd,"error","",MB_OK);
		}*/
		
		while ((GetTickCount() - dwStart) < 22);
			
	while (lpddsprimary->Flip(NULL,DDFLIP_WAIT));
	}
	//MessageBox(hwnd,"Flip","信息",MB_OK);
	//Sleep(30);
	//


ReleaseDC(hwnd,hdc);

}
// return to Windows like this
return(msg.wParam);

} // end WinMain






int gameinit(){
//全局变量初始化
START_SELECT=-1;
ShowCursor(FALSE);
//主事件循环前加上gameinit,就是一般的初始化DD，什么的数据结构和全局变量什么的

if (FAILED(DirectDrawCreateEx(NULL, (void **)&lpdd, IID_IDirectDraw7, NULL))){
	return(0);}
else{
	if (ECHODEBUG==1)
		MessageBox(globalhwnd,"创建DD接口成功","信息",NULL);
}
if (FAILED(lpdd->QueryInterface(IID_IDirectDraw7,(LPVOID *)&lpdd))){//可以省去，没有IID_IDirectDraw8了
//echo error
}
else{
	if(ECHODEBUG==1)
		MessageBox(globalhwnd,"升级接口成功","信息",NULL);
}
if (FAILED(lpdd->SetCooperativeLevel(globalhwnd,DDSCL_FULLSCREEN|DDSCL_EXCLUSIVE|DDSCL_ALLOWREBOOT|DDSCL_ALLOWMODEX))){
//echo error
}
else{
	if(ECHODEBUG==1)
		MessageBox(globalhwnd,"设置协作等级成功","信息",NULL);
}
//lpdd->SetDisplayMode(640,480,8,0,0);
lpdd->SetDisplayMode(640,480,16,0,0);

//PALETTEENTRY          palette[256];          // color palette

LPDIRECTDRAWPALETTE lpddpal=NULL;
#if 0
//填充结构
// build up the palette data array
for (int color=1; color < 255; color++)
    {
    // fill with random RGB values
    palette[color].peRed   = rand()%256;
    palette[color].peGreen = rand()%256;
    palette[color].peBlue  = rand()%256;

    // set flags field to PC_NOCOLLAPSE
    palette[color].peFlags = PC_NOCOLLAPSE;
    } // end for color

// now fill in entry 0 and 255 with black and white
palette[0].peRed   = 0;
palette[0].peGreen = 0;
palette[0].peBlue  = 0;
palette[0].peFlags = PC_NOCOLLAPSE;

palette[255].peRed   = 255;
palette[255].peGreen = 255;
palette[255].peBlue  = 255;
palette[255].peFlags = PC_NOCOLLAPSE;


// 创建调色板对象
if (FAILED(lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 | 
                                DDPCAPS_INITIALIZE, 
                                palette,&lpddpal, NULL)))
{
// echo error
return(0);

} // end if
#endif
//memset(&ddsd,0,sizeof(ddsd));//为ddsd分配内存地址，也可以使用ZeroMemory
ZeroMemory(&ddsd,sizeof(ddsd));
//填充DDSURFACEDESC2数据结构
ddsd.dwSize=sizeof(ddsd);
DDRAW_INIT_STRUCT(ddsd); 
ddsd.dwFlags=DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
ddsd.dwBackBufferCount=1;
ddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE|DDSCAPS_COMPLEX|DDSCAPS_FLIP;
if (FAILED(lpdd->CreateSurface(&ddsd,&lpddsprimary,NULL))){
//echo error
}

#if 0//old windows gmp上的，有BUG做掉了！
//选择调色板

if(FAILED(lpddsprimary->SetPalette(lpddpal))){
//echo error
	MessageBox(globalhwnd,"设置调色板失败","信息",MB_OK);
}
#endif

#if 0
lpddpal = DDLoadPalette(lpdd, "Resource/bmp/map.bmp"); // Call the function above to load the palette

if (lpddpal)
    lpddsprimary->SetPalette(lpddpal); // this sets the palette for the primary surface

#endif
globallpddpal=lpddpal;



//sound
// initialize directsound
DSound_Init();

// initialize directmusic
DMusic_Init();


// load in the digital sound
sound_ids[0] = DSound_Load_WAV("Resource\\wave\\stage1.wav");
sound_ids[1] = DSound_Load_WAV("Resource\\wave\\shot.wav");
sound_ids[2] = DSound_Load_WAV("Resource\\wave\\bom3.wav");
sound_ids[3] = DSound_Load_WAV("Resource\\wave\\bom.wav");
sound_ids[4] = DSound_Load_WAV("Resource\\wave\\pow.wav");
sound_ids[5] = DSound_Load_WAV("Resource\\wave\\spark.wav");
sound_ids[6] = DSound_Load_WAV("Resource\\wave\\button.wav");
sound_ids[7] = DSound_Load_WAV("Resource\\wave\\button2.wav");
sound_ids[8] = DSound_Load_WAV("Resource\\wave\\begin.wav");
sound_ids[9] = DSound_Load_WAV("Resource\\wave\\stage2.wav");
sound_ids[10] = DSound_Load_WAV("Resource\\wave\\boss.wav");
sound_ids[11] = DSound_Load_WAV("Resource\\wave\\caution.wav");
sound_ids[12] = DSound_Load_WAV("Resource\\wave\\done.wav");
sound_ids[13] = DSound_Load_WAV("Resource\\wave\\bigbom.wav");

//char bufferx[20];
//sprintf(bufferx,"soundFILE%d",sound_ids[0]);
//MessageBox(globalhwnd,bufferx,"",MB_OK);

//sound_ids[1] = DSound_Load_WAV("EXP0.WAV");
//sound_ids[2] = DSound_Load_WAV("INCORRECT1.WAV");
//sound_ids[3] = DSound_Load_WAV("UPLINK1.WAV");

#if 0
// load the midi segments
for (int index=0; index<10; index++)
{
    // build up string name
    sprintf(buffer,"MIDIFILE%d.MID",index);
    // load midi file
    midi_ids[index] = DMusic_Load_MIDI(buffer);
    } // end for index
#endif


//
GetLastError();
return 1;
}

//消息回调函数
LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
// this is the main message handler of the system
PAINTSTRUCT		ps;		// used in WM_PAINT
HDC				hdc;	// handle to a device context

// what is the message 
switch(msg)
	{	
	case WM_CREATE: 
        {
		// do initialization stuff here
		g_hFont = CreateFont( 14, 0, 0, 0, 600, false, false, false, 
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
			VARIABLE_PITCH | FF_DONTCARE, "Times New Roman" );
		status_info_hFont= CreateFont( 14, 0, 0, 0, 600, false, false, false, 
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
			VARIABLE_PITCH | FF_DONTCARE, "Times New Roman" );
        // return success
		return(0);
		} break;

	case WM_PAINT: 
		{
		// simply validate the window
		hdc = BeginPaint(hwnd,&ps);	 
		// you would do all your painting here
        EndPaint(hwnd,&ps);

        // return success
		return(0);
   		} break;
		
	case WM_KEYDOWN:{//WM_KEYDOWN键盘事件
		int virtualcode=(int)wparam;
		int keystate=(int)lparam;


//检测键盘这部分代码移走了。

#if 0
			static int emu_return_once=0;
			if (emu_return_once==0){
				keybd_event(VK_RETURN,0,0,0);
				++emu_return_once;
			}
#endif
		switch(virtualcode){
			/*
		case VK_RIGHT:{//MessageBox(hwnd,"右","caution",MB_OK);
#ifdef USE_GAMEDEV_TILE_1
			world_camerax += 8;
#endif
			offsetx+=1;
					  }break;
					  */
			/*
		case VK_LEFT:{//MessageBox(hwnd,"左","caution",MB_OK);
#ifdef USE_GAMEDEV_TILE_1
			world_camerax -= 8;
#endif
			offsetx-=1;
					 }break;
					 */
			/*
		case VK_UP:{
			//MessageBox(hwnd,"上","caution",MB_OK);
			if (START_SELECT==1)
				START_SELECT=0;
			else if(START_SELECT==0)
				START_SELECT=1;
			//else
			//	START_SELECT=0;
			offsety-=1;
				   }break;
				   */
			/*
		case VK_DOWN:{
			//MessageBox(hwnd,"下","caution",MB_OK);
			if (START_SELECT==1)
				START_SELECT=0;
			else if(START_SELECT==0)
				START_SELECT=1;
			offsety+=1;
			//else
			//	START_SELECT=0;
					 }break;
					 */
		case VK_UP:{
			if (COPYRIGHT==0 && !enter_staff_bg && enter_option_bg)
			{
				DSound_Play(sound_ids[6]);
				//MessageBeep(MB_ICONASTERISK);
				if (option_edit_which==edit_difficult)
				{
					option_edit_which=edit_lianfa;
				}
				else{
					option_edit_which=edit_difficult;
				}

			}			
			break;
		}
		case VK_DOWN:{
			if (COPYRIGHT==0 && !enter_staff_bg && enter_option_bg)
			{
				DSound_Play(sound_ids[6]);
				//MessageBeep(MB_ICONASTERISK);
				if (option_edit_which==edit_difficult)
				{
					option_edit_which=edit_lianfa;
				}
				else{
					option_edit_which=edit_difficult;
				}
				
			}			
			break;
		}
		case VK_RIGHT:{
			if (COPYRIGHT==0 && !enter_staff_bg && !enter_option_bg)
			{
				DSound_Play(sound_ids[6]);
				//MessageBeep(MB_ICONASTERISK);
				if (whichopt<4)
				{
					whichopt+=1;
				}
				else{
					whichopt=1;
				}
			}
#if 1
			if (COPYRIGHT==0 && !enter_staff_bg && enter_option_bg){
				if (option_edit_which==edit_difficult){
					if (option_state_which==option_state_eazy_shoudong)
					{
						option_state_which=option_state_normal_shoudong;
					}
					else if (option_state_which==option_state_normal_shoudong)
					{
						option_state_which=option_state_hard_shoudong;
					}
					else if (option_state_which==option_state_hard_shoudong)
					{
						option_state_which=option_state_eazy_shoudong;
					}
					else if (option_state_which==option_state_eazy_lianfa)
					{
						option_state_which=option_state_normal_lianfa;
					}
					else if (option_state_which==option_state_normal_lianfa)
					{
						option_state_which=option_state_hard_lianfa;
					}
					else if (option_state_which==option_state_hard_lianfa)
					{
						option_state_which=option_state_eazy_lianfa;
					}
				}
				else if (option_edit_which==edit_lianfa){
					if (option_state_which==option_state_eazy_shoudong)
					{
						option_state_which=option_state_eazy_lianfa;
					}
					else if (option_state_which==option_state_normal_shoudong)
					{
						option_state_which=option_state_normal_lianfa;
					}
					else if (option_state_which==option_state_hard_shoudong)
					{
						option_state_which=option_state_hard_lianfa;
					}
					else if (option_state_which==option_state_eazy_lianfa)
					{
						option_state_which=option_state_eazy_shoudong;
					}
					else if (option_state_which==option_state_normal_lianfa)
					{
						option_state_which=option_state_normal_shoudong;
					}
					else if (option_state_which==option_state_hard_lianfa)
					{
						option_state_which=option_state_hard_shoudong;
					}
					
				}
			}
#endif
			break;
		}
		case VK_LEFT:{
			if (COPYRIGHT==0 && !enter_staff_bg && !enter_option_bg)
			{
				DSound_Play(sound_ids[6]);
				//MessageBeep(MB_ICONASTERISK);
				if (whichopt>1)
				{
					whichopt-=1;
				}
				else{
					whichopt=4;
				}
			}	
#if 1
			if (COPYRIGHT==0 && !enter_staff_bg && enter_option_bg){
				if (option_edit_which==edit_difficult){
					if (option_state_which==option_state_eazy_shoudong)
					{
						option_state_which=option_state_normal_shoudong;
					}
					else if (option_state_which==option_state_normal_shoudong)
					{
						option_state_which=option_state_hard_shoudong;
					}
					else if (option_state_which==option_state_hard_shoudong)
					{
						option_state_which=option_state_eazy_shoudong;
					}
					else if (option_state_which==option_state_eazy_lianfa)
					{
						option_state_which=option_state_normal_lianfa;
					}
					else if (option_state_which==option_state_normal_lianfa)
					{
						option_state_which=option_state_hard_lianfa;
					}
					else if (option_state_which==option_state_hard_lianfa)
					{
						option_state_which=option_state_eazy_lianfa;
					}
				}
				else if (option_edit_which==edit_lianfa){
					if (option_state_which==option_state_eazy_shoudong)
					{
						option_state_which=option_state_eazy_lianfa;
					}
					else if (option_state_which==option_state_normal_shoudong)
					{
						option_state_which=option_state_normal_lianfa;
					}
					else if (option_state_which==option_state_hard_shoudong)
					{
						option_state_which=option_state_hard_lianfa;
					}
					else if (option_state_which==option_state_eazy_lianfa)
					{
						option_state_which=option_state_eazy_shoudong;
					}
					else if (option_state_which==option_state_normal_lianfa)
					{
						option_state_which=option_state_normal_shoudong;
					}
					else if (option_state_which==option_state_hard_lianfa)
					{
						option_state_which=option_state_hard_shoudong;
					}
					
				}
			}
#endif
			break;
		}
		case VK_RETURN:{
			if(COPYRIGHT==1){//如果进入了游戏画面
				/*if(START_SELECT==0){
					MessageBox(hwnd,"暂停","pause",MB_OK);
				}
				else if(START_SELECT==1){
					MessageBox(hwnd,"暂停1","pause",MB_OK);
				}*/
				if(pause!=1)
					pause=1;
				else
					pause=0;
			}
			else if (COPYRIGHT==0){
				DSound_Play(sound_ids[7]);
				if (whichopt==startgameopt)
				{
					COPYRIGHT=1;
				}
				else if(whichopt==exitopt){
					//keybd_event(VK_ESCAPE,0,0,0);
					exit(0);
				}
				else if(whichopt==staffopt){
					if (enter_staff_bg)
					{
						enter_staff_bg=0;
					}
					else{
						enter_staff_bg=1;
					}
					
				}
				else if(whichopt==optionopt){
					if (enter_option_bg)
					{
						enter_option_bg=0;
					}
					else{
						enter_option_bg=1;
					}
					
				}
			}
					   }break;
		default:break;
		}
					}break;
	case WM_LBUTTONDBLCLK:{//鼠标双击事件
		int mouse_x=(int)LOWORD(lparam);int mouse_y=(int)HIWORD(lparam);
		//char buf2[50];改成全局变量
		sprintf(buf2,"你双击了!坐标是X:%d Y:%d",mouse_x,mouse_y);
		//MessageBox(hwnd,buf2,"hehe",MB_OK);
						  }break;
	

	case WM_DESTROY: 
		{
		// kill the application, this sends a WM_QUIT message 
		PostQuitMessage(0);

        // return success
		return(0);
		} break;

	default:break;

    } // end switch

// process any messages that we didn't take care of 
return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc











/*
 * Description:		Creates a DirectDraw surface or restores an existing one.
 *
 * Parameters:		lplpDDS   - Address of a pointer to a DirectDraw surface. If the
 *							    pointer contains NULL, the surface will be newly created,
 *						        otherwise the surface it points at will be restored.
 *
 *					lpSize    - Pointer to a size structure that contains the desired size
 *						        of the surface. If *lplpDDS is non-zero, this parameter 
 *							    will be ignored.
 *
 *					gSysMem   - If true, the surface will be created in system memory.
 *
 * Return value:	At the moment the function always returns 0.
 *
 */
int CreateDDSurface( LPDIRECTDRAWSURFACE7* lplpDDS, LPSIZE lpSize, bool gSysMem )
{
	DDSURFACEDESC2	ddsd;


	// If the pointer referenced by lplpDDS is not pointing at NULL,
	// the surface it points at shall be restored.
	if ( *lplpDDS != NULL )
	{
		//
		// Restore the surface in case it is lost.
		//
		if ( ( *lplpDDS )->IsLost() == DDERR_SURFACELOST )
			( *lplpDDS )->Restore();
	}
	// If the pointer referenced by lplpDDS is pointing at NULL,
	// the surface shall be newly created.
	else
	{
		//
		// Initialize the surface description. A color key is always used.
		//
		ZeroMemory( &ddsd, sizeof ddsd );
		ddsd.dwSize = sizeof ddsd;	
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_CKSRCBLT; 
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;    
		ddsd.dwWidth  = lpSize->cx;
		ddsd.dwHeight = lpSize->cy;
		ddsd.ddckCKSrcBlt.dwColorSpaceHighValue = 0;
		ddsd.ddckCKSrcBlt.dwColorSpaceLowValue = 0;

		//
		// If desired, arrange for the surface to get into system memory
		//
		if ( gSysMem )
			ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

		// Create the surface.
		lpdd->CreateSurface( &ddsd, lplpDDS, NULL );
	}

	return 0;
}

LPDIRECTDRAWSURFACE7 reloadmap(LPDIRECTDRAW7 lpdd,int stagenum){
LPDIRECTDRAWSURFACE7 lpdds;
	if (stagenum==0){
		Init_Map(stagenum);
		//lpdds=DDLoadBitmap(lpdd,"Resource/bmp/map.bmp");
		JPEGINFO f;
		lpdds=NULL;
		LoadJPEG(FROM_FILE,"Resource/bmp/map.jpg",&lpdds,lpdd,f);
	}
	else if (stagenum==1){
		Init_Map(stagenum);
		//lpdds=DDLoadBitmap(lpdd,"Resource/bmp/map1.bmp");
		JPEGINFO f1;
		lpdds=NULL;
		LoadJPEG(FROM_FILE,"Resource/bmp/map1.jpg",&lpdds,lpdd,f1);
	}
	return lpdds;
}
