#include "myengine.h"




//�������أ�blt���أ�
void mybltclip(HWND hwnd,USHORT * lpsurface,int mempitch){//To do ���alt+f4�˳�����Ӧ����game_shutdown
	//MessageBox(hwnd,"test","test",strlen("test"));
	//�������صķ���,�����16λ�ķ���
	lpsurface[100+220*(mempitch>>1)]=_RGB16BIT565(rand()%256, rand()%256, rand()%256);
}


///////////////////////////////////////////////////////////



//����λͼ��blt����棩
void mybltclip2(LPDIRECTDRAWSURFACE7 lpddsprimary,
				HWND hwnd,
				int x, 
				int y,
				int width,
				int height,
				UCHAR * bitmap,
				UCHAR * lpsurface,
				int mempitch){
	if ((x >= 640) || (y>= 480) ||
		((x + width) <= 0) || ((y + height) <= 0))
	return;//�ȼ���ǲ�����ȫ�����û���


	// һЩ��ֵ
	int x1 = x;
	int y1 = y;
	int x2 = x1 + width - 1;
	int y2 = y1 + height -1;

	// ���ϽǼ����ж�
	if (x1 < 0)
	   x1 = 0;

	if (y1 < 0)
	   y1 = 0;

	// ���½Ǽ����ж�
	if (x2 >= 640)
		x2 = 640-1;

	if (y2 >= 480)
		y2 = 480-1;

	//MessageBox(hwnd,"test","test",strlen("test"));
/*	HDC xdc;//��ӡһЩ������Ϣ
	if (FAILED(lpddsprimary->GetDC(&xdc))){
		//echo error
		}
	SetTextColor(xdc,RGB(0,0,rand()%255));

	TextOut(xdc,40,400,"��δ��ݱ���������ַ���",strlen("��δ��ݱ���������ַ���"));
	lpddsprimary->ReleaseDC(xdc);*/

	//
	// now we know to draw only the portions of the bitmap from (x1,y1) to (x2,y2)
// compute offsets into bitmap on x,y axes, we need this to compute starting point
// to rasterize from
int x_off = x1 - x;
int y_off = y1 - y;

// compute number of columns and rows to blit
int dx = x2 - x1 + 1;
int dy = y2 - y1 + 1;

// compute starting address in video_buffer 
lpsurface += (x1 + y1*mempitch);

// compute starting address in bitmap to scan data from
bitmap += (x_off + y_off*width);

// at this point bitmap is pointing to the first pixel in the bitmap that needs to
// be blitted, and video_buffer is pointing to the memory location on the destination
// buffer to put it, so now enter rasterizer loop

UCHAR pixel; // used to read/write pixels

for (int index_y = 0; index_y < dy; index_y++)
     {
     // inner loop, where the action takes place
     for (int index_x = 0; index_x < dx; index_x++)
          {
          // read pixel from source bitmap, test for transparency and plot
          if ((pixel = bitmap[index_x]))
              lpsurface[index_x] = pixel;

          } // end for index_x
     
          // advance pointers
          lpsurface+=mempitch;  // bytes per scanline
          bitmap      +=width;     // bytes per bitmap row

     } // end for index_y
	//
	
}

//һ����ֹab���粢�Һ;��������ϵĺ���
bool myBAMMX(LPDIRECTDRAWSURFACE7 lpddswork,LPDIRECTDRAWSURFACE7 lpddsback,int startx,int starty,
			 int iAlpha,SPRITE_OBJ * whichsp,LPDIRECTDRAWSURFACE7 sprite){
	//�Ȳ�ѯ����ĳ���
	int sp_width=whichsp->theframe_width;
	int sp_height=whichsp->theframe_height;
	//int startx,starty;
//�д��������
	//startx=437;
	//starty=-8;

	//�����ϵ�
	/*startx=400;//bltĿ�ĵ���ʼ��
	starty=-83;*/
	//�������Ƴ������ϵ�
	/*startx=-83;//bltĿ�ĵ���ʼ��
	starty=-83;*/
	//���󲿷��Ƴ�
	//startx=-113;//bltĿ�ĵ���ʼ��
	//starty=123;
	//���Ƴ����֣�����
	/*startx=-111;//bltĿ�ĵ���ʼ��
	starty=400;*/
	//���²����Ƴ�
	/*startx=100;//bltĿ�ĵ���ʼ��
	starty=400;*/
	//���Ƴ����֣�����
	/*startx=560;//bltĿ�ĵ���ʼ��
	starty=400;*/
	//���Ҳ��Ƴ�
	/*startx=560;//bltĿ�ĵ���ʼ��
	int starty=100;*/
	//���Ƴ����֣�����
	/*startx=520;//bltĿ�ĵ���ʼ��
	int starty=-80;*/
	//�����������Ļ�ڣ��������ó�����
	/*startx=100;//bltĿ�ĵ���ʼ��
	starty=200;*/

	int blt_flag;//�Ƿ�ִ��blt���
	blt_flag=1;
	int debug_position;//�����õķ�λ���
	debug_position=-999;
	//��ֹAB������磬�ȿ������������棡
	RECT dsrect,rcrect;
#if 1
	dsrect.top=starty;//��Ļ����λ��
	dsrect.left=startx;//��Ļ����λ��
	dsrect.right=startx+sp_width;//��Ļ����λ�ã�right-leftû�г��������ȵ����ֵ
	dsrect.bottom=starty+sp_height;//��Ļ����λ�ã�bottom-top����û�г�������ĸ߶����ֵ
	rcrect.top=0;//�������λ��
	rcrect.left=0;//�������λ��
	rcrect.right=sp_width;//�������λ�ã�right-left=����Ŀ�ȵĻ����Ǿ���Ŀ���ˡ�
	rcrect.bottom=sp_height;//�������λ�ã�����������λ���Ǿ���ĸ߶ȼ���x�Ļ�����ôҪblt����
	//��ͼ����ˣ���������Ҫ����Ŀ��Blt���ε������ˡ��ؼ������Ƴ���Ļ����ֵ��һ������src���ȥ��Ӧ�ģ�
	//��������ﵽ�ľ���ĳ����Ͳ�Ҫblt�ˣ������Ͷ��ˡ�����des������Բ����Գ�����Ļ��Χ��
#endif

	//���µ��Կ���ʹ��function.cpp��alert������ӡ������
	//��ʱ�뿪ʼ��8��������Զ���surface��clip�㷨
	//������
	if (640-startx-sp_width>=0 && startx>=0 && startx<640 && sp_width+starty>0 && starty<0){
		dsrect.top=0;
		dsrect.left=startx;
		dsrect.right=sp_width+dsrect.left;
		dsrect.bottom=starty+sp_height;
		rcrect.top=-starty;
		rcrect.left=0;
		rcrect.right=sp_width;
		rcrect.bottom=sp_height;
		debug_position=1;
	}
	//�������Ƴ�������
	else if (startx<0 && starty<0 && starty+sp_height>0 && startx+sp_width>0){
		dsrect.top=0;
		dsrect.left=0;
		dsrect.right=sp_width+startx;
		dsrect.bottom=sp_height+starty;
		rcrect.top=-starty;
		rcrect.left=-startx;			
		rcrect.right=sp_width;
		rcrect.bottom=sp_height;
		debug_position=2;
	}
	//���󲿷��Ƴ�
	else if(startx+sp_width>0 && startx<0 && sp_height-(480-starty)<=0 && 480>starty && starty>=0){
		dsrect.top=starty;
		dsrect.left=0;
		dsrect.right=sp_width+startx;
		dsrect.bottom=sp_height+starty;
		rcrect.top=0;
		rcrect.left=-startx;
		rcrect.right=sp_width;
		rcrect.bottom=sp_height;
		debug_position=3;
	}
	//���Ƴ����֣�����
	else if(startx+sp_width>0 && startx<0 && 480>starty && starty>0 && sp_height-(480-starty)>0){
		dsrect.top=starty;
		dsrect.left=0;
		dsrect.right=sp_width+startx;
		dsrect.bottom=480;
		rcrect.top=0;
		rcrect.left=-startx;
		rcrect.right=sp_width;
		rcrect.bottom=480-starty;
		debug_position=4;
	}
	//���²����Ƴ�
	else if(startx>0 && startx<=640-sp_width && starty<480 && starty>0 && sp_height-(480-starty)>0){
		dsrect.top=starty;
		dsrect.left=startx;
		dsrect.right=startx+sp_width;
		dsrect.bottom=480;
		rcrect.top=0;
		rcrect.left=0;
		rcrect.right=sp_width;
		rcrect.bottom=480-starty;
		debug_position=5;
	}
	//���Ƴ����֣�����
	else if(startx<640 && startx>0 && 640-startx-sp_width<0 && 480>starty && starty>0 && sp_height-(480-starty)>0){
		dsrect.top=starty;
		dsrect.bottom=startx;
		dsrect.right=640;
		dsrect.bottom=480;
		rcrect.top=0;
		rcrect.left=0;
		rcrect.right=640-startx;
		rcrect.bottom=480-starty;
		debug_position=6;
	}
	//���Ҳ��Ƴ�
	else if(640>startx && startx>0 && 640-startx-sp_width<0 && 480>starty && starty>=0){
		dsrect.top=starty;
		dsrect.left=startx;
		dsrect.right=640;
		dsrect.bottom=starty+sp_height;
		rcrect.top=0;
		rcrect.left=0;
		rcrect.right=640-startx;
		rcrect.bottom=sp_height;
		debug_position=7;
	}
	//���Ƴ����֣�����
	else if(startx>0 && startx<640 && 640-startx<sp_width && starty<0 && sp_height+starty>0){
		dsrect.top=0;
		dsrect.left=startx;
		dsrect.right=640;
		dsrect.bottom=sp_height+starty;
		rcrect.top=-starty;
		rcrect.left=0;
		rcrect.right=640-startx;
		rcrect.bottom=sp_height;
		debug_position=8;
	}
	//�����������Ļ�ڣ��������ó�����
	else if(480-starty>=sp_height && 640-startx>=sp_width && startx>=0 && starty>=0){
		//do blt
		debug_position=99;
	}
	//����8�������Ƴ�����Ҫ�е�
	//������ϣ��󲿷��Ƴ�
	else if(-startx<=sp_width && starty==0 && startx<0){
		dsrect.top=0;
		dsrect.left=0;
		dsrect.right=sp_width+startx;
		dsrect.bottom=sp_height;
		rcrect.top=0;
		rcrect.left=-startx;
		rcrect.right=sp_width;
		rcrect.bottom=sp_height;
	}
	//��������ϲ����Ƴ�
	else if(startx==0 && starty<=0 && sp_height+starty>0){
	dsrect.top=0;
	dsrect.left=0;
	dsrect.right=sp_width;
	dsrect.bottom=sp_height+starty;
	rcrect.top=sp_height+starty;
	rcrect.left=0;
	rcrect.right=sp_width;
	rcrect.bottom=sp_height;
	}
	//�����²����Ƴ�
	else if(startx==0 && 480>starty && starty>=0 && 480-starty<=sp_height){
	dsrect.top=starty;
	dsrect.left=0;
	dsrect.right=sp_width;
	dsrect.bottom=480;
	rcrect.top=0;
	rcrect.left=0;
	rcrect.right=sp_width;
	rcrect.bottom=480-starty;
	}
	//������£��󲿷��Ƴ�
	else if(starty==480-sp_height && startx+sp_width<=0){
	dsrect.top=starty;
	dsrect.left=0;
	dsrect.right=sp_width+startx;
	dsrect.bottom=480;
	rcrect.top=0;
	rcrect.left=sp_width+startx;
	rcrect.right=sp_width;
	rcrect.bottom=sp_height;
	}
	//���£��Ҳ����Ƴ�
	else if(640<startx && startx>=0 && startx<=0 && starty==480-sp_width){
	dsrect.top=starty;
	dsrect.left=startx;
	dsrect.right=640;
	dsrect.bottom=480;
	rcrect.top=0;
	rcrect.left=0;
	rcrect.right=640-startx;
	rcrect.bottom=480-starty;
	}
	//Todo:ʣ��3���㷨����Ҫ�ˣ��Ѿ������������ֱ������ң��²����Ƴ������ϣ��Ҳ����Ƴ������ң��ϲ����Ƴ�
	else{
	//	blt_flag=0;
	}
	//alert(debug_position);
	
	if (blt_flag){
		ClearSurface(lpddswork,0,0,0);
		if (FAILED(lpddswork->Blt(
			&dsrect,
			sprite,
			&rcrect,
			DDBLT_KEYSRC | DDBLT_WAIT,
			&ddbltfx
			))){//MessageBox(globalhwnd,"BLT������","",MB_OK);
		}
		BltAlphaMMX( lpddsback, lpddswork,0, 0, NULL, ( int ) iAlpha, 1 );
	}
	return 1;
}