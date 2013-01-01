#include "winGMP.h"
int gwidth  = -1;//scanbitmap的
int gheight = -1;

// storage for our lookup tables一个2D位置查找表，可以用cos_look[度数]来代替cos(度数)来算
float cos_look[361]; // 1 extra element so we can store 0-360 inclusive
float sin_look[361];
///////////////////////////////////////////////////////////

LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(LPDIRECTDRAWSURFACE7 lpdds,
                                         int num_rects,
                                         LPRECT clip_list)

{
// this function creates a clipper from the sent clip list and attaches
// it to the sent surface

int index;                         // looping var
LPDIRECTDRAWCLIPPER lpddclipper;   // pointer to the newly created dd clipper
LPRGNDATA region_data;             // pointer to the region data that contains
                                   // the header and clip list

// first create the direct draw clipper
if (FAILED(lpdd->CreateClipper(0,&lpddclipper,NULL)))
   return(NULL);

// now create the clip list from the sent data

// first allocate memory for region data
region_data = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER)+num_rects*sizeof(RECT));

// now copy the rects into region data
memcpy(region_data->Buffer, clip_list, sizeof(RECT)*num_rects);

// set up fields of header
region_data->rdh.dwSize          = sizeof(RGNDATAHEADER);
region_data->rdh.iType           = RDH_RECTANGLES;
region_data->rdh.nCount          = num_rects;
region_data->rdh.nRgnSize        = num_rects*sizeof(RECT);

region_data->rdh.rcBound.left    =  64000;
region_data->rdh.rcBound.top     =  64000;
region_data->rdh.rcBound.right   = -64000;
region_data->rdh.rcBound.bottom  = -64000;

// find bounds of all clipping regions
for (index=0; index<num_rects; index++)
    {
    // test if the next rectangle unioned with the current bound is larger
    if (clip_list[index].left < region_data->rdh.rcBound.left)
       region_data->rdh.rcBound.left = clip_list[index].left;

    if (clip_list[index].right > region_data->rdh.rcBound.right)
       region_data->rdh.rcBound.right = clip_list[index].right;

    if (clip_list[index].top < region_data->rdh.rcBound.top)
       region_data->rdh.rcBound.top = clip_list[index].top;

    if (clip_list[index].bottom > region_data->rdh.rcBound.bottom)
       region_data->rdh.rcBound.bottom = clip_list[index].bottom;

    } // end for index

// now we have computed the bounding rectangle region and set up the data
// now let's set the clipping list

if (FAILED(lpddclipper->SetClipList(region_data, 0)))
   {
   // release memory and return error
   free(region_data);
   return(NULL);
   } // end if

// now attach the clipper to the surface
if (FAILED(lpdds->SetClipper(lpddclipper)))
   {
   // release memory and return error
   free(region_data);
   return(NULL);
   } // end if

// all is well, so release memory and send back the pointer to the new clipper
free(region_data);
return(lpddclipper);

} // end DDraw_Attach_Clipper

//////////////////////////////////////////////////////////////////////////////////////
int Load_Bitmap_File(HWND hwnd,BITMAP_FILE_PTR bitmap, char *filename){
	int fh,loopindex;//文件句柄和循环变量
	OFSTRUCT fd;//文件打开信息数据结构

	if ((fh = OpenFile(filename,&fd,OF_READ))==-1){
		MessageBox(hwnd,"error open bmp file","error open bmp file",MB_USERICON);
		return 0;
	}
	
	//读出BITMAP_FILE结构里，结构类型为BITMAPFILEHEADER的成员
	_lread(fh, &bitmap->bitmapfileheader,sizeof(BITMAPFILEHEADER));
		// 检查是否是bitmap
	if (bitmap->bitmapfileheader.bfType!=0x4D42)
	   {
	   // close the file
	   _lclose(fh);
		MessageBox(hwnd,"这不是合法的bitmap文件","这不是合法的bitmap文件",MB_USERICON);
	   // return error
	   return(0);
	   } // end if
	
	//读出BITMAP_FILE结构里，结构类型为BITMAPINFOHEADER的成员
	_lread(fh, &bitmap->bitmapinfoheader,sizeof(BITMAPINFOHEADER));
	if (bitmap->bitmapinfoheader.biBitCount == 8){
   _lread(fh, &bitmap->palette,256*sizeof(PALETTEENTRY));
   for (loopindex=0; loopindex < 256; loopindex++)
       {
       // 逆置R索引和B索引，因为BITMAPINFOHEADER如果包含的颜色索引表，他的格式是BGR QUAD,正好是RGB反过来
       int temp_color                = bitmap->palette[loopindex].peRed;
       bitmap->palette[loopindex].peRed  = bitmap->palette[loopindex].peBlue;
       bitmap->palette[loopindex].peBlue = temp_color;
       
       // 设置不要和windows调色板冲突
       bitmap->palette[loopindex].peFlags = PC_NOCOLLAPSE;
       } // end for index
    } // end if

	//然后是图像数据
	_lseek(fh,-(int)(bitmap->bitmapinfoheader.biSizeImage),SEEK_END);
	//读图像，如果图像是8位或者16位的，简单读入，如果是24位的，先读到一个临时区域，然后转换为16位
	if (bitmap->bitmapinfoheader.biBitCount==8 || bitmap->bitmapinfoheader.biBitCount==16 || 
		bitmap->bitmapinfoheader.biBitCount==24)
	   {
	   // 删除最近一次的图像，如果存在的话
	   if (bitmap->buffer)
		   free(bitmap->buffer);
	   // 给图像分配内存
	   if (!(bitmap->buffer = (UCHAR *)malloc(bitmap->bitmapinfoheader.biSizeImage)))
		  {
		   //关闭文件
		  _lclose(fh);
		  MessageBox(hwnd,"分配内存出错了","出错了",MB_APPLMODAL);
		  return(0);
		  } // end if
	   //OK，分配完，读吧
	   _lread(fh,bitmap->buffer,bitmap->bitmapinfoheader.biSizeImage);
	   } // end if
	else
	   {
	   // 严重问题
	   return(0);
	   } // end else
	_lclose(fh);//关闭文件
	//逆置图像，否则是颠倒的
	Flip_Bitmap(bitmap->buffer, 
            bitmap->bitmapinfoheader.biWidth*(bitmap->bitmapinfoheader.biBitCount/8), 
            bitmap->bitmapinfoheader.biHeight);
	return 1;//返回读取成功

}//end Load_Bitmap_File



///////////////////////////////////////////////////////////

int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height)
{
// this function is used to flip bottom-up .BMP images

UCHAR *buffer; // used to perform the image processing
int index;     // looping index

// allocate the temporary buffer
if (!(buffer = (UCHAR *)malloc(bytes_per_line*height)))
   return(0);

// copy image to work area
memcpy(buffer,image,bytes_per_line*height);

// flip vertically
for (index=0; index < height; index++)
    memcpy(&image[((height-1) - index)*bytes_per_line],
           &buffer[index*bytes_per_line], bytes_per_line);

// release the memory
free(buffer);

// return success
return(1);

} // end Flip_Bitmap


///////////////////////////////////////////////////////////
//卸载bitmap
int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap)
{
// this function releases all memory associated with "bitmap"
if (bitmap->buffer)
   {
   // release memory
   free(bitmap->buffer);

   // reset pointer
   bitmap->buffer = NULL;

   } // end if

// return success
return(1);

} // end Unload_Bitmap_File


//创建离屏画面函数
LPDIRECTDRAWSURFACE7 Create_Offscreen_Surface(int w,int h,int memflag){
DDSURFACEDESC2 ddsd;
LPDIRECTDRAWSURFACE7 lpdds;
DDRAW_INIT_STRUCT(ddsd);
ddsd.dwFlags=DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
ddsd.dwWidth=w;
ddsd.dwHeight=h;
ddsd.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN | memflag;//根据menflag来在VRAM或者内存里创建画面
	if (FAILED(lpdd->CreateSurface(&ddsd,&lpdds,NULL))){
	return NULL;
	}
	DDCOLORKEY ck;//设置键色为紫红色
	ck.dwColorSpaceHighValue=_RGB16BIT565(255,0,255);
	ck.dwColorSpaceLowValue=_RGB16BIT565(255,0,255);
	lpdds->SetColorKey(DDCKEY_SRCBLT,&ck);//关联键色到画面
	return lpdds;//返回画面
}

///////////////////////////////////////////////////////////////

LPDIRECTDRAWSURFACE7 DDraw_Create_Surface(int width, int height, int mem_flags, int color_key = 0)
{
// this function creates an offscreen plain surface

DDSURFACEDESC2 ddsd;         // working description
LPDIRECTDRAWSURFACE7 lpdds;  // temporary surface
    
// set to access caps, width, and height
memset(&ddsd,0,sizeof(ddsd));
ddsd.dwSize  = sizeof(ddsd);
ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

// set dimensions of the new bitmap surface
ddsd.dwWidth  =  width;
ddsd.dwHeight =  height;

// set surface to offscreen plain
if (mem_flags==3){
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
}
else
ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | mem_flags;

// create the surface
if (FAILED(lpdd->CreateSurface(&ddsd,&lpdds,NULL))){
	MessageBox(globalhwnd,"创建画面失败","info",MB_OK);
   return(NULL);
}
// test if user wants a color key
if (color_key >= 0)
   {
   // set color key to color 0
   DDCOLORKEY color_key; // used to set color key
   //color_key.dwColorSpaceLowValue  = 0;
   //color_key.dwColorSpaceHighValue = 0;
   color_key.dwColorSpaceLowValue  = _RGB16BIT565(255,0,255);
   color_key.dwColorSpaceHighValue = _RGB16BIT565(255,0,255);
   // now set the color key for source blitting
   lpdds->SetColorKey(DDCKEY_SRCBLT, &color_key);
   } // end if

// return surface
return(lpdds);
} // end DDraw_Create_Surface



///////////////////////////////////////////////////////////////

int DDraw_Draw_Surface(LPDIRECTDRAWSURFACE7 source, // source surface to draw
                      int x, int y,                 // position to draw at
                      int width, int height,        // size of source surface
                      LPDIRECTDRAWSURFACE7 dest,    // surface to draw the surface on
                      int transparent = 1)          // transparency flag
{
// draw a bob at the x,y defined in the BOB
// on the destination surface defined in dest

RECT dest_rect,   // the destination rectangle
     source_rect; // the source rectangle                             

// fill in the destination rect
dest_rect.left   = x;
dest_rect.top    = y;
dest_rect.right  = x+width-1;
dest_rect.bottom = y+height-1;

// fill in the source rect
source_rect.left    = 0;
source_rect.top     = 0;
source_rect.right   = width-1;
source_rect.bottom  = height-1;

// test transparency flag

if (transparent)
   {
   // enable color key blit
   // blt to destination surface
   if (FAILED(dest->Blt(&dest_rect, source,
                     &source_rect,(DDBLT_WAIT | DDBLT_KEYSRC),
                     NULL)))
           return(0);

   } // end if
else
   {
   // perform blit without color key
   // blt to destination surface
   if (FAILED(dest->Blt(&dest_rect, source,
                     &source_rect,(DDBLT_WAIT),
                     NULL)))
           return(0);

   } // end if

// return success
return(1);

} // end DDraw_Draw_Surface




///////////////////////////////////////////////////////////////

int Scan_Image_Bitmap(BITMAP_FILE_PTR bitmap,     // bitmap file to scan image data from
                      LPDIRECTDRAWSURFACE7 lpdds, // surface to hold data
                      int cx, int cy)             // cell to scan image from
{
// this function extracts a bitmap out of a bitmap file

UCHAR *source_ptr,   // working pointers
      *dest_ptr;

DDSURFACEDESC2 ddsd;  //  direct draw surface description 

// get the addr to destination surface memory

// set size of the structure
ddsd.dwSize = sizeof(ddsd);

// lock the display surface
lpdds->Lock(NULL,
            &ddsd,
            DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,
            NULL);

// compute position to start scanning bits from
cx = cx*(ddsd.dwWidth+1) + 1;
cy = cy*(ddsd.dwHeight+1) + 1;

gwidth  = ddsd.dwWidth;
gheight = ddsd.dwHeight;

// extract bitmap data
source_ptr = bitmap->buffer + cy*bitmap->bitmapinfoheader.biWidth+cx;

// assign a pointer to the memory surface for manipulation
dest_ptr = (UCHAR *)ddsd.lpSurface;

// iterate thru each scanline and copy bitmap
for (int index_y=0; index_y < (int)ddsd.dwHeight; index_y++)
    {
    // copy next line of data to destination
    memcpy(dest_ptr, source_ptr, ddsd.dwWidth);

    // advance pointers
    dest_ptr   += (ddsd.lPitch); // (ddsd.dwWidth);
    source_ptr += bitmap->bitmapinfoheader.biWidth;
    } // end for index_y

// unlock the surface 
lpdds->Unlock(NULL);

// return success
return(1);

} // end Scan_Image_Bitmap


#if 0
/////////////////////////////////////////////////////////////////
/* rect collision from D2D gameframework*/
int rectcl (Rect *dest, Rect *src, float oldx, float oldy)
{
    int isbeat = 0;
    float dy, dx0, dx1, dx, dy0, dy1;
    float ix, iy; /* interpolation xy */

    /* right */
    if (oldx+src->w <= dest->x && src->x+src->w > dest->x) {
        /* Y interpolation */
        dy = src->y - oldy;
        dx0 = dest->x-oldx-src->w;
        dx1 = src->x+src->w-dest->x;
        iy = dy * dx0 / (dx0+dx1) + oldy;
        if (iy+src->h > dest->y && iy < dest->y+dest->h) {
            src->x = dest->x - src->w;
            isbeat |= CL_RIGHT;
        }
    }
    else
    if (oldx >= dest->x+dest->w && src->x < dest->x+dest->w) {
        dy = src->y - oldy;
        dx0 = oldx - dest->x-dest->w;
        dx1 = dest->x+dest->w-src->x;
        iy = dy * dx0 / (dx0 + dx1) + oldy;
        if (iy+src->h > dest->y && iy < dest->y+dest->h) {
            src->x = dest->x + dest->w;
            isbeat |= CL_LEFT;
        }
    }
    else
    /* down */
    if (oldy+src->h <= dest->y && src->y+src->h > dest->y) {
        dx = src->x - oldx;
        dy0 = dest->y-oldy-src->h;
        dy1 = src->y+src->h-dest->y;
        ix = dx * dy0 / (dy0+dy1) + oldx;
        if (ix+src->w > dest->x && ix < dest->x+dest->w) {
            src->y = dest->y - src->h;
            isbeat |= CL_DOWN;
        }
    }
    else
    if (oldy >= dest->y+dest->h && src->y < dest->y+dest->h) {
        dx = src->x - oldx;
        dy0 = oldy-dest->y-dest->h;
        dy1 = dest->y+dest->h-src->y;
        ix = dx * dy0 / (dy0+dy1) + oldx;
        if (ix+src->w > dest->x && ix < dest->x+dest->w) {
            src->y = dest->y + dest->h;
            isbeat |= CL_UP;
        }
    }

    return isbeat;
}
#endif

///////////////////////////////////////////////////////////////////////////

int Collision_Test(int x1, int y1, int w1, int h1, 
                   int x2, int y2, int w2, int h2) 
{
// this function tests if the two rects overlap

// get the radi of each rect
int width1  = (w1>>1) - (w1>>3);
int height1 = (h1>>1) - (h1>>3);

int width2  = (w2>>1) - (w2>>3);
int height2 = (h2>>1) - (h2>>3);

// compute center of each rect
int cx1 = x1 + width1;
int cy1 = y1 + height1;

int cx2 = x2 + width2;
int cy2 = y2 + height2;

// compute deltas
int dx = abs(cx2 - cx1);
int dy = abs(cy2 - cy1);

// test if rects overlap
if (dx < (width1+width2) && dy < (height1+height2))
   return(1);
else
// else no collision
return(0);

} // end Collision_Test

//////////////////////////////////////////////////////////////////////////



void Build_Sin_Cos_Tables(void)
{
  
// create sin/cos lookup table
// note the creation of one extra element; 360
// this helps with logic in using the tables

// generate the tables 0 - 360 inclusive
for (int ang = 0; ang <= 360; ang++)
    {
    // convert ang to radians
    float theta = (float)ang*PI/(float)180;

    // insert next entry into table
    cos_look[ang] = cos(theta);
    sin_look[ang] = sin(theta);

    } // end for ang

} // end Build_Sin_Cos_Tables



////////////////////////////////////////////////////////////////////////////

int Fast_Distance_2D(int x, int y)
{
// this function computes the distance from 0,0 to x,y with 3.5% error

// first compute the absolute value of x,y
x = abs(x);
y = abs(y);

// compute the minimum of x,y
int mn = MIN(x,y);

// return the distance
return(x+y-(mn>>1)-(mn>>2)+(mn>>4));

} // end Fast_Distance_2D