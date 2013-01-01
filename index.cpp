// DEMO2_3.CPP - A complete windows program
//����CVS
// INCLUDES ///////////////////////////////////////////////
#define GAME_NAME "��ҵ���-C++���������ϷDEMO"
#define WIN32_LEAN_AND_MEAN  // just say no to MFC

//#define USE_GAMEDEV_TILE_1//����gamedev��tile in directx��TILE����(��������ȽϺ�)
#define USE_GAMEDEV_TILE_2//����gamedev��Game Programming GenesisPart VIII : Basic Tile Engines ��TILE����
#define DDRAW_INIT_STRUCT(ddstruct) {memset(&ddstruct,0,sizeof(ddstruct));ddstruct.dwSize=sizeof(ddstruct);}
// this builds a 16 bit color value in 5.6.5 format (green dominate mode)
#include <windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#include <stdio.h>
#include <math.h>
#include <ddraw.h>
#include <io.h>//_lseek Ҫ������
#include <iostream>//����û��H�����޷���std�ģ�
using namespace std;
#include "winGMP.h"//windows��Ϸ��̴�ʦ�ϵ�һЩ�ͼ�����������������������棩
#include "myengine.h"//DX�;���Ľṹ����
#include "function.h"
#include "jpeglib.h"
#ifdef USE_GAMEDEV_TILE_1
#include "map.h"//��ͼ���ݱ�����tile����
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
//����һ�����Ա�ǣ���BltҪ��ҪRect������������
//7.6������ĵ�ɫ�����Σ���FLIP��
//7.7�����ز��У�FLIP��//���߻�û��
//7.8��������У���λͼ�ϵĶ������е����棬��FLIP��
//7.9��DDCLIPPER(���ߵ�����û��FLIP���ҵ���)
//7.10�Ǽ���һ��8λɫλͼ
//�Ƿ��Ƽ�ʹ��7.8?�Ǹ����⣬ӦΪ�����ǻ���˫����ģ��Ǹ߼�ָ��lpsurface��mempitchֱ�Ӳ���
//####################
//1��7.6������
//2��7.7������
//3��ͬʱ7.6��7.7������
//4��7.6��7.7��7.8��7.9������
//5��7.10�����ӣ�����û����FLIP,����ס�������������ddsd.lpSurface����������bitmap��buffer��Ȼ��unlock��Unload_bitmap
//,����attach surface˫����֮��FLIP����ס�󱸻���֮�󣬲���ddsd.lpSurface,��unlock��Unload_bitmap
//##################################################
#define ECHODEBUG 0//��Ҫ��ʾCOM���󴴽��ɹ���Ϣ
#define DEBUG 6

// defines for windows 
#define WINDOW_CLASS_NAME "WINCLASS1"



#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)


// GLOBALS ////////////////////////////////////////////////
//����������͸������棨2�����壩
/** ToDo:������ȫ�ֱ��������ⶼ�Ż���û��ȫ�ֱ����ķ�ʽ
*/

HWND globalhwnd=NULL;//ȫ�ִ��ھ��
HINSTANCE main_instance=NULL; //��ʵ�����
LPDIRECTDRAWPALETTE globallpddpal;//ȫ�ֵ�ɫ��(��ν��ȫ�ֵ�ɫ����ʵ����Ϊ�˷�װ��gameinit��������Ե����������Ż�,����Ϸ���ȫ�Ӵ��ｲ������)
LPDIRECTDRAWSURFACE7 lpddsprimary=NULL;
LPDIRECTDRAWSURFACE7 lpddsback=NULL;
LPDIRECTDRAWSURFACE7 lpddsbackground=NULL;
LPDIRECTDRAWSURFACE7 lpddswork=NULL;
//LPDIRECTDRAWSURFACE7 lpddswork2=NULL;

DDSURFACEDESC2 ddsd;
LPDIRECTDRAW7 lpdd=NULL;

LPDIRECTDRAWCLIPPER   lpddclipper  = NULL;   // dd clipper
DDBLTFX ddbltfx;//��Ҫ�ṹ
//BITMAP_FILE bitmap,bitmapsp,bitmapemy,bitmapmenu,bitmapcopyright;
BITMAP_FILE bitmap,bitmapsp,bitmapemy;
PALETTEENTRY          palette[256];          // color palette
//LPDIRECTDRAWPALETTE lpddpal=NULL;
char buf2[50];
int mytransform;//ȫ����������BLTλ�ñ仯����
int COPYRIGHT;//һ��ʼ��������˾��Ȩ���棬0��δ���֣�1���Ѿ����ֹ���
int START_SELECT; //���˵�ѡ����ʲô��0�ǾɵĻ��䣬1���µ���Ϸ,-1����δ����

RECT rcRect;

static int offsety,offsetx;//�������ǵ�ƫ��
int pause;//�Ƿ���ͣ���
int g_iFireInputDelay;//���������ӵ����Ʒ���
int g_iMissileInputDelay;//�������ǵ������Ʒ���
DWORD dwStart;
const int timelimit=3000;
int mysm;//��״̬��ʵ��

static int current_fps;//��ǰ��fps
static int max_fps;//���fps
int already_record_max_fps_time;//��һ��ʱ���¼������fps
int key_press_counter;//������ʱ����������ֹ������ǿ�Ʋ����~
int enter_staff_bg;//���빤����Ա�������
int enter_option_bg;//����ѡ������
int stage_num;//�������
int sound_ids[20]; // storage for 4 sound fx ids
//ȫ�־�������
list <cls_sprite *> sprite_list;
list <cls_sprite *> sprite_list2;
list <cls_sprite *>::iterator sprite_list_iter,temp_sprite_list_iter;
//ȫ�ֵл��ӵ�����
//list <bullet *> enemy_all_bullet_list;
//list <bullet *>::iterator enemy_all_bullet_list_iter,temp_enemy_all_bullet_list_iter;
//ȫ�ֻ�׹��
int hits;
//��Ϸ��ʼʱ���ѡ��
int whichopt;
//��ʼ�����ĻԹ������
int shinecounter;
//boss1�������(�����л���ͼ)
int boss1die;
//boss2�������
int boss2die;
//��ťѡ��
typedef enum optx{
	startgameopt=1,
	optionopt=2,
	staffopt=3,
	exitopt=4
};
int option_state_which;//��¼��Ϸ����ѡ���ѡ��
//����ѡ����״̬
typedef enum enter_option_state{
option_state_eazy_shoudong=0,
option_state_normal_shoudong=1,
option_state_hard_shoudong=2,
option_state_eazy_lianfa=3,
option_state_normal_lianfa=4,
option_state_hard_lianfa=5
};
int option_edit_which;//��¼��λ��ѡ��������ѶȻ�������
typedef enum enter_option_edit_which{
edit_difficult=0,
edit_lianfa=1
};
int bossstatecounter;//boss״̬������(Todo,�����ü�״̬��)
//prototype
int gameinit();
LPDIRECTDRAWSURFACE7 reloadmap(LPDIRECTDRAW7,int);//���ص�ͼ

void mybltclip(HWND hwnd,USHORT * lpsurface,int mempitch);//һ�������ü�����
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
LPDIRECTDRAWSURFACE7 mysprite;//����һ������
LPDIRECTDRAWSURFACE7 gamemenu;//������Ϸ���˵�
HFONT 		g_hFont;//��ʾFPS����
HFONT		status_info_hFont;//��ʾ״̬��Ϣ����

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
gameinit();//��ʼ����Ϸ
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
//Todo:��GAMEDEV_TILE_2��ͼƬ



#if 1
if(!(mysprite=Create_Offscreen_Surface(480,480,DDSCAPS_SYSTEMMEMORY))){
	//echo error
	MessageBox(hwnd,"������������spriteʧ��","��Ϣ",MB_OK);
}
else{
	//MessageBox(hwnd,"������������Ok","��Ϣ",MB_OK);
}
#endif
//�����˵�����
if(!(gamemenu=Create_Offscreen_Surface(480,480,DDSCAPS_SYSTEMMEMORY))){
	//echo error
	MessageBox(hwnd,"��������������Ϸ�˵�ʧ��","��Ϣ",MB_OK);
}

/*
#if 0
if(!(mysprite = DDraw_Create_Surface(240,480,0,-1))){
	MessageBox(hwnd,"��������ʧ��","info",MB_OK);
}
else{
	//MessageBox(hwnd,"��������OK","info",MB_OK);
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
	//����ʼbitmap
	//if(!Load_Bitmap_File(hwnd,&bitmapcopyright,"Resource/bmp/start.bmp")){
	//		MessageBox(hwnd,"��ȡͼ��ERROR","��Ϣ",MB_OK);
	//}
	//������bitmap
	if(Load_Bitmap_File(hwnd,&bitmapsp,"Resource/bmp/bitmap8_1.bmp")){
		if(ECHODEBUG==1)
			MessageBox(hwnd,"��ȡͼ��OK","��Ϣ",MB_OK);
	}
	//����ͼbitmap
	//if(Load_Bitmap_File(hwnd,&bitmap,"Resource/bmp/testbitmap.bmp")){
	//	if(ECHODEBUG==1)
	//		MessageBox(hwnd,"��ȡͼ��OK","��Ϣ",MB_OK);
	//}
	//���˵�bitmap
	//if(FAILED(Load_Bitmap_File(hwnd,&bitmapmenu,"Resource/bmp/gamemenu.bmp"))){
	//		MessageBox(hwnd,"��ȡ��Ϸ���˵�ʧ��","��Ϣ",MB_OK);
	//}
/*	//���˵�bitmap
	if(FAILED(Load_Bitmap_File(hwnd,&bitmapemy,"soldier_sm.bmp"))){
			MessageBox(hwnd,"��ȡ��Ϸ���˵�ʧ��","��Ϣ",MB_OK);
	}*/

	//Ϊlpddsbackgroundɨ��λͼbitmap8
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

	//ΪlpddsTilesetɨ��λͼbitmap8,ԭ����ɨ��ͼƬ������ķ���
#if 0
	
#ifdef USE_GAMEDEV_TILE_2
//lpddsBack = DDraw_Create_Surface(640,480,0,1);
lpddsTileset = DDraw_Create_Surface(640,480,0,1);//�������FAILED(lpdd7->CreateSurface(&ddsd, &lpddsTileset, NULL))����
//������������һ��32*384�������ͼƬ����������Ķ��뻭�溯����������ģ�ֻ�ܶ�640*480�Ķ�����
//ToDo:�������������װ�ɿ��Լ��ص������С�Ļ��棡
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
	
#if 0//�µ�GDIЭ������V1��δ��������256ɫͼƬ

lpddsTileset = DDraw_Create_Surface(6400,480,0,1);
//lpddsTileset->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
//���������������Ҫ�����������Լ���װ�ˣ����⣡
//The GetDC method creates a GDI-compatible device context for the surface. 
//It uses an internal version of the Lock method to lock the surface,
// and the surface will remain locked until ReleaseDC is called.
// See the description of the Lock method for more information on this behavior.
if (!Load_Bitmap256("Resource/bmp/map.bmp",0,0,lpddsTileset))
MessageBox(hwnd,"����λͼʧ�ܣ�","",NULL);
//lpddsTileset->Unlock(NULL);
#endif
#if 0
lpddsTileset = DDraw_Create_Surface(1298,1446,0,1);
if (!Loadbmp2Surface("big.bmp",0,0,lpddsTileset))
MessageBox(hwnd,"����λͼʧ�ܣ�","",NULL);
#endif

#if 1//���ڿ�ʼʹ�����������
//from gamedev new test
//lpddsTileset = DDraw_Create_Surface(6400,480,0,1);
//lpddsTileset = CreateOffScreenSurface(lpdd,640,480);



//lpddsTileset=DDLoadBitmap(lpdd,"Resource/bmp/map.bmp");
//lpddsTileset=DDLoadBitmap(lpdd,"Resource/bmp/map1.bmp");
lpddsTileset=reloadmap(lpdd,0);

//lpddsTileset=reloadmap(lpdd,0);
#endif

//��ʼ��������
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
//LPDIRECTDRAWSURFACE7 smallboss_surface=NULL;//Todo bug!Ϊʲô����ɾ����䣡
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
//���˼·����һ���زĻ���ֱ���ܹ�����1�龫�飬Ȼ��������龫��
#if 0
girl_swordman_surface_test=sfobj->getSpriteSurface(girl_swordman_surface_test,"��ɫŮ��ʿ�ز�",\
"����Ů��ʿ","Resource/bmp/swordwoman.bmp",4);
//��������Ƕ���ĵ��ã���������ָ��
girl_swordman_surface_test1=sfobj->getSpriteSurface(girl_swordman_surface_test1,"��ɫŮ��ʿ�ز�",\
"����Ů��ʿ","Resource/bmp/swordwoman.bmp",4);
girl_swordman_surface_test2=sfobj->getSpriteSurface(girl_swordman_surface_test2,"��ɫŮ��ʿ�ز�",\
"����Ů��ʿ","Resource/bmp/swordwoman2.bmp",4);
girl_swordman_surface_red=sfobj->getSpriteSurface(girl_swordman_surface_red,"��ɫŮ��ʿ�ز�",\
"����Ů��ʿ","Resource/bmp/swordwomanred.bmp",4);
#endif
int player_seqnum=6;
player_plane=sfobj->getSpriteSurface(player_plane,"��ҷɻ��ز�","��ҷɻ�","Resource/bmp/playerplane.bmp",player_seqnum);
int player_sfid=friend_cls_sprite_surface_getmax(sfobj);//��ȡ��ǰ�زĻ����������id

//int player_sfid=sfobj->get_max_sprite_surface(1);Ч��һ���ģ�
//cloud_surface=sfobj->getSpriteSurface(cloud_surface,"�����ز�","����","Resource/bmp/cloud.bmp",1);
boss1_surface=sfobj->getSpriteSurface(boss1_surface,"��ɫ��boss�ز�","��ɫ��boss","Resource/bmp/boss1.bmp",6);
int boss1_sfid=sfobj->get_max_sprite_surface(0);

//boss1shadow_surface=sfobj->getSpriteSurface(boss1shadow_surface,"��ɫ��bossӰ���ز�","��ɫ��bossӰ��","Resource/bmp/boss1_shadow.bmp",1);
//int boss1_shadow_sfid=sfobj->get_max_sprite_surface(0);

bullet0_surface=sfobj->getSpriteSurface(bullet0_surface,"�ȼ�1�ӵ��ز�","�ȼ�1�ӵ�","Resource/bmp/bullet0.bmp",1);
int bullet0_sfid=sfobj->get_max_sprite_surface(0);
bullet1_surface=sfobj->getSpriteSurface(bullet1_surface,"�ȼ�2�ӵ��ز�","�ȼ�2�ӵ�","Resource/bmp/bullet1.bmp",1);
int bullet1_sfid=sfobj->get_max_sprite_surface(0);
bullet2_surface=sfobj->getSpriteSurface(bullet2_surface,"�ȼ�3�ӵ��ز�","�ȼ�3�ӵ�","Resource/bmp/bullet2.bmp",1);
int bullet2_sfid=sfobj->get_max_sprite_surface(0);
bullet3_surface=sfobj->getSpriteSurface(bullet3_surface,"�ȼ�4�ӵ��ز�","�ȼ�4�ӵ�","Resource/bmp/bullet3.bmp",1);
int bullet3_sfid=sfobj->get_max_sprite_surface(0);
sfobj->getSpriteSurface(e_bullet_surface,"�л��ӵ��ز�","�л��ӵ�","Resource/bmp/e_bullet.bmp",1);
int e_bullet_sfid=sfobj->get_max_sprite_surface(0);
sfobj->getSpriteSurface(big_e_bullet_surface,"��л��ӵ��ز�","��л��ӵ�","Resource/bmp/big_e_bullet.bmp",6);
int big_e_bullet_sfid=sfobj->get_max_sprite_surface(0);
sfobj->getSpriteSurface(soft_e_bullet_surface,"�ɴ���л��ӵ��ز�","�ɴ���л��ӵ�","Resource/bmp/soft_e_bullet.bmp",6);
int soft_e_bullet_sfid=sfobj->get_max_sprite_surface(0);

enemy0_surface=sfobj->getSpriteSurface(enemy0_surface,"�л�0�ز�","�л�0","Resource/bmp/enemy0.bmp",6);
int enemy0_sfid=sfobj->get_max_sprite_surface(0);
//alertme("������");
enemy1_surface=sfobj->getSpriteSurface(enemy1_surface,"�л�1�ز�","�л�1","Resource/bmp/enemy1.bmp",6);
int enemy1_sfid=sfobj->get_max_sprite_surface(0);
enemy2_surface=sfobj->getSpriteSurface(enemy2_surface,"�л�2�ز�","�л�2","Resource/bmp/enemy2.bmp",6);
int enemy2_sfid=sfobj->get_max_sprite_surface(0);
enemy3_surface=sfobj->getSpriteSurface(enemy3_surface,"�л�3�ز�","�л�3","Resource/bmp/enemy3.bmp",6);
int enemy3_sfid=sfobj->get_max_sprite_surface(0);
enemy3_red_surface=sfobj->getSpriteSurface(enemy3_red_surface,"�л�3���ز�","�л�3��","Resource/bmp/enemy3_red.bmp",6);
int enemy3_red_sfid=sfobj->get_max_sprite_surface(0);

enemy4_surface=sfobj->getSpriteSurface(enemy4_surface,"�л�4�ز�","�л�4","Resource/bmp/enemy4.bmp",6);
int enemy4_sfid=sfobj->get_max_sprite_surface(0);

expl_surface=sfobj->getSpriteSurface(expl_surface,"��ը�ز�","��ը","Resource/bmp/expl.bmp",14,false);
int expl_sfid=sfobj->get_max_sprite_surface(0);


bossspark_surface=sfobj->getSpriteSurface(bossspark_surface,"boss���ز�","boss��","Resource/bmp/bossspark.bmp",2);
int bossspark_sfid=sfobj->get_max_sprite_surface(0);
boss2spark_surface=sfobj->getSpriteSurface(boss2spark_surface,"boss2���ز�","boss2��","Resource/bmp/boss2spark.bmp",2);
int boss2spark_sfid=sfobj->get_max_sprite_surface(0);
spark_surface=sfobj->getSpriteSurface(spark_surface,"���ز�","��","Resource/bmp/spark.bmp",2);
int spark_sfid=sfobj->get_max_sprite_surface(0);

bigexpl_surface=sfobj->getSpriteSurface(bigexpl_surface,"��ը�ز�","��ը","Resource/bmp/bigexpl.bmp",28);
int bigexpl_sfid=sfobj->get_max_sprite_surface(0);
//leftbar_surface=sfobj->getSpriteSurface(leftbar_surface,"�������ز�","������","Resource/bmp/leftbar.bmp",1);
JPEGINFO jpgf;
LoadJPEG(FROM_FILE,"Resource/bmp/leftbar.jpg",&leftbar_surface,lpdd,jpgf);
//MessageBeep(MB_ICONASTERISK);
//int leftbar_sfid=sfobj->get_max_sprite_surface(0);
//rightbar_surface=sfobj->getSpriteSurface(rightbar_surface,"�Ҳ�����ز�","�Ҳ����","Resource/bmp/rightbar.bmp",1);
//int rightbar_sfid=sfobj->get_max_sprite_surface(0);
LoadJPEG(FROM_FILE,"Resource/bmp/rightbar.jpg",&rightbar_surface,lpdd,jpgf);
mainloginbg_surface=sfobj->getSpriteSurface(mainloginbg_surface,"����½�����ز�","����½����","Resource/bmp/startbg.bmp",9);
int mainloginbg_sfid=sfobj->get_max_sprite_surface(0);
pause_surface=sfobj->getSpriteSurface(pause_surface,"��ͣ�ز�","��ͣ","Resource/bmp/pause.bmp",1);
int pause_sfid=sfobj->get_max_sprite_surface(0);
//smallboss_surface=sfobj->getSpriteSurface(smallboss_surface,"Сboss�ز�","Сboss","Resource/bmp/smallboss.bmp",2);
//int smallboss_sfid=sfobj->get_max_sprite_surface(0);
ro_bullet_surface=sfobj->getSpriteSurface(ro_bullet_surface,"��ת�ӵ��ز�","��ת�ӵ�","Resource/bmp/ro_bullet.bmp",12);
int ro_bullet_sfid=sfobj->get_max_sprite_surface(0);
gameover_surface=sfobj->getSpriteSurface(gameover_surface,"gameover�ز�","gameover","Resource/bmp/gameover.bmp",1);
int gameover_sfid=sfobj->get_max_sprite_surface(0);
mission1_surface=sfobj->getSpriteSurface(mission1_surface,"mission1�ز�","mission1","Resource/bmp/mission1.bmp",1);
int mission1_sfid=sfobj->get_max_sprite_surface(0);

mission2_surface=sfobj->getSpriteSurface(mission2_surface,"mission2�ز�","mission2","Resource/bmp/mission2.bmp",1);
int mission2_sfid=sfobj->get_max_sprite_surface(0);

tank_surface=sfobj->getSpriteSurface(tank_surface,"tank�ز�","tank","Resource/bmp/tank.bmp",8);
int tank_sfid=sfobj->get_max_sprite_surface(0);
normal_e_bullet_surface=sfobj->getSpriteSurface(normal_e_bullet_surface,"��ͨ��ת�ӵ��ز�","��ͨ��ת�ӵ�","Resource/bmp/normal_e_bullet.bmp",8);
int normal_e_bullet_sfid=sfobj->get_max_sprite_surface(0);
tank_e_bullet_surface=sfobj->getSpriteSurface(tank_e_bullet_surface,"̹���ӵ��ز�","̹���ӵ�","Resource/bmp/tank_e_bullet.bmp",8);
int tank_e_bullet_sfid=sfobj->get_max_sprite_surface(0);
//alphabg_surface=sfobj->getSpriteSurface(alphabg_surface,"alpha�����ز�","alpha����","Resource/bmp/alphabg.bmp",1);
alphabg_surface=sfobj->getSpriteSurfaceJPG(alphabg_surface,"alpha�����ز�","alpha����","Resource/bmp/alphabg.jpg",1);
int alphabg_sfid=sfobj->get_max_sprite_surface(0);
//alphabg_pause_surface=sfobj->getSpriteSurfaceJPG(alphabg_pause_surface,"alpha��ͣ�����ز�","alpha��ͣ����","Resource/bmp/alphabg_pause.jpg",1);
//int alphabg_pause_sfid=sfobj->get_max_sprite_surface(0);
power_surface=sfobj->getSpriteSurface(power_surface,"power�����ز�","power����","Resource/bmp/power.bmp",2);
int power_sfid=sfobj->get_max_sprite_surface(0);
//staff_surface=sfobj->getSpriteSurface(staff_surface,"staff�ز�","staff�ز�","Resource/bmp/staff.bmp",1);
staff_surface=sfobj->getSpriteSurfaceJPG(staff_surface,"staff�ز�","staff�ز�","Resource/bmp/staff.jpg",1);
int staff_sfid=sfobj->get_max_sprite_surface(0);
//option_surface=sfobj->getSpriteSurface(option_surface,"option�ز�","option�ز�","Resource/bmp/option.bmp",1);
option_surface=sfobj->getSpriteSurfaceJPG(option_surface,"option�ز�","option�ز�","Resource/bmp/option.jpg",1);
//LPDIRECTDRAWSURFACE7 ppps=NULL;
///ppps=DDLoadBitmap(lpdd,"Resource/bmp/product.bmp");
//LPDIRECTDRAWSURFACE7 girl_swordman_surface;
//	girl_swordman_surface=DDLoadBitmap(lpdd,"Resource/bmp/bitmap8_1.bmp");
//JPEGINFO ji;
//LoadJPEG(FROM_FILE,"Resource/bmp/example1s.jpg",&ppps,lpdd,ji);

int option_sfid=sfobj->get_max_sprite_surface(0);
product_surface=sfobj->getSpriteSurface(product_surface,"��Ʒ�ز�","��Ʒ�ز�","Resource/bmp/product.bmp",1);
int product_sfid=sfobj->get_max_sprite_surface(0);
underline_surface=sfobj->getSpriteSurface(underline_surface,"�»����ز�","�»����ز�","Resource/bmp/underline.bmp",1);
int underline_sfid=sfobj->get_max_sprite_surface(0);
opt1_surface=sfobj->getSpriteSurface(opt1_surface,"�˵���1�ز�","�˵���1�ز�","Resource/bmp/opt1.bmp",1);
int opt1_sfid=sfobj->get_max_sprite_surface(0);
opt2_surface=sfobj->getSpriteSurface(opt2_surface,"�˵���2�ز�","�˵���2�ز�","Resource/bmp/opt2.bmp",1);
int opt2_sfid=sfobj->get_max_sprite_surface(0);
opt3_surface=sfobj->getSpriteSurface(opt3_surface,"�˵���3�ز�","�˵���3�ز�","Resource/bmp/opt3.bmp",1);
int opt3_sfid=sfobj->get_max_sprite_surface(0);
opt4_surface=sfobj->getSpriteSurface(opt4_surface,"�˵���4�ز�","�˵���4�ز�","Resource/bmp/opt4.bmp",1);
int opt4_sfid=sfobj->get_max_sprite_surface(0);
option_difficult_surface=sfobj->getSpriteSurface(option_difficult_surface,"�Ѷ�ѡ���ز�","�Ѷ��ز�","Resource/bmp/option_difficult.bmp",1);
int option_difficult_sfid=sfobj->get_max_sprite_surface(0);
option_repeatfire_surface=sfobj->getSpriteSurface(option_repeatfire_surface,"����ѡ���ز�","�����ز�","Resource/bmp/option_repeatfire.bmp",1);
int option_repeatfire_sfid=sfobj->get_max_sprite_surface(0);
missile_surface=sfobj->getSpriteSurface(missile_surface,"�����ز�","�����ز�","Resource/bmp/missile.bmp",16);
int missile_sfid=sfobj->get_max_sprite_surface(0);
missile_surface0=sfobj->getSpriteSurface(missile_surface0,"����0�ز�","����0�ز�","Resource/bmp/missile0.bmp",1);
int missile0_sfid=sfobj->get_max_sprite_surface(0);
missile_surface1=sfobj->getSpriteSurface(missile_surface1,"����1�ز�","����1�ز�","Resource/bmp/missile1.bmp",1);
int missile1_sfid=sfobj->get_max_sprite_surface(0);
missile_surface2=sfobj->getSpriteSurface(missile_surface2,"����2�ز�","����2�ز�","Resource/bmp/missile2.bmp",1);
int missile2_sfid=sfobj->get_max_sprite_surface(0);
missile_surface3=sfobj->getSpriteSurface(missile_surface3,"����3�ز�","����3�ز�","Resource/bmp/missile3.bmp",1);
int missile3_sfid=sfobj->get_max_sprite_surface(0);
missile_surface4=sfobj->getSpriteSurface(missile_surface4,"����4�ز�","����4�ز�","Resource/bmp/missile4.bmp",1);
int missile4_sfid=sfobj->get_max_sprite_surface(0);
missile_surface5=sfobj->getSpriteSurface(missile_surface5,"����5�ز�","����5�ز�","Resource/bmp/missile5.bmp",1);
int missile5_sfid=sfobj->get_max_sprite_surface(0);
missile_surface6=sfobj->getSpriteSurface(missile_surface6,"����6�ز�","����6�ز�","Resource/bmp/missile6.bmp",1);
int missile6_sfid=sfobj->get_max_sprite_surface(0);
missile_surface7=sfobj->getSpriteSurface(missile_surface7,"����7�ز�","����7�ز�","Resource/bmp/missile7.bmp",1);
int missile7_sfid=sfobj->get_max_sprite_surface(0);
missile_surface8=sfobj->getSpriteSurface(missile_surface8,"����8�ز�","����8�ز�","Resource/bmp/missile8.bmp",1);
int missile8_sfid=sfobj->get_max_sprite_surface(0);
missile_surface9=sfobj->getSpriteSurface(missile_surface9,"����9�ز�","����9�ز�","Resource/bmp/missile9.bmp",1);
int missile9_sfid=sfobj->get_max_sprite_surface(0);
missile_surface10=sfobj->getSpriteSurface(missile_surface10,"����10�ز�","����10�ز�","Resource/bmp/missile10.bmp",1);
int missile10_sfid=sfobj->get_max_sprite_surface(0);
missile_surface11=sfobj->getSpriteSurface(missile_surface11,"����11�ز�","����11�ز�","Resource/bmp/missile11.bmp",1);
int missile11_sfid=sfobj->get_max_sprite_surface(0);
missile_surface12=sfobj->getSpriteSurface(missile_surface12,"����12�ز�","����12�ز�","Resource/bmp/missile12.bmp",1);
int missile12_sfid=sfobj->get_max_sprite_surface(0);
missile_surface13=sfobj->getSpriteSurface(missile_surface13,"����13�ز�","����13�ز�","Resource/bmp/missile13.bmp",1);
int missile13_sfid=sfobj->get_max_sprite_surface(0);
missile_surface14=sfobj->getSpriteSurface(missile_surface14,"����14�ز�","����14�ز�","Resource/bmp/missile14.bmp",1);
int missile14_sfid=sfobj->get_max_sprite_surface(0);
missile_surface15=sfobj->getSpriteSurface(missile_surface15,"����15�ز�","����15�ز�","Resource/bmp/missile15.bmp",1);
int missile15_sfid=sfobj->get_max_sprite_surface(0);
option_eazy_surface=sfobj->getSpriteSurface(option_eazy_surface,"ѡ����ز�","ѡ����ز�","Resource/bmp/option_eazy.bmp",1);
int option_eazy_sfid=sfobj->get_max_sprite_surface(0);
option_eazy1_surface=sfobj->getSpriteSurface(option_eazy1_surface,"ѡ���1�ز�","ѡ���1�ز�","Resource/bmp/option_eazy1.bmp",1);
int option_eazy1_sfid=sfobj->get_max_sprite_surface(0);
option_normal_surface=sfobj->getSpriteSurface(option_normal_surface,"ѡ��һ���ز�","ѡ��һ���ز�","Resource/bmp/option_normal.bmp",1);
int option_normal_sfid=sfobj->get_max_sprite_surface(0);
option_normal1_surface=sfobj->getSpriteSurface(option_normal1_surface,"ѡ��һ��1�ز�","ѡ��һ��1�ز�","Resource/bmp/option_normal1.bmp",1);
int option_normal1_sfid=sfobj->get_max_sprite_surface(0);
option_hard_surface=sfobj->getSpriteSurface(option_hard_surface,"ѡ�������ز�","ѡ�������ز�","Resource/bmp/option_hard.bmp",1);
int option_hard_sfid=sfobj->get_max_sprite_surface(0);
option_hard1_surface=sfobj->getSpriteSurface(option_hard1_surface,"ѡ������1�ز�","ѡ������1�ز�","Resource/bmp/option_hard1.bmp",1);
int option_hard1_sfid=sfobj->get_max_sprite_surface(0);
option_on_surface=sfobj->getSpriteSurface(option_on_surface,"ѡ����ز�","ѡ����ز�","Resource/bmp/option_on.bmp",1);
int option_on_sfid=sfobj->get_max_sprite_surface(0);
option_on1_surface=sfobj->getSpriteSurface(option_on1_surface,"ѡ���1�ز�","ѡ���1�ز�","Resource/bmp/option_on1.bmp",1);
int option_on1_sfid=sfobj->get_max_sprite_surface(0);
option_off_surface=sfobj->getSpriteSurface(option_off_surface,"ѡ��ر��ز�","ѡ��ر��ز�","Resource/bmp/option_off.bmp",1);
int option_off_sfid=sfobj->get_max_sprite_surface(0);
option_off1_surface=sfobj->getSpriteSurface(option_off1_surface,"ѡ��ر�1�ز�","ѡ��ر�1�ز�","Resource/bmp/option_off1.bmp",1);
int option_off1_sfid=sfobj->get_max_sprite_surface(0);
boss2_surface=sfobj->getSpriteSurface(boss2_surface,"boss2�ز�","boss2�ز�","Resource/bmp/boss2.bmp",6);
int boss2_sfid=sfobj->get_max_sprite_surface(0);
//init player
//��ʼ�����ǣ�ս����
cls_player *player=new cls_player();
POINT pt_startpoint;
pt_startpoint.x=300;
pt_startpoint.y=480;
//�������ǳ�ʼλ��
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
//��ʼ����һ���л�����6��
for (int i=0;i<=8;i++){
	enemy_list.push_back((cls_enemy *)new cls_enemy());
//}
//pt_startpoint.x=130;
//pt_startpoint.y=230;

//list<cls_enemy * >::iterator   temp_enemy_iter;//�����滻ɾ������ʱ������
//enemy_iter=enemy_list.begin();
//while(enemy_iter!=enemy_list.end()){
	floatpoint ptf_startpoint;
	ptf_startpoint.x=rand()%640;
	ptf_startpoint.y=rand()%480-480;
	enemy_list.back()->SetPos(ptf_startpoint);
	enemy_list.back()->sprite_surface_id=enemy3_sfid;
	enemy_list.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo��ð��ӵ������к͵л������зֿ�
	enemy_list.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
// 	(*enemy_iter)->sprite_surface_id=enemy4_sfid;
// 	(*enemy_iter)->SetAnimateSequence("0,1");
// 	(*enemy_iter)->SetAnimateData("167|152,167|152");
	enemy_list.back()->current_frame=(rand()%1);
	enemy_list.back()->SetVelocity(4);
	enemy_list.back()->SetAiState(1);
	enemy_list.back()->iFireInputDelay=23;
	if (i==4)//��һ������
	{
		enemy_list.back()->sprite_surface_id=enemy3_red_sfid;
		enum bonus bs=power;//��enum������Ӧc��������
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
//��ըЧ��
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

//��ըЧ��
cls_sprite *bigexpl=new cls_sprite();

//��Ч��
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

//boss��Ч��
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

//boss2��Ч��
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
//ai->Set_AI_File_Dir("Script");//Todo:����һ��������·����ʵ��

ai->Open_AI_File("Script/enemy_move.lua");


cls_ai * boss1ai=new cls_ai();
boss1ai->Open_AI_File("Script/boss1_move.lua");

cls_ai * powerai=new cls_ai();
powerai->Open_AI_File("Script/power_move.lua");
//static player_plane_aniseq[]={0,1};//��ս��Ϸ���ǵĶ���2֡

//Ϊmyspriteɨ��λͼbitmap8_1�����ǣ���ʱ��Ů��ʿ��
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


//��ʼ���������������ݽṹ
static girl_swordman_stand_aniseq[]={0,1,2,3};//վ������4֡
static girl_swordman_run_aniseq[]={0,1,2,3,4,5,6,7};//��·����8֡
static girl_swordman_attach_aniseq[]={0,1,2,3,4,5};//��������6֡
static enemy_1_aniseq[]={0,1};
static int player_plane_aniseq[]={0,1,2,3,4,5};//��ս��Ϸ���ǵĶ���2֡
static enemy_plane_aniseq[]={0,1};//һ��л��Ķ���1֡
//static av_boss_aniseq[]={0,1};




//boss2_surface=sfobj->getSpriteSurface(boss2_surface,"AVboss�ز�","AVboss","boss2.bmp",2);
SPRITE_OBJ girl_swordman;//ջ�����Ľṹ�����壡
SPRITE_OBJ enemy_1;

girl_swordman.frames[0]=DDraw_Create_Surface(239,239,3,1);//�������ĸ�����
girl_swordman.frames[1]=DDraw_Create_Surface(239,239,3,1);//�������ĸ�����
girl_swordman.frames[2]=DDraw_Create_Surface(239,239,3,1);//�������ĸ�����
girl_swordman.frames[3]=DDraw_Create_Surface(239,239,3,1);//�������ĸ�����


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

//ΪŮ��ʿ�������ͼƬ
#if 0//windows GMP���Ϸ���
//scan_bitmap����
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
DDRAW_INIT_STRUCT(ddbltfx);//��ʼ��fx�ṹ
ddbltfx.dwFillColor = _RGB16BIT565(0,0,0);//���󱸻��������ɫ
#if 1//�·�����256��16ɫ���ݵ��º������ã���δ��װ��ȡ���麯��
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
	//	MessageBox(hwnd,"ɾ��λͼ����ʧ��","",MB_OK);
	//}
	//free(bmp_info);
#endif

girl_swordman.current_frame=0;

/*Scan_Image_Bitmap(&bitmapemy,enemy_1.frames[0],0,0);

Scan_Image_Bitmap(&bitmapemy,enemy_1.frames[1],1,0);
enemy_1.current_frame=0;*/






//Ϊgamemenuɨ��λͼbitmapmenu
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
//��ʹ��bitmap8�ĵ�ɫ��
//	if (FAILED(globallpddpal->SetEntries(0,0,256,bitmap.palette)))
//	   return(0);


//���Ӻ󱸻��浽������
ddsd.ddsCaps.dwCaps=DDSCAPS_BACKBUFFER;
if(FAILED(lpddsprimary->GetAttachedSurface(&ddsd.ddsCaps,&lpddsback))){
//echo error
	MessageBox(hwnd,"���Ӻ󱸻���ʧ��","��Ϣ",MB_OK);
}
//
//�������������lpSurfaceָ��ֱ�Ӳ�����Ҫ����Lock�󱸻��廭��
	UCHAR happy_bitmap[64] = {0,0,0,0,0,0,0,0,//����һ��Ц�������ַ�����ֱ�Ӳ���������
	0,0,1,1,1,1,0,0,
	0,1,0,1,1,0,1,0,
	0,1,1,1,1,1,1,0,
	1,1,0,1,1,0,1,0,
	0,1,1,0,0,1,1,0,
	0,0,1,1,1,1,0,0,
	0,0,0,0,0,0,0,0};
	typedef struct HAPPY_FACE_TYP//����Ц�����ݽṹ����Ҫ�ǿ�����������
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




//�ͷŶ����Ͻӿ�
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
//Todo ע�⻹��1��tilesets����Ҫ��װ��λͼ��λͼ������������24λ�ģ�
//stage_num=1;
//if (!Init_Map(stage_num))//
//MessageBox(globalhwnd,"��ͼ��ʼ��ʧ��","",MB_OK);
//return(FALSE);

// all set!
#endif
//MessageBox(hwnd,"׼����������Ϸѭ��","caution!!",MB_OK)
Build_Sin_Cos_Tables();
while(1){//��ѭ��
	dwStart = GetTickCount();
	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
		if (msg.message == WM_QUIT)//����Ƿ��˳�
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
	//7.6������2�����Σ�������Bltָ�����򵽺󱸻�������򣬺󱸻������л��������档
	//�Ҳ���ʵʵ����Ϸ��Ӧ�ò�����ô�����ģ�
	static RECT source_rect;
	source_rect.top=70;
	source_rect.bottom=120;
	source_rect.left=100;
	source_rect.right=200;
	//DDRAW_INIT_STRUCT(ddsd);
ddbltfx.dwFillColor = _RGB16BIT565(0,0,0);//���������������ɫ,��������ɫ����
// make the blitter call����
if (FAILED(lpddsback->Blt(NULL, // pointer to dest RECT, NULL for whole thing
                          NULL, // pointer to source surface
                          NULL, // pointer to source RECT
                          DDBLT_COLORFILL | DDBLT_WAIT, 
                          // do a color fill and wait if you have to
                          &ddbltfx))) // pointer to DDBLTFX holding info
{
	MessageBox(hwnd,"�Ծ������󱸻���ʧ��","��Ϣ",MB_OK);
	return(0);
}
else{
	//MessageBox(hwnd,"�Ծ������󱸻���OK","��Ϣ",MB_OK);
}
#if 0
ddbltfx.dwFillColor = _RGB16BIT565(0,120,250);//���������������ɫ
// make the blitter call
if (FAILED(mysprite->Blt(NULL, // pointer to dest RECT, NULL for whole thing
                          NULL, // pointer to source surface
                          NULL, // pointer to source RECT
                          DDBLT_COLORFILL | DDBLT_WAIT, 
                          // do a color fill and wait if you have to
                          &ddbltfx))) // pointer to DDBLTFX holding info
{
	MessageBox(hwnd,"�Ծ��������������ʧ��","��Ϣ",MB_OK);
	return(0);
}
else{
	if (ECHODEBUG==1)
		MessageBox(hwnd,"�Ծ��������������OK","��Ϣ",MB_OK);
}
#endif


#if 0
		//��ס�󱸱���
	if(FAILED(lpddsback->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL))){
	//echo error
	}

	

	HDC xdc;//dx�ļ����豸����������
	if (FAILED(lpddsprimary->GetDC(&xdc))){
	//echo error
		MessageBox(hwnd,"���������ȡ�豸������ʧ��","��Ϣ",MB_OK);
	}
	SetTextColor(xdc,RGB(0,255,0));
	SetBkColor(xdc,RGB(0,0,0));
	SetBkMode(xdc,TRANSPARENT);
	if (DEBUG==1){
		char x[]="Directx��GDI���ʹ�õ�����";
		TextOut(xdc,300,200,(char *)x,strlen(x));
	}
	else if (DEBUG>1 && DEBUG<6){
		char x[]="Directx��flip��GDI���ʹ�õ�����";
		TextOut(xdc,300,200,(char *)x,strlen(x));
	}
	
	lpddsprimary->ReleaseDC(xdc);

	//��һ��8λɫ��仭������
	

	// for now test if user is hitting ESC and send WM_CLOSE
if (KEYDOWN(VK_ESCAPE))
   {
   PostMessage(hwnd,WM_CLOSE,0,0);
   //window_closed = 1;
   } // end if
	
	//ddbltfx.dwFillColor=rand()%256;//ѡ��һ����ɫ
	ddbltfx.dwFillColor = _RGB16BIT565(120, 120, 120);
	//ddbltfx.dwFillColor = 0;



	
mybltclip(hwnd,(USHORT *)ddsd.lpSurface,(int)ddsd.lPitch);//����16bit����

if (DEBUG==4){
			for (int face=0; face < 100; face++){//����bitmap
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
	//��birmap
	if(Load_Bitmap_File(hwnd,&bitmap,"bitmap8.bmp")){
	//MessageBox(hwnd,"��ȡͼ��OK","��Ϣ",MB_OK);
	}

	//��bitmap�ĵ�ɫ��
	// load it's palette into directdraw
	if (FAILED(globallpddpal->SetEntries(0,0,256,bitmap.palette)))
	   return(0);

	//�������ν�Ķ���
	// get video pointer to primary surfce
	UCHAR *primary_buffer = (UCHAR *)ddsd.lpSurface;       

	// test if memory is linear����ڴ��ǲ������Ե�
	if (ddsd.lPitch == 640){
	//	MessageBox(hwnd,"�ڴ������Ե�","��Ϣ",MB_OK);
	   // copy memory from double buffer to primary buffer
	   memcpy((void *)primary_buffer, (void *)bitmap.buffer, 640*480);
	} // end if
	else
	   { 
	//	MessageBox(hwnd,"�ڴ治�����Ե�","��Ϣ",MB_OK);
	   // make copy of source and destination addresses
	   UCHAR *dest_ptr = primary_buffer;
	   UCHAR *src_ptr  = bitmap.buffer;

	   // memory is non-linear, copy line by line
	   for (int y=0; y < 480; y++)
		   {
		   // ���п���
		   memcpy((void *)dest_ptr, (void *)src_ptr, 640);
		   // advance pointers to next line
		   dest_ptr+=ddsd.lPitch;
		   src_ptr +=640;
		   } // end for
	   } // end else
}

//###################################################################



	//�����󱸱���
	if(FAILED(lpddsback->Unlock(NULL))){
	//echo error
	}
	else{
		if(ECHODEBUG==1)
			MessageBox(hwnd,"�����󱸱���OK","��Ϣ",NULL);
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
			MessageBox(hwnd,"Bltʧ��","��Ϣ",NULL);
			return(0);
		}
	}//ebnd debug1
	else if (DEBUG==2){//�������Ӳ�̫���ԣ��������ǿ���
		if (FAILED(lpddsprimary->Blt(&des_rect, // pointer to dest RECT
			lpddsback, // pointer to source surface
			&source_rect, // pointer to source RECT
			DDBLT_WAIT, //�����޷�ʹ��DDBLT_COLORFILL�ˣ�
			// do a color fill and wait if you have to
			&ddbltfx))) // pointer to DDBLTFX holding info
			
		{
			MessageBox(hwnd,"Bltʧ��","��Ϣ",NULL);
			return(0);
		}
	}//end debug2
	else if (DEBUG==3){//7.6��7.7��ϣ���Ч�����������Ҫ������ɫ���Σ�ֻ�а�Blt
		//����DDBLT_COLORFILL�ķŵ���󣬷��򵹹����ᱻ�µ���DDBLT_COLORFILL��BLT��ͬ���򸲸ǵ��ģ�
		if (FAILED(lpddsprimary->Blt(&des_rect, // pointer to dest RECT
			lpddsback, // pointer to source surface
			&source_rect, // pointer to source RECT
			DDBLT_WAIT, //�����޷�ʹ��DDBLT_COLORFILL�ˣ�
			// do a color fill and wait if you have to
			&ddbltfx))) // pointer to DDBLTFX holding info
			
		{
			MessageBox(hwnd,"Bltʧ��","��Ϣ",NULL);
			return(0);
		}//end 7.7
		if (FAILED(lpddsprimary->Blt(&des_rect, // pointer to dest RECT
			NULL, // pointer to source surface
			NULL, // pointer to source RECT
			DDBLT_COLORFILL | DDBLT_WAIT, 
			// do a color fill and wait if you have to
			&ddbltfx))) // pointer to DDBLTFX holding info
			
		{
			MessageBox(hwnd,"Bltʧ��","��Ϣ",NULL);
			return(0);
		}	//end7.6
	}//end debug3
	
	if(DEBUG==4){
		//����DDBLT_COLORFILL�ķŵ���󣬷��򵹹����ᱻ�µ���DDBLT_COLORFILL��BLT��ͬ���򸲸ǵ��ģ�
		if (FAILED(lpddsprimary->Blt(&des_rect, // pointer to dest RECT
			lpddsback, // pointer to source surface
			&source_rect, // pointer to source RECT
			DDBLT_WAIT, //�����޷�ʹ��DDBLT_COLORFILL�ˣ�
			// do a color fill and wait if you have to
			&ddbltfx))) // pointer to DDBLTFX holding info
			
		{
			MessageBox(hwnd,"Bltʧ��","��Ϣ",NULL);
			return(0);
		}//end 7.7
		if (FAILED(lpddsprimary->Blt(&des_rect, // pointer to dest RECT
			NULL, // pointer to source surface
			NULL, // pointer to source RECT
			DDBLT_COLORFILL | DDBLT_WAIT, 
			// do a color fill and wait if you have to
			&ddbltfx))) // pointer to DDBLTFX holding info
			
		{
			MessageBox(hwnd,"Bltʧ��","��Ϣ",NULL);
			return(0);
		}	//end7.6
		
		
		
		//DDraw_Draw_Surface(lpddsbackground,0,0, 640,480, lpddsback,0);    
		//////////////////////////////////////////////////////////////////////////
		//����������Blt���󱸱���
		RECT srcrect,desrect;
		srcrect.left=srcrect.top=0;
		srcrect.right=240-1;
		srcrect.bottom=480-1;
		desrect.left=0;
		desrect.top=0;
		desrect.right=desrect.left+srcrect.right-1;
		desrect.bottom=desrect.top+srcrect.bottom-1;
		if(FAILED(lpddsback->Blt(&desrect,mysprite,&srcrect,(DDBLT_WAIT),NULL))){
			MessageBox(hwnd,"mysprite��������Blt���󱸻���ʧ��","��Ϣ",MB_OK);
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

	if(DEBUG==6){//���ڴ����￪ʼ
static int singleton_num;//�Ժ�ĳɵ���ģʽ
//��ɨ�迪ʼ��������Ȩ����
///
//Ϊlpddsbackgroundɨ��λͼbitmap8,�ⲿ��Ӧ�÷�����Ϸ��ʼ�����������Ҳ�֪����;�л���ʱ������ô����
//�����Ͽ϶�����game_main���
if (COPYRIGHT==0){
#ifdef USE_GAMEDEV_TILE_2
	// now create and attach clipper
	
	if (singleton_num<=1){
	//	MessageBox(hwnd,"Ӧ����һ�β�����","debug",MB_OK);
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
#if 1//����ƿ��,������ʵû��


if (COPYRIGHT==1 && START_SELECT==-1){//���START��Ļ���ֹ��ˣ��ͼ�������һ������
	DSound_Stop_All_Sounds();
	DSound_Play(sound_ids[0]);
	START_SELECT=0;//��ֵ�������Ѿ���ʼ�˵�������
#if 0//ע������Ŀ��ܵ������������,��֮��Ĵ���
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
		Unload_Bitmap_File(&bitmapcopyright);//����˵�ѡ���棬ɾ��֮ǰ��������˾��Ȩ����
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
#endif//ע������Ŀ��ܵ������������,������Ĵ��棬��ʵ���ⲻ������ų���
	lpddsbackground=DDLoadBitmap(lpdd,"tilemap.bmp");
}//end if (COPYRIGHT==1 && START_SELECT==-1)


#endif
///
#ifdef USE_GAMEDEV_TILE_1
//draw_tiles(lpddsbackground,lpddsback);
#endif
#if 1
#if 0
#ifdef USE_GAMEDEV_TILE_2//����û����룬Ӧ�÷�������Ϣѭ�����������Ϣ�ص������
	//To do:������ļ������ŵ����
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
	//�����Ĳ���
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
	//����������Blt���󱸱���,��ʵ�������ϵ�DDRAW_DRAW_SURFACE
	static RECT srcrect,desrect;//һ����ª�ġ��������С�ʵ��
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
	//BLTĿ�ĵ�RECT
	desrect.left=190;
	desrect.top=60;
	desrect.right=0+430-1;
	desrect.bottom=0+300-1;
	//��KEYSRC��͸��
	if(START_SELECT!=-1){//START_SELECT����֮���������
		//�ޱ��Ķ�������,�Ϸ���
		//if(FAILED(lpddswork->Blt(&desrect,mysprite,&srcrect,(DDBLT_WAIT|DDBLT_KEYSRC ),NULL))){
		//	MessageBox(hwnd,"mysprite��������Blt���󱸻���ʧ��","��Ϣ",MB_OK);
		//}
		//��ǿ��Ļ��ƶ������飬û������
		if (++girl_swordman.counter >= (8 - girl_swordman.velocity))
        {
			// reset counter
			girl_swordman.counter = 0;
			
			// advance to next frame
			if (++girl_swordman.current_frame > 3)
				girl_swordman.current_frame = 0;
			
        } // end if
		//��װ��ĸ��²���
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
	



//////////////���˵�����//////////////////////////////////////////////
#if 1//�Ȼ��Ƶ�ɫ
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
	//BltĿ�ĵؾ�������
	desrect_mn.left=210;
	desrect_mn.top=240;
	desrect_mn.right=0+410-1;
	desrect_mn.bottom=0+400-1;
	
	if(START_SELECT!=-1){//������������˵�
		//if(FAILED(lpddsback->Blt(&desrect_mn,gamemenu,&srcrect_mn,(DDBLT_WAIT|DDBLT_KEYSRC ),NULL))){
		//	MessageBox(hwnd,"mysprite��������Blt���󱸻���ʧ��","��Ϣ",MB_OK);
		//}
	}
#endif
//���ûѡ����ôĬ��������һ���˵�����{�ɵĻ���}����ɫ�ġ�
#if 1
	if (START_SELECT==0){
		srcrect_mn.left=0;
		srcrect_mn.top=0;
		srcrect_mn.right=200-1;
		srcrect_mn.bottom=80-1;
		//BltĿ�ĵؾ�������
		desrect_mn.left=210;
		desrect_mn.top=251;
		desrect_mn.right=0+410-1;
		desrect_mn.bottom=0+331-1;
		//if(FAILED(lpddsback->Blt(&desrect_mn,gamemenu,&srcrect_mn,(DDBLT_WAIT|DDBLT_KEYSRC ),NULL))){
		//	MessageBox(hwnd,"mysprite��������Blt���󱸻���ʧ��","��Ϣ",MB_OK);
		//}
	}
#endif
	//������µ���Ϸ
#if 1
	if (START_SELECT==1){
		srcrect_mn.left=0;
		srcrect_mn.top=100;
		srcrect_mn.right=200-1;
		srcrect_mn.bottom=160-1;
		//BltĿ�ĵؾ�������
		desrect_mn.left=210;
		desrect_mn.top=351;
		desrect_mn.right=0+410-1;
		desrect_mn.bottom=0+411-1;
		//if(FAILED(lpddsback->Blt(&desrect_mn,gamemenu,&srcrect_mn,(DDBLT_WAIT|DDBLT_KEYSRC ),NULL))){
		//	MessageBox(hwnd,"mysprite��������Blt���󱸻���ʧ��","��Ϣ",MB_OK);
		//}
	}
#endif
//////////////////////////////////////////////////////////////////////////


	//��ס��������
	if(FAILED(mysprite->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL))){
	//echo error
		MessageBox(hwnd,"������������ʧ��","��Ϣ",MB_OK);
	}
	else{
	//MessageBox(hwnd,"������������OK","��Ϣ",MB_OK);
	}
	//������Щ��������Blt,GDI�ȵ�
	HDC xdc0;
	if (FAILED(mysprite->GetDC(&xdc0))){
		MessageBox(hwnd,"�����������ȡ�豸������ʧ��","��Ϣ",MB_OK);
	}
	SetTextColor(xdc0,1);
	SetBkColor(xdc0,0);
	SetBkMode(xdc0,TRANSPARENT);
	//char x0[]="����������GDI,BLT���󱸻��壡";
	char x0[]="���������ϵ�����һ����";
	TextOut(xdc0,20,10,(char *)buf2,strlen(buf2));
	mysprite->ReleaseDC(xdc0);
	if(FAILED(mysprite->Unlock(NULL))){
		//echo error
	}
	else{
		if(ECHODEBUG==1)
			MessageBox(hwnd,"������������OK","��Ϣ",NULL);
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
/*	if (FAILED(lpddsback->Blt(//��������BLT���󱸻���
		&rcRect,
		lpddswork,
		&rcRect,
		(DDBLT_WAIT|DDBLT_KEYSRC ),
		NULL
		))){
	MessageBox(hwnd,"","",MB_OK);
	}*/
	lpddsback->BltFast( 0, 0, lpddswork, NULL, DDBLTFAST_WAIT );//�Ż���

#if 1
#ifdef USE_GAMEDEV_TILE_2//����û����룬Ӧ�÷�������Ϣѭ�����������Ϣ�ص������
		//To do:������ļ������ŵ����
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
		//�����Ĳ���
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

//��ս��Ϸ�Լ�����
		/*if (mapdata.yCamera<20)
mapdata.yCamera =800;*/
		if ((mapdata.yCamera > 4)){
			//���boss1�����ˣ������û������Ļ��������������������
			//if(!(mapdata.yCamera<2000 && boss1!=NULL)){
			//if(boss1!=NULL){
				if(COPYRIGHT!=0)
				mapdata.yCamera -= 3;
				if (stage_num==1 && mapdata.yCamera<=3000)//����boss�ı����ٿ�һ��
				{
					mapdata.yCamera -= 4;
				}
				if (stage_num==0 && mapdata.yCamera<=2000)//����boss�ı����ٿ�һ��
				{
					mapdata.yCamera -= 2;
				}
			//}
		}
		// Display the frame rate.
		current_fps=ShowFrameRate(max_fps);
		if (max_fps<current_fps) 
			max_fps=current_fps;

//�л�������������
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

		//��ɺͷ�������������״̬��
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
				//�������ǳ�ʼλ��
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


//����һ������
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
				//�Ľ������ٶȣ��ĳ������60
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

//ˢ��
#if 0
//////////////////////////////////////////////////////////////////////////
		if(mapdata.yCamera<10800){
			
			static test_tank=0;
			if(test_tank==0){
				//alertme("��ൽ��");
// 				for (int i=0;i<=8;i++){
// 					enemy_list.push_back((cls_enemy *)new cls_enemy());
// 					pt_startpoint.x=rand()%640-rand()%14;
// 					pt_startpoint.y=rand()%480+rand()%22-500;//����һ��ʼ��������
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
					ptf_startpoint.y=rand()%480+rand()%22-500;//����һ��ʼ��������
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
				//��ʼ����һ���л�����6��
				for (int i=0;i<=8;i++){
					enemy_list.push_back((cls_enemy *)new cls_enemy());
					//}
					//pt_startpoint.x=130;
					//pt_startpoint.y=230;
					
					//list<cls_enemy * >::iterator   temp_enemy_iter;//�����滻ɾ������ʱ������
					//enemy_iter=enemy_list.begin();
					//while(enemy_iter!=enemy_list.end()){
					floatpoint ptf_startpoint;
					ptf_startpoint.x=rand()%640;
					ptf_startpoint.y=rand()%480-480;
					enemy_list.back()->SetPos(ptf_startpoint);
					enemy_list.back()->sprite_surface_id=enemy3_sfid;
					enemy_list.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo��ð��ӵ������к͵л������зֿ�
					enemy_list.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
					// 	(*enemy_iter)->sprite_surface_id=enemy4_sfid;
					// 	(*enemy_iter)->SetAnimateSequence("0,1");
					// 	(*enemy_iter)->SetAnimateData("167|152,167|152");
					enemy_list.back()->current_frame=(rand()%1);
					enemy_list.back()->SetVelocity(4);
					enemy_list.back()->SetAiState(1);
					enemy_list.back()->iFireInputDelay=23;
					if (i==4)//��һ������
					{
						enemy_list.back()->sprite_surface_id=enemy3_red_sfid;
						enum bonus bs=power;//��enum������Ӧc��������
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
//�ڶ�b��
		if(mapdata.yCamera<7500){
			
			static the_2bnd_enemy=0;
			if(the_2bnd_enemy==0){
				//alertme("��ൽ��");
				MessageBeep(MB_ICONASTERISK);
				for (int i=0;i<=8;i++){
					enemy_list.push_back((cls_enemy *)new cls_enemy());
					pt_startpoint.x=rand()%640-rand()%14;
					pt_startpoint.y=rand()%480+rand()%22-500;//����һ��ʼ��������
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
//������
		if(mapdata.yCamera<7000){
			static the_3rd0_enemy=0;
			if(the_3rd0_enemy==0){
		//	MessageBeep(MB_DEFBUTTON1);
				for (int i=0;i<=2;i++){
					enemy_list.push_back((cls_enemy *)new cls_enemy());
				
				//enemy_iter=enemy_list.begin();
				//while(enemy_iter!=enemy_list.end()){
					ptf_startpoint.x=rand()%640-rand()%14;
					ptf_startpoint.y=rand()%480+rand()%22-500;//����һ��ʼ��������
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
//////////////////////ѭ��ˢ��////////////////////////////////////
		//
#if 0
		if(mapdata.yCamera%500==0 && mapdata.yCamera<10000){
			
				for (int i=0;i<=3;i++){
					enemy_list.push_back((cls_enemy *)new cls_enemy());
				}
				enemy_iter=enemy_list.begin();
				while(enemy_iter!=enemy_list.end()){
					ptf_startpoint.x=rand()%640-rand()%14;
					ptf_startpoint.y=rand()%480+rand()%22-500;//����һ��ʼ��������
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
					++enemy_iter;//Todo��Щ����ʲôʱ������
				}

		//	
		}
#endif
//////////////////////////////////////////////////////////////////////////
#if 0
//������
		if(mapdata.yCamera<6700){
			
			static the_4th_enemy=0;
			if(the_4th_enemy==0){
				//alertme("�쵽��");
// 				for (int x=0;x<enemy_list.size();x++)
// 				{
// 					MessageBeep(MB_ICONASTERISK);
// 					Sleep(300);
// 				}
				
				for (int i=0;i<=12;i++){
					enemy_list.push_back((cls_enemy *)new cls_enemy());
					ptf_startpoint.x=rand()%640-rand()%14;
					ptf_startpoint.y=rand()%480+rand()%22-500;//����һ��ʼ��������
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
// 					ptf_startpoint.y=rand()%480+rand()%22-500;//����һ��ʼ��������
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
		//RGB565ģʽ
		
		//�޹صľͲ�Ҫ�ŵ�������������������½����أ�
		//һ���Ϻ���,֧���κ�λ����BLT���Ƽ�
		/*DDraw_Draw_Surface(sfobj->sprite_surface[2].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]], 
			girl_swordman.x, girl_swordman.y,
			sfobj->sprite_surface[2].sprite.theframe_width,sfobj->sprite_surface[2].sprite.theframe_height,
			lpddsback,1);*/
	/*	DDraw_Draw_Surface(sfobj->sprite_surface[6].sprite.frames[0], 
			girl_swordman.x+20, girl_swordman.y+70,
			sfobj->sprite_surface[6].sprite.theframe_width,sfobj->sprite_surface[6].sprite.theframe_height,
			lpddsback,1);*/
#if 0
//������
		static the_5th_enemy=0;
				if(mapdata.yCamera<6670){
			
			
			if(the_5th_enemy==0){
// 				for (int x=0;x<enemy_list.size();x++)
// 				{
// 					MessageBeep(MB_DEFBUTTON1);
// 					Sleep(300);
// 				}
				//alertme("��ൽ��");
				for (int i=0;i<=29;i++){
					enemy_list.push_back((cls_enemy *)new cls_enemy());
					ptf_startpoint.x=rand()%640-rand()%14;
					ptf_startpoint.y=rand()%480+rand()%22-500;//����һ��ʼ��������
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
// 					ptf_startpoint.y=rand()%480+rand()%22-500;//����һ��ʼ��������
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
//������
				if(mapdata.yCamera<2000){
			
			static the_6th_enemy=0;
			if(the_6th_enemy==0){
				//alertme("��ൽ��");
				for (int i=0;i<=8;i++){
					enemy_list.push_back((cls_enemy *)new cls_enemy());
					ptf_startpoint.x=rand()%640-rand()%14;
					ptf_startpoint.y=rand()%480+rand()%22-500;//����һ��ʼ��������
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
// 					ptf_startpoint.y=rand()%480+rand()%22-500;//����һ��ʼ��������
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
//boss�������µ�ͼ
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
		//�Ȼ�����Ӱ��abЧ��ddraw���ɫ�Բ��������о͸ĳɺ�ɫ����
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
//��ҷɻ�,�ĳɷ�װ�汾��
			DDraw_Draw_Surface(sfobj->sprite_surface[player_sfid].sprite.frames[player_plane_aniseq[player->GetCurrentFrame()]], 
			girl_swordman.x+offsetx*5, 480+offsety*4,
			sfobj->sprite_surface[player_sfid].sprite.theframe_width,sfobj->sprite_surface[player_sfid].sprite.theframe_height,
			lpddsback,1);
#endif


#if 1
//�ȼ�0�ӵ�
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
			//������������
			ptpl.x=player->GetPos().x+offsetx;ptpl.y=player->GetPos().y+offsety;
			if (player->GetState()!=starting && player->GetState()!=exitareaing)
			{
				//directionһ��������
				//��
				if (!KEYSTATE(VK_UP) && KEYSTATE(VK_LEFT)  && !KEYSTATE(VK_DOWN) ){
					//offsetx=-2;
					ptpl.x-=4;
				}
				//��
				if (!KEYSTATE(VK_UP) && KEYSTATE(VK_RIGHT)  && !KEYSTATE(VK_DOWN) ){
					//offsetx=2;
					ptpl.x+=4;
				}
				//��
				if (KEYSTATE(VK_UP) && !KEYSTATE(VK_RIGHT)  && !KEYSTATE(VK_LEFT) ){
					//offsety=-2;
					ptpl.y-=4;
				}
				//��
				if (KEYSTATE(VK_DOWN) && !KEYSTATE(VK_RIGHT)  && !KEYSTATE(VK_LEFT) ){
					//offsety=2;
					ptpl.y+=4;
				}
				//����һ���������ͬʱ������SPACE
				//��control
				if (!KEYSTATE(VK_UP) && KEYSTATE(VK_LEFT)  && !KEYSTATE(VK_DOWN) && KEYSTATE(VK_CONTROL) ){
					//offsetx=-2;
					//pt.y-=2;
				}
				//��control
				if (!KEYSTATE(VK_UP) && KEYSTATE(VK_RIGHT)  && !KEYSTATE(VK_DOWN) && KEYSTATE(VK_CONTROL) ){
					//offsetx=2;
					//pt.x+=2;
				}
				//��control
				if (KEYSTATE(VK_UP) && !KEYSTATE(VK_RIGHT)  && !KEYSTATE(VK_LEFT) && KEYSTATE(VK_CONTROL) ){
					//offsety=-2;
					//pt.y-=2;
				}
				//��control
				if (KEYSTATE(VK_DOWN) && !KEYSTATE(VK_RIGHT)  && !KEYSTATE(VK_LEFT) && KEYSTATE(VK_CONTROL) ){
					//offsety=2;
					//pt.y+=2;
				}
				
				
				//direction����ͬʱ����
				//����
				if (KEYSTATE(VK_UP)  && KEYSTATE(VK_LEFT) ){
					//MessageBox(hwnd,"test","info",MB_OK);
					//offsetx=-2;
					//offsety=-2;
					ptpl.x-=4;
					ptpl.y-=4;
				}
				//����
				if (KEYSTATE(VK_UP)  && KEYSTATE(VK_RIGHT)   ){
					//MessageBox(hwnd,"test","info",MB_OK);
					//offsetx=2;
					//offsety=-2;
					ptpl.x+=4;
					ptpl.y-=4;
				}
				//����
				if (KEYSTATE(VK_DOWN)  && KEYSTATE(VK_RIGHT)   ){
					//MessageBox(hwnd,"test","info",MB_OK);
					//offsetx=2;
					//offsety=2;
					ptpl.x+=4;
					ptpl.y+=4;
				}
				//����
				if (KEYSTATE(VK_DOWN)  && KEYSTATE(VK_LEFT)   ){
					//MessageBox(hwnd,"test","info",MB_OK);
					//offsetx=-2;
					//offsety=2;
					ptpl.x-=4;
					ptpl.y+=4;
				}
				//���������ƶ���Χ
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
				//����2�������ʱ������SPACE
				//����control
				if (KEYSTATE(VK_UP)  && KEYSTATE(VK_LEFT) && KEYSTATE(VK_CONTROL) ){
					//����������ǿ��Եģ�keystate�Ǽ���ж��źŵģ������п���ĳЩ
					//����ͬʱ���ⲻ�����ͱ�������ո񣬸ĳ�����control�Ϳ����ˡ�
					//MessageBox(hwnd,"test","info",MB_OK);Sleep(6);
					//offsetx=-2;
					//offsety=-2;
					//pt.x-=2;
					//pt.y-=2;
				}
				//����control
				if (KEYSTATE(VK_UP)  && KEYSTATE(VK_RIGHT) && KEYSTATE(VK_CONTROL) ){
					//MessageBox(hwnd,"test","info",MB_OK);
					//offsetx=2;
					//offsety=-2;
					//pt.x+=2;
					//pt.y-=2;
				}
				//����control
				if (KEYSTATE(VK_DOWN)  && KEYSTATE(VK_RIGHT) && KEYSTATE(VK_CONTROL) ){
					//MessageBox(hwnd,"test","info",MB_OK);
					//offsetx=2;
					//offsety=2;
					//pt.x+=2;
					//pt.y+=2;
				}
				//����control
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
// 			//�����ɻ�
// 			player->DrawMe(sfobj,player_sfid);
			
			//���ǿ���
			//fire!!!
			/*if(KEYSTATE(VK_SPACE))
			{
				player->AddBullet(pt);
			}*/
			//�����ɿ����
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


			
			//�ӵ�
			if(++g_iFireInputDelay>2 && KEYSTATE(VK_CONTROL) && key_press_counter<10)
			{
				//ѡ���ӵ��ȼ���Ȼ������ӵ�
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
			//end �ӵ�
			
			//����
			if(++g_iMissileInputDelay>30 && KEYSTATE(VK_CONTROL) && key_press_counter<10)
			{
				//ѡ���ӵ��ȼ���Ȼ������ӵ�
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
			//end ����

		}
			//�ƶ��ӵ�,�������ӵ�����Ҫ�˶���
			player->RunBullet(enemy_list);

			//����1
			enemy_iter=enemy_list.begin();
			while(enemy_iter!=enemy_list.end()){
				//�����ƶ�AIʵ��
				floatpoint ptf;
				ptf.x=(*enemy_iter)->GetPos().x;
				ptf.y=(*enemy_iter)->GetPos().y;
				//pt.x=ai->GetNewPos(pt,1).x;//Todo���ĳɶ�ʱ�л��ģ�
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
				//��������ǵļнǿ�ʼ
				if (player->GetPos().y>(*enemy_iter)->GetPos().y){//�����ڵл��·�
					if(player->GetPos().x!=(*enemy_iter)->GetPos().x){
						jiajiao0=atan(fabs(\
							((float)player->GetPos().y-(*enemy_iter)->GetPos().y)/\
							((float)player->GetPos().x-(*enemy_iter)->GetPos().x)\
							)\
							)/PI*180;
						//atan�ǻ��ȣ�����Ҫת�Ƕȣ�
						//http://zhidao.baidu.com/question/42746372.html
//  						if(player->GetPos().x<(*enemy_iter)->GetPos().x)
//  							jiajiao0+=180;//�����ڵл�����
//  						else
// 							jiajiao0+=270;//�����ڵл�����
					}
// 					else
// 						jiajiao0=270;//���Ǻ͵л���ֱ
						}
				//�����̹���ڹ�ָ������
				dynamic_cast<cls_tank *>(*enemy_iter)->AimTarget(player,jiajiao0);
#endif
				//(*enemy_iter)->DrawMe(sfobj,enemy2_sfid);
				if((*enemy_iter)->HP!=0){
					(*enemy_iter)->DrawMe(sfobj,(*enemy_iter)->sprite_surface_id);
				}
				
				//�л�����
				if(Fast_Distance_2D( (player->GetPos().x-(*enemy_iter)->GetPos().x),
					(player->GetPos().y-(*enemy_iter)->GetPos().y)
					)<600){//�����Ч��Χ��
					//alertme("caution ,under attack!");
					if( ++((*enemy_iter)->b_counter)==(*enemy_iter)->iFireInputDelay){
						int jiajiao;
						//��������ǵļнǿ�ʼ
						if (player->GetPos().y>(*enemy_iter)->GetPos().y){//�����ڵл��·�
							if(player->GetPos().x!=(*enemy_iter)->GetPos().x){
								jiajiao=atan(fabs(\
								((float)player->GetPos().y-(*enemy_iter)->GetPos().y)/\
								((float)player->GetPos().x-(*enemy_iter)->GetPos().x)\
								)\
								)/PI*180;
								//atan�ǻ��ȣ�����Ҫת�Ƕȣ�
								//http://zhidao.baidu.com/question/42746372.html
								if(player->GetPos().x<(*enemy_iter)->GetPos().x)
									jiajiao+=180;//�����ڵл�����
								else
									jiajiao+=270;//�����ڵл�����
							}
							else
								jiajiao=270;//���Ǻ͵л���ֱ
						}
						else
							jiajiao=-1;//��������ڵл��Ϸ��Ͳ����ˡ�
						//��������ǵļнǽ���
						if (jiajiao!=-1){//����ĳ�enemy���addbullet���������þͿ�����
							if((*enemy_iter)->HP!=0){								
								dynamic_cast<cls_tank *>(*enemy_iter)->AimTarget(player,0);
								if((*enemy_iter)->GetPos().x<118 || 
									(*enemy_iter)->GetPos().x>552 ||
									(*enemy_iter)->GetPos().y>600){
									//����뿪��Ļ̫Զ���Ͳ�Ҫ���ӵ��ˡ�
								}
								else{
									if ((*enemy_iter)->b_type==0 && (*enemy_iter)->use_fire_fsm!=1)
									{
										(*enemy_iter)->AddBullet(ptf,5,0,normal_e_bullet_sfid,jiajiao);//Todo�˴����й©��
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
									//Todo�ĳ�6���ӵ��������ǲ�������������ǰ�������ӵ�
									}
									//2009-11-18�µĵл�״̬
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
							//û��й¶!stl�Լ��������෽����malloc

						}
						(*enemy_iter)->b_counter=0;
					}
				}
				

				//�ĳ��жϵл����ӵ������Ա�ǲ���Ϊ�ղ���Խ�硣
				(*enemy_iter)->RunBullet();
				//������HP���ӵ���û��ʱ���Ļ���ɾ�����󣬵���HPΪ0��ʱ����������л�
// 				if((*enemy_iter)->isdie){
// 					temp_enemy_iter=enemy_iter;
// 					delete(*temp_enemy_iter);
// 					++enemy_iter;
// 					enemy_list.erase(temp_enemy_iter);
// 				}

				(*enemy_iter)->DrawBullet(sfobj);
				//�Ƿ񱻵л����еļ��

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
							//�Ľ������ٶȣ��ĳ������60
							sprite_list.back()->SetVelocity(57);
							sprite_list.back()->SetPos(ptf);
						}
						POINT ptdie;
						ptdie.x=300;
						ptdie.y=480;
						player->SetPos(ptdie);
						player->SetState(starting);
						//	MessageBox(NULL,"������","",MB_OK);
					}
				}



				//�Ƿ���ел��ļ��
				if(player->IsAttack(*enemy_iter)){
					DSound_Play(sound_ids[5]);//���л�
					ptf.x=(*enemy_iter)->GetPos().x;
					ptf.y=(*enemy_iter)->GetPos().y;
					/*expl->SetPos(pt);
					expl->UpdateFrame();
					expl->DrawMe(sfobj,expl_sfid);*/
					spark->SetPos(ptf);
					spark->UpdateFrame();
					spark->DrawMe(sfobj,spark_sfid,0,-35);

					//
					temp_enemy_iter=enemy_iter;//��ʱ������
					if((*temp_enemy_iter)->HP<0)
						(*temp_enemy_iter)->HP=0;
					if((*temp_enemy_iter)->HP==0){//�ĳ��ӵ���ʧʱ�����ٵл�����
						DSound_Play(sound_ids[2]);
						//delete(*temp_enemy_iter);
						hits+=1;//��׹��+1
						//�Ƿ��н����ļ��
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
							//�Ľ������ٶȣ��ĳ������60
							sprite_list2.back()->SetVelocity(55);
							sprite_list2.back()->SetNspeed(7);
							sprite_list2.back()->SetPos(pt);
							sprite_list2.back()->sprite_type=1;
							sprite_list2.back()->SetAngle(315);
							//////////////////////////////////////////////////////////////////////////
						}
						++enemy_iter;
						//expl->PlayOnce(sfobj,expl_sfid);
						//�����װ����ԣ�ѹ��1����ը�������
#if 1
						sprite_list.push_back(new cls_sprite());
						sprite_list.back()->Bind(sfobj);
						sprite_list.back()->sprite_surface_id=expl_sfid;
						sprite_list.back()->SetAnimateSequence("0,1,2,3,4,5,6,7,8,9,10,11,12,13");
						sprite_list.back()->SetAnimateData("23|33,23|33,23|33,14|33,23|33,14|33");
						sprite_list.back()->SetSeqNum(14);
						sprite_list.back()->current_frame=0;
						//�Ľ������ٶȣ��ĳ������60
						sprite_list.back()->SetVelocity(57);
						sprite_list.back()->SetPos(ptf);

						

#endif			
						//enemy_list.erase(temp_enemy_iter);
					}
					else {
						++enemy_iter;
					}//end if hp==0
				}//end if player->isattack
				else{//���û�й������Ļ����ͷű�ըͼ������б�ը�Ļ���	(�Ƶ����洦����)					
//					sprite_list_iter=sprite_list.begin();
//					while(sprite_list_iter!=sprite_list.end()){
// 						(*sprite_list_iter)->UpdateFrame();
// 						//����Ǳ�ը��������Ч�����飬��ô����Ϳ�����
// 						if((*sprite_list_iter)->sprite_type==0 && \
// 							(*sprite_list_iter)->PlayTime(1,(*sprite_list_iter)->sprite_surface_id)){
// 							temp_sprite_list_iter=sprite_list_iter;
// 							delete(*temp_sprite_list_iter); 
// 							++sprite_list_iter;
// 							sprite_list.erase(temp_sprite_list_iter);
// 						}
// 						//���������
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


			///////////////�Ƿ�boss1�����ļ��/////////////////////
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
						//�Ľ������ٶȣ��ĳ������60
						sprite_list.back()->SetVelocity(57);
						sprite_list.back()->SetPos(ptf);
					}
					POINT ptdie;
					ptdie.x=300;
					ptdie.y=480;
					player->SetPos(ptdie);
					player->SetState(starting);
					//	MessageBox(NULL,"������","",MB_OK);
				}
			}
			///////////////�Ƿ�boss2�����ļ��/////////////////////
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
						//�Ľ������ٶȣ��ĳ������60
						sprite_list.back()->SetVelocity(57);
						sprite_list.back()->SetPos(ptf);
					}
					POINT ptdie;
					ptdie.x=300;
					ptdie.y=480;
					player->SetPos(ptdie);
					player->SetState(starting);
					//	MessageBox(NULL,"������","",MB_OK);
				}
			}
			//////////////////////////////////////////////////////////////////////////

			//�Ѿ������л��������s
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
				//������HP���ӵ���û��ʱ���Ļ���ɾ�����󣬵���HPΪ0��ʱ����������л�
				if((*enemy_iter)->isdie){
					delete(*temp_enemy_iter);
					++enemy_iter;
					enemy_list.erase(temp_enemy_iter);
				}
				//Խ�����Ϊ������Ҳɾ������
				else if((*enemy_iter)->GetPos().x<-2000 || 
					(*enemy_iter)->GetPos().x>2000 ||
					(*enemy_iter)->GetPos().y>2000
					){
					(*enemy_iter)->HP=0;
					//delete(*temp_enemy_iter);
					++enemy_iter;
					//enemy_list.erase(temp_enemy_iter);
				}
				//����Ҳ������ȥ
				else{
					++enemy_iter;
				}
			}
			//�Ѿ������л��������e
			
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
						{ptf.x=boss1ai->GetNewPos(ptf,1).x;//Todo���ĳɶ�ʱ�л��ģ�
						ptf.y=boss1ai->GetNewPos(ptf,1).y;
						}
						else if (++bossstatecounter>=500 && bossstatecounter<=800)
						{ptf.x=boss1ai->GetNewPos(ptf,2).x;//Todo���ĳɶ�ʱ�л��ģ�
						ptf.y=boss1ai->GetNewPos(ptf,2).y;
						}
						else
							bossstatecounter=0;

						boss1->SetPos(ptf);
						boss1->UpdateFrame();//Todoÿ֡��ʵ��һ��û�н�����ӵ���ͼƬ�ķ��롣
						boss1->DrawMe(sfobj,boss1_sfid);
						//Todo:����ĳ�һ�����ʷ��伤��
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
						ptf.x=boss1ai->GetNewPos(ptf,1).x;//Todo���ĳɶ�ʱ�л��ģ�
						ptf.y=boss1ai->GetNewPos(ptf,1).y;
						}
						else if (++bossstatecounter>=400 && bossstatecounter<=800)
						{
						ptf.x=boss1ai->GetNewPos(ptf,2).x;//Todo���ĳɶ�ʱ�л��ģ�
						ptf.y=boss1ai->GetNewPos(ptf,2).y;
						}
						else if (bossstatecounter>=800 && bossstatecounter<=1000)
						{
							ptf.x=boss1ai->GetNewPos(ptf,5).x;//Todo���ĳɶ�ʱ�л��ģ�
							ptf.y=boss1ai->GetNewPos(ptf,5).y;
						}
						else
							bossstatecounter=0;
						
						boss2->SetPos(ptf);
						boss2->UpdateFrame();//Todoÿ֡��ʵ��һ��û�н�����ӵ���ͼƬ�ķ��롣
						boss2->DrawMe(sfobj,boss2_sfid);
						//Todo:����ĳ�һ�����ʷ��伤��
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
//���µ�������
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
//�����ӵ��͵������þ���������
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
//AlphaBlend16���ܲ���ô��
//AlphaBlend16(lpddswork,rcRect,girl_swordman.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],rcRect,5555);
		//		BltAlphaFastMMX( lpddsback, girl_swordman.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]], 0, 0, NULL, 1 );
		//���ˣ�ֻҪ����iAlphaΪ255�Ϳ����ˣ���Ҫȥ��DDraw_Draw_Surface�����ˣ����ʺ�������Ļ����㷨��
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
	//�����Ƿ��ȡ����
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
//����Ǳ�ը��������Ч�����飬��ô����Ϳ�����
if((*sprite_list_iter)->sprite_type==0 && \
   (*sprite_list_iter)->PlayTime(1,(*sprite_list_iter)->sprite_surface_id)){
	temp_sprite_list_iter=sprite_list_iter;
	delete(*temp_sprite_list_iter); 
	++sprite_list_iter;
	sprite_list.erase(temp_sprite_list_iter);
}
////���������
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
// 				//�����̹���ڹ�ָ������
// 				dynamic_cast<cls_tank *>(*enemy_iter)->AimTarget(player,0);
// 				++enemy_iter;
// 			}
		//BltAlphaMMX( lpddsback, sfobj->sprite_surface[2].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],200, 0, NULL, ( int ) iAlpha, 1 );
		//BltAlphaMMX( lpddsback, sfobj->sprite_surface[0].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],200, 200, NULL, ( int ) 255, 1 );
		//BltAlphaMMX( lpddsback, sfobj->sprite_surface[0].sprite.frames[girl_swordman_stand_aniseq[girl_swordman.current_frame]],300, 0, NULL, ( int ) iAlpha, 1 );
		//�ɻ�
		//BltAlphaMMX( lpddsback, sfobj->sprite_surface[3].sprite.frames[player_plane_aniseq[sfobj->sprite_surface[3].sprite.current_frame]],sfobj->sprite_surface[3].sprite.x, sfobj->sprite_surface[3].sprite.y, NULL, ( int ) iAlpha, 1 );
		//��ɫ��boss
		//BltAlphaMMX( lpddsback, sfobj->sprite_surface[boss1_sfid].sprite.frames[0],300, 0, NULL, ( int ) 30, 1 );
		//����Ч��
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

		//�����ɻ�
		if (player->GetHp()>0)
		{
				player->DrawMe(sfobj,player_sfid);
		}
		

		//��ʾ��һ��
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
					//��ĸͣ��һ��
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
					//��ĸͣ��һ��
				}
				else{
					zimu_offset1+=1;
				}
			}
		}

		

//������
		DDraw_Draw_Surface(leftbar_surface, 
			0, 0,
			118,481,
			lpddsback,0);
//�Ҳ����
		//DDraw_Draw_Surface(sfobj->sprite_surface[rightbar_sfid].sprite.frames[0], 
		DDraw_Draw_Surface(rightbar_surface, 
			523, 0,
			118,481,
			lpddsback,0);
		//��ʾHITS
		showHITS(hits);
		//��ʾHP
		showHP(player->GetHp(),20);
//��Ʒ
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
		//��ʾHITS
		showHITS(hits);
		//��ʾHP
		showHP(player->GetHp(),20);
//��ʱboss2
// 		DDraw_Draw_Surface(sfobj->sprite_surface[boss2_sfid].sprite.frames[0], 
// 			210, 110,
// 			139,142,
// 			lpddsback,1);
		
//���������Ϸ��ʾ����
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
//������һ��9֡�Թ�Ч��
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
		

		if (whichopt==startgameopt){//start��ť
			DDraw_Draw_Surface(sfobj->sprite_surface[opt1_sfid].sprite.frames[0], 
				72, 443,
				132,38,
			lpddsback,1);
		}
		if (whichopt==optionopt){//option��ť
			DDraw_Draw_Surface(sfobj->sprite_surface[opt2_sfid].sprite.frames[0], 
				254, 443,
				78,38,
				lpddsback,1);
		}
		if (whichopt==staffopt){//staff��ť
			DDraw_Draw_Surface(sfobj->sprite_surface[opt3_sfid].sprite.frames[0], 
				399, 443,
				64,38,
				lpddsback,1);
		}
		if (whichopt==exitopt){//exit��ť
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
			//ѡ������
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
				if (option_edit_which==edit_difficult)//����
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_eazy1_sfid].sprite.frames[0], 
						320, 110,
						165,58,
					lpddsback,1);
				}
				else if (option_edit_which==edit_lianfa)//����
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
				if (option_edit_which==edit_difficult)//����
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_normal1_sfid].sprite.frames[0], 
						320, 110,
						165,58,
						lpddsback,1);
				}
				else if (option_edit_which==edit_lianfa)//����
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
				if (option_edit_which==edit_difficult)//����
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_hard1_sfid].sprite.frames[0], 
						320, 110,
						165,58,
						lpddsback,1);
				}
				else if (option_edit_which==edit_lianfa)//����
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
				if (option_edit_which==edit_difficult)//����
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_eazy1_sfid].sprite.frames[0], 
						320, 110,
						165,58,
						lpddsback,1);
				}
				else if (option_edit_which==edit_lianfa)//����
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
				if (option_edit_which==edit_difficult)//����
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_normal1_sfid].sprite.frames[0], 
						320, 110,
						165,58,
						lpddsback,1);
				}
				else if (option_edit_which==edit_lianfa)//����
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
				if (option_edit_which==edit_difficult)//����
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_hard1_sfid].sprite.frames[0], 
						320, 110,
						165,58,
						lpddsback,1);
				}
				else if (option_edit_which==edit_lianfa)//����
				{
					DDraw_Draw_Surface(sfobj->sprite_surface[option_on1_sfid].sprite.frames[0], 
						320, 260,
						103,58,
						lpddsback,1);
				}
			}
			//end ѡ������
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
		//�������7.6��Ҫ��FLIP!
	}
	else{
		// lock to 30 FPS (33ms per frame)
		int wantfps=0;
		if (wantfps>70 && wantfps<=80)
			wantfps=22;//22����while��ʵ��Ч����max75fps�Ļ���32
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
	//MessageBox(hwnd,"Flip","��Ϣ",MB_OK);
	//Sleep(30);
	//


ReleaseDC(hwnd,hdc);

}
// return to Windows like this
return(msg.wParam);

} // end WinMain






int gameinit(){
//ȫ�ֱ�����ʼ��
START_SELECT=-1;
ShowCursor(FALSE);
//���¼�ѭ��ǰ����gameinit,����һ��ĳ�ʼ��DD��ʲô�����ݽṹ��ȫ�ֱ���ʲô��

if (FAILED(DirectDrawCreateEx(NULL, (void **)&lpdd, IID_IDirectDraw7, NULL))){
	return(0);}
else{
	if (ECHODEBUG==1)
		MessageBox(globalhwnd,"����DD�ӿڳɹ�","��Ϣ",NULL);
}
if (FAILED(lpdd->QueryInterface(IID_IDirectDraw7,(LPVOID *)&lpdd))){//����ʡȥ��û��IID_IDirectDraw8��
//echo error
}
else{
	if(ECHODEBUG==1)
		MessageBox(globalhwnd,"�����ӿڳɹ�","��Ϣ",NULL);
}
if (FAILED(lpdd->SetCooperativeLevel(globalhwnd,DDSCL_FULLSCREEN|DDSCL_EXCLUSIVE|DDSCL_ALLOWREBOOT|DDSCL_ALLOWMODEX))){
//echo error
}
else{
	if(ECHODEBUG==1)
		MessageBox(globalhwnd,"����Э���ȼ��ɹ�","��Ϣ",NULL);
}
//lpdd->SetDisplayMode(640,480,8,0,0);
lpdd->SetDisplayMode(640,480,16,0,0);

//PALETTEENTRY          palette[256];          // color palette

LPDIRECTDRAWPALETTE lpddpal=NULL;
#if 0
//���ṹ
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


// ������ɫ�����
if (FAILED(lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 | 
                                DDPCAPS_INITIALIZE, 
                                palette,&lpddpal, NULL)))
{
// echo error
return(0);

} // end if
#endif
//memset(&ddsd,0,sizeof(ddsd));//Ϊddsd�����ڴ��ַ��Ҳ����ʹ��ZeroMemory
ZeroMemory(&ddsd,sizeof(ddsd));
//���DDSURFACEDESC2���ݽṹ
ddsd.dwSize=sizeof(ddsd);
DDRAW_INIT_STRUCT(ddsd); 
ddsd.dwFlags=DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
ddsd.dwBackBufferCount=1;
ddsd.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE|DDSCAPS_COMPLEX|DDSCAPS_FLIP;
if (FAILED(lpdd->CreateSurface(&ddsd,&lpddsprimary,NULL))){
//echo error
}

#if 0//old windows gmp�ϵģ���BUG�����ˣ�
//ѡ���ɫ��

if(FAILED(lpddsprimary->SetPalette(lpddpal))){
//echo error
	MessageBox(globalhwnd,"���õ�ɫ��ʧ��","��Ϣ",MB_OK);
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

//��Ϣ�ص�����
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
		
	case WM_KEYDOWN:{//WM_KEYDOWN�����¼�
		int virtualcode=(int)wparam;
		int keystate=(int)lparam;


//�������ⲿ�ִ��������ˡ�

#if 0
			static int emu_return_once=0;
			if (emu_return_once==0){
				keybd_event(VK_RETURN,0,0,0);
				++emu_return_once;
			}
#endif
		switch(virtualcode){
			/*
		case VK_RIGHT:{//MessageBox(hwnd,"��","caution",MB_OK);
#ifdef USE_GAMEDEV_TILE_1
			world_camerax += 8;
#endif
			offsetx+=1;
					  }break;
					  */
			/*
		case VK_LEFT:{//MessageBox(hwnd,"��","caution",MB_OK);
#ifdef USE_GAMEDEV_TILE_1
			world_camerax -= 8;
#endif
			offsetx-=1;
					 }break;
					 */
			/*
		case VK_UP:{
			//MessageBox(hwnd,"��","caution",MB_OK);
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
			//MessageBox(hwnd,"��","caution",MB_OK);
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
			if(COPYRIGHT==1){//�����������Ϸ����
				/*if(START_SELECT==0){
					MessageBox(hwnd,"��ͣ","pause",MB_OK);
				}
				else if(START_SELECT==1){
					MessageBox(hwnd,"��ͣ1","pause",MB_OK);
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
	case WM_LBUTTONDBLCLK:{//���˫���¼�
		int mouse_x=(int)LOWORD(lparam);int mouse_y=(int)HIWORD(lparam);
		//char buf2[50];�ĳ�ȫ�ֱ���
		sprintf(buf2,"��˫����!������X:%d Y:%d",mouse_x,mouse_y);
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
