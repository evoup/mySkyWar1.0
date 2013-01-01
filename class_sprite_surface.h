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
/**�ýṹ��Ϊ�����زĻ����ѡ����
*/

//##ModelId=4A1E88FB03E1
typedef struct SPRITE_SURFACE{
IDirectDrawSurface7 * lpddsrc;//���������زĵĻ���
LPCSTR srcname;//�زĵ�����
int sprite_numframe;//�����֡��
SPRITE_OBJ sprite;//����ṹ��
LPCSTR spritename;//���������
int used;//�û����Ƿ�ʹ�ù�

//SPRITE_SURFACE * next;//�γ�1������֮�����д��lpdds���ֲ����Եĺ���,��ʱ��������
//��ֱ�����±�1000����
}SP_SF,*lp_sprite_surface;

//##ModelId=4A1E88FC0009
typedef struct SURFACE_MGR_STRUCT{//һ���������ṹ����������SPRITE_SURFACE�ṹ�Ķ���
	LPCSTR sprite_group_name;//���������ͳһ���֣������ñ�LV1
	LPCSTR sprite_surface_array_indexes;//�������������ָ�����'|'�ɹ�����
	LPCSTR source_pic_name;//�ز��ļ�����
	IDirectDrawSurface7 * lpdds;//���ص����ĸ��زĻ���
	int dds_width;//�زĳ�
	int dds_height;//�زĿ�
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
	
	int get_max_sprite_surface(int debug);/**��ȡ��SPRITE_SURFACE�ṹ��������**/
	
	IDirectDrawSurface7 * getSpriteSurface(IDirectDrawSurface7 * lpdds,char * srcname,char * spritename,\
		LPCSTR szBitmap,int frame_num,bool debugflag=0);
	IDirectDrawSurface7 * getSpriteSurfaceJPG(IDirectDrawSurface7 * lpdds,char * srcname,char * spritename,\
		LPCSTR szJpg,int frame_num,bool debugflag=0);
	
	SPRITE_SURFACE sprite_surface[1000];//�ṹ���Ա����������ƥ�䵽�زĻ���Ͷ�Ӧ�ľ��鶯������
	
	LPCSTR get_serial_surface(char * sp_gp_name);//���ݾ������ַ���һϵ�о��黭��ľ��"1|2|3",֮��
	//���������������������"���ñ�"���᷵�����ɸ������ñ������黭��������±�
	//std::list <SPRITE_SURFACE *> sprite_surface_list;

	//���ⲿ���Է��ʱ���Ա��ĳЩ����
	
	friend int friend_cls_sprite_surface_getmax(cls_sprite_surface *);//ע��ʡ�βε�д������׼д������(cls_sprite_surface *obj)
private:
	
	HWND hwnd;
	
	std::list<SF_MGR * > sprite_handler_list;
	
	SF_MGR  * sfmgrtemp;//������ʱ���sprite_handler_list�ýṹ��
	
	std::list<SF_MGR * >::iterator iter;//����sprite_handler_list����


};
///
#endif 
