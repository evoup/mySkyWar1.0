#include "myengine.h"




//剪切像素（blt像素）
void mybltclip(HWND hwnd,USHORT * lpsurface,int mempitch){//To do 解决alt+f4退出报错，应该是game_shutdown
	//MessageBox(hwnd,"test","test",strlen("test"));
	//绘制像素的方法,这个是16位的方法
	lpsurface[100+220*(mempitch>>1)]=_RGB16BIT565(rand()%256, rand()%256, rand()%256);
}


///////////////////////////////////////////////////////////



//剪切位图（blt软件版）
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
	return;//先检查是不是完全不在用户区


	// 一些赋值
	int x1 = x;
	int y1 = y;
	int x2 = x1 + width - 1;
	int y2 = y1 + height -1;

	// 左上角剪切判断
	if (x1 < 0)
	   x1 = 0;

	if (y1 < 0)
	   y1 = 0;

	// 右下角剪切判断
	if (x2 >= 640)
		x2 = 640-1;

	if (y2 >= 480)
		y2 = 480-1;

	//MessageBox(hwnd,"test","test",strlen("test"));
/*	HDC xdc;//打印一些调试信息
	if (FAILED(lpddsprimary->GetDC(&xdc))){
		//echo error
		}
	SetTextColor(xdc,RGB(0,0,rand()%255));

	TextOut(xdc,40,400,"如何传递变量到这个字符串",strlen("如何传递变量到这个字符串"));
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

//一个防止ab出界并且和精灵类整合的函数
bool myBAMMX(LPDIRECTDRAWSURFACE7 lpddswork,LPDIRECTDRAWSURFACE7 lpddsback,int startx,int starty,
			 int iAlpha,SPRITE_OBJ * whichsp,LPDIRECTDRAWSURFACE7 sprite){
	//先查询精灵的长宽
	int sp_width=whichsp->theframe_width;
	int sp_height=whichsp->theframe_height;
	//int startx,starty;
//有错误的坐标
	//startx=437;
	//starty=-8;

	//部分上的
	/*startx=400;//blt目的地起始点
	starty=-83;*/
	//均部分移出，左上的
	/*startx=-83;//blt目的地起始点
	starty=-83;*/
	//仅左部分移出
	//startx=-113;//blt目的地起始点
	//starty=123;
	//均移出部分，左下
	/*startx=-111;//blt目的地起始点
	starty=400;*/
	//仅下部分移出
	/*startx=100;//blt目的地起始点
	starty=400;*/
	//均移出部分，右下
	/*startx=560;//blt目的地起始点
	starty=400;*/
	//仅右部移出
	/*startx=560;//blt目的地起始点
	int starty=100;*/
	//均移出部分，右上
	/*startx=520;//blt目的地起始点
	int starty=-80;*/
	//如果精灵在屏幕内（包括正好撑满）
	/*startx=100;//blt目的地起始点
	starty=200;*/

	int blt_flag;//是否执行blt标记
	blt_flag=1;
	int debug_position;//调试用的方位标记
	debug_position=-999;
	//防止AB运算出界，先拷贝到工作画面！
	RECT dsrect,rcrect;
#if 1
	dsrect.top=starty;//屏幕的上位置
	dsrect.left=startx;//屏幕的左位置
	dsrect.right=startx+sp_width;//屏幕的右位置，right-left没有超过精灵宽度的最大值
	dsrect.bottom=starty+sp_height;//屏幕的下位置，bottom-top这样没有超过精灵的高度最大值
	rcrect.top=0;//精灵的上位置
	rcrect.left=0;//精灵的左位置
	rcrect.right=sp_width;//精灵的右位置，right-left=精灵的宽度的话就是精灵的宽度了。
	rcrect.bottom=sp_height;//精灵的下位置，如果精灵的下位置是精灵的高度减少x的话，那么要blt的区
	//域就减少了，这样就是要修正目标Blt矩形的区域了。关键在于移出屏幕的数值，一定在在src里减去相应的，
	//而且如果达到的精灵的长宽，就不要blt了，这样就对了。此外des坐标绝对不可以超过屏幕范围。
#endif

	//以下调试可以使用function.cpp的alert函数打印出整数
	//逆时针开始的8个方向的自定义surface的clip算法
	//部分上
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
	//均部分移出，左上
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
	//仅左部分移出
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
	//均移出部分，左下
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
	//仅下部分移出
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
	//均移出部分，右下
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
	//仅右部移出
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
	//均移出部分，右上
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
	//如果精灵在屏幕内（包括正好撑满）
	else if(480-starty>=sp_height && 640-startx>=sp_width && startx>=0 && starty>=0){
		//do blt
		debug_position=99;
	}
	//另增8个贴边移出部分要切掉
	//如果贴上，左部分移出
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
	//如果贴左，上部分移出
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
	//贴左，下部分移出
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
	//如果贴下，左部分移出
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
	//贴下，右部分移出
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
	//Todo:剩余3个算法不需要了，已经工作正常。分别是贴右，下部分移出；贴上，右部分移出；贴右，上部分移出
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
			))){//MessageBox(globalhwnd,"BLT！出错","",MB_OK);
		}
		BltAlphaMMX( lpddsback, lpddswork,0, 0, NULL, ( int ) iAlpha, 1 );
	}
	return 1;
}