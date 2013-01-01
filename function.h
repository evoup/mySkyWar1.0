//from gamedev.net tileengine demo load bitmap to surface function
//////////////////////////////////////////////////////////////////////
// LOAD_BITMAP
// Loads a bitmap resource onto the specified DirectDraw surface.
//////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_FUNCTION
#define INCLUDE_FUNCTION
#pragma warning(disable : 4786) 
#define _15BIT(r,g,b) (((r&248)<<7) + ((g&248)<<2) + (b>>3))//15b 555
#define _16BIT(r,g,b) (((r&248)<<8) + ((g&252)<<3) + (b>>3))//16b 565
#include <ddraw.h>
#include <stdio.h>
#include <string>//��2����stringstreamҪ�õ� 
#include <sstream>
#include <vector>
//#include <list>
using namespace std;
extern HINSTANCE hMainInstance;
extern HWND globalhwnd;
extern PALETTEENTRY palette[256];
extern LPDIRECTDRAWPALETTE globallpddpal;
extern LPDIRECTDRAWSURFACE7 lpddsback;
extern HFONT g_hFont;
//add by evoup
//##ModelId=4A1E88FB03C5
extern struct bmpinfo{//����������ʱ��Ϣ��DDCopyBitmap2������
	//##ModelId=4A1E88FB03D3
	HBITMAP hbmp;
	//##ModelId=4A1E88FB03D5
	BITMAP bm;
}*bmpinfo_struct;
int Load_Bitmap(char *lpszName, int x, int y, LPDIRECTDRAWSURFACE7 lpdds);//ֻ�ܶ�16ɫ24ɫ�ĺ���
int Load_Bitmap256(char *lpszName, int x, int y, LPDIRECTDRAWSURFACE7 lpdds);//��Ȼ�ܶ�256ɫ���ǵ�ɫ�岻�Եĺ���
bool GetBitmapandPalette(LPCTSTR lpszresource,HBITMAP &bitmap, HPALETTE &pal,long *w,long *h);//��ȡλͼ��ɫ��ĺ���
bool Loadbmp2Surface(char *lpszName, int x, int y, LPDIRECTDRAWSURFACE7 lpdds);

//from gamedev new 4 function
IDirectDrawSurface7 * DDLoadBitmap(IDirectDraw7 *pdd, LPCSTR szBitmap);
IDirectDrawSurface7 * DDLoadBitmap2(IDirectDraw7 *pdd, LPCSTR szBitmap,bmpinfo &bmpinfo);
IDirectDrawSurface7 * CreateOffScreenSurface(IDirectDraw7 *pdd, int dx, int dy);
HRESULT DDCopyBitmap(IDirectDrawSurface7 *pdds, HBITMAP hbm, int dx, int dy);
/*IDirectDrawSurface7 * DDCopyBitmap2(IDirectDrawSurface7 *pdds,IDirectDrawSurface7 *src_pdds,\
									HBITMAP hbm,int dx, int dy,int srcx,int srcy);*/
IDirectDrawPalette * DDLoadPalette(IDirectDraw7 *pdd, LPCSTR szBitmap);
//void testx();
bool ClearSurface(LPDIRECTDRAWSURFACE7 surface, int r, int g, int b);

//2�������õĺ��������Դ�ӡ���λ���CHAR *
/*bool alert(int word);
bool alert2(LPCSTR word);*/
//template <typename T > void alertme(const T );//ʡ�β�,ע��VC��֧��ģ���������
//�ĳɺ���ģ��
template <typename T > void alertme(const T &word){
char buf[30];
if ( strcmp( typeid( word ).name( ), "int" ) == 0 ) {
	sprintf(buf,"%d",word);
}
else if ( strcmp( typeid( word ).name( ), "double" ) == 0 ) {
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%f",word);
}
else if ( strcmp( typeid( word ).name( ), "long" ) == 0 ) {
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d",word);
}
else {
		sprintf(buf,"%s",word);
}

MessageBox(globalhwnd,buf,"alertinfo",MB_OK);
//return 1;
}

void alert3(int v);


bool debugshow(int,int);

//C++�汾��phpapi->explode
vector<string> explode( const string &delimiter, const string &explodeme);

//C++STL��string����ת��int
int string2int(const string &);

//C++STL��string����ת��float
float string2float(const string &s);
#endif
