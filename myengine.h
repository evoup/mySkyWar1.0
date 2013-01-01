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
	LPDIRECTDRAWSURFACE7 frames[100]; // ��/���м�������֡,��Ĭ�Ϸֱ�100�����棡�����ܳ���100���������аɣ�
	float x,y; // ���Ͻ�����
	int velocity; // ����
	int current_frame; // �л�����һ֡
	int counter; // ��ʱ
	int theframe_width;//��֡�Ŀ�ȣ�Ĭ���ǳ����Ŀ�ȣ����ڶ������¾�����ʱ���Ƿ�Χ��һ��������������Ĭ�ϣ�С�˾͵ò�ѯ�����ͬ���ѯһ��
	int theframe_height;//��֡�ĸ߶�
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
typedef struct HAPPY_FACE_TYP//����Ц�����ݽṹ����Ҫ�ǿ�����������
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

//BltAlphaMMX�Ĳ�����װ�����aplha������ddrawclipper����Խ���������⡣
bool myBAMMX(LPDIRECTDRAWSURFACE7 work,LPDIRECTDRAWSURFACE7 back,int startx,int starty,
			 int alpha,SPRITE_OBJ * whichsp,LPDIRECTDRAWSURFACE7 sprite);


//##ModelId=4A1E9C300170
typedef struct bullet
{
  struct bullet* next; //ָ����һ���ӵ�����STL�Ͳ���next�ˡ�
  int nStyle;          //�ӵ�������
  float nSpeed;          //�ӵ����ٶ�
  int nDirection;      //�ӵ��ķ���
  int nDisplayState;   //��ʾ��ʽ���������Ŀ�꣬��ת��ը״̬���Ա�
                       //Runbullet������ʾ��ըЧ����ͬ�ϱߵķɻ�������һ��
  float x;//�ӵ�X����
  float y;//�ӵ�Y����
  int width;//�ӵ���
  int height;//�ӵ���
  int sfid;//����id
  int damage;//����
  int canhit;//���Ա����(Todo:�ӵ�Ҳ��HP)
}BULLET_OBJ,*BULLET_OBJ_PTR;

typedef enum bonus{
	nothing=0,
	power=1,
	bomb=2,
	money500=3,
	money1000=4,
	money2000=5
};

//����֡���ݣ���Ҫ���һ֡����Ч��������������ײ��������һЩ���
//##ModelId=4A28D60B008C
typedef struct anidata{
int id;//����id
int width;//��ǰ���еľ�����Ч�����ȣ������ĵ����
int height;//��ǰ���еľ�����Ч����߶ȣ������ĵ����
}ANIDATA_OBJ,*ANIDATA;

//��Ҫ��ŷɻ��Ļ���ָ�룬��Ч��������ÿ֡���ı�
//##ModelId=4A1E9F570122
typedef struct planedisplay{
LPDIRECTDRAWSURFACE7 * player_surface;//����ָ��
RECT validate_area;//��Ч����
}PLANEDISPLAY_OBJ,*PLANEDISPLAY_OBJ_PTR;

//�������ؾ�ȷ����Ľṹ
//##ModelId=4B02B4E40196
typedef struct floatpoint{
float x;
float y;
}*struct_floatpoint;


//
#endif
