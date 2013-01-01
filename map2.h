
//这是gamedev的Game Programming GenesisPart VIII : Basic Tile Engines 的TILE引擎
#ifndef INCLUDE_ME_ONLY_ONCE_OR_DIE2
#define INCLUDE_ME_ONLY_ONCE_OR_DIE2
//macro
#define KEYSTATE(key) ((GetAsyncKeyState(key) & 0x8000) ? TRUE : FALSE)
//To do:下面这全局指针要encapsulate
HINSTANCE hMainInstance;


//LPDIRECTDRAWSURFACE7 lpddsBack = NULL;
LPDIRECTDRAWSURFACE7 lpddsTileset = NULL;

//extern DDBLTFX ddbltfx;

// DATA TYPES
//TILE结构
//##ModelId=4A1E88FB0378
typedef struct TILE_type
{
    RECT rcLocation;    // location on DirectDraw surface
    int bWalkOK;        // can the tile be walked on?
    int nAnimSpeed;     // animation speed in frames
    DWORD dwFlags;      // approach flags
    TILE_type *lpNext;  // next tile in animation
	UINT uiFrames;      // frame counter
} TILE, FAR* LPTILE;

// Since we're not dealing with NPCs or scripts just yet, I've
// commented those fields out of the map data structure.
//地图结构
//##ModelId=4A1E88FB0387
#ifndef INCLUDE_MAPDATA_TYPE
#define INCLUDE_MAPDATA_TYPE
//##ModelId=4A1E88FB0387
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
// CONSTANTS
//#define NUM_TILES        20
const int NUM_TILES=1000;//Todo:根据实际读取来计算！
//const int TILESET_WIDTH=20;
//#define TILESET_HEIGHT  222

// tile engine
BYTE byMap[3000][3000][3];         // the actual tiles暂时先改成300格*300格的地图
MAPDATA mapdata;                 // the other map data
TILE tileData[NUM_TILES];
LPTILE lptile[NUM_TILES];



// PROTOTYPES
int Init_Map(int);
void Render_Map(LPDIRECTDRAWSURFACE *);
//int Load_Bitmap(char *lpszName, int x, int y, LPDIRECTDRAWSURFACE7 lpdds);
void Shutdown_Tileset(void);
///////////////////////////////////////////////////////////////
// INIT_MAP
// Loads the map, fills out the map data structure, and sets up
// the tile data structures. Be sure to read my comments here.
///////////////////////////////////////////////////////////////
int Init_Map(int stagenum)
{
	
	FILE *pfile;
	int x, y;
	
	// first open the map

	char *themap;
	if (stagenum==0)
	{
		//MessageBeep(MB_CUR_MAX);
		themap="Map/testmap.dat";
	}
	else if (stagenum==1)
	{
		//MessageBeep(MB_ICONASTERISK);
		themap="Map/testmap1.dat";
	}
	
	if ((pfile = fopen(themap, "rb")) == NULL){
		MessageBox(globalhwnd,"文件不存在","",MB_OK);
		return(FALSE);
	}

	// read the map dimensions
#if 1	
	//int xmax,ymax;
	char s[100];//包含源
	char thestr[20];
	char buf[300];
	//数据格式 mapH100V200|
	//先获取100个字符，里面包含“地图横格子数|地图竖格子数$”的字符串
	fgets(s,100,pfile);
	//fclose(pfile);
	fseek(pfile, 0,SEEK_SET);
	//const char* s = "mapH100V200|";
	sscanf( s, "%*[^H]H%[^V]", thestr );
	sprintf(buf,"%s",thestr);
	//MessageBox(globalhwnd,buf,"",MB_OK);
	mapdata.xMax=atoi(thestr)-1;//因为下标是从0开始，故减去1
	sscanf( s, "%*[^V]V%[^|]", thestr );
	sprintf(buf,"%s",thestr);
	//MessageBox(globalhwnd,buf,"",MB_OK);	
	mapdata.yMax=atoi(thestr)-1;
	int cut_str_num;//之后部分的就是地图CELL数据了
	///计算非CELL数据区域的字符数量！比如MAP:H112V123|这些字符的数量！
	int tmp_mapdata_xMax=mapdata.xMax;
	int tmp_mapdata_yMax=mapdata.yMax;
	int   weishu   =   1;  
	while(1){  
		tmp_mapdata_xMax   /=   10;  
		if   (tmp_mapdata_xMax){  
			weishu++;  
		}  
		else{  
			break;  
		}  
	}   
	while(1){  
		tmp_mapdata_yMax   /=   10;  
		if   (tmp_mapdata_yMax){  
			weishu++;  
		}  
		else{  
			break;  
		}  
	}   


	cut_str_num=7+weishu;//MAP:HV|是7位数
//#ifdef REMOTEDEBUG
//	if ((pfile = fopen("c:/testmap.dat", "rb")) == NULL)
//		return(FALSE);
//#else
//	if ((pfile = fopen("testmap.dat", "rb")) == NULL)
//		return(FALSE);
//#endif
	for(int i=0;i<13;i++){
		getc(pfile);//切掉这些字符
	}
#endif
//	getc(pfile);getc(pfile);getc(pfile);getc(pfile);
	//mapdata.xMax = getc(pfile);
	//mapdata.yMax = getc(pfile);


	/*char buf[30];
	sprintf(buf,"%d",mapdata.xMax);
	MessageBox(globalhwnd,buf,"",MB_OK);	
	sprintf(buf,"%d",mapdata.yMax);
	MessageBox(globalhwnd,buf,"",MB_OK);*/
#if 0
	mapdata.xMax=299;
	mapdata.yMax=299;
#endif

	// now read the actual map data and close the file
	for (x=0; x<=mapdata.xMax; x++)
	{
		for (y=0; y<=mapdata.yMax; y++)
		{
			byMap[x][y][0] = getc(pfile);
			byMap[x][y][1] = getc(pfile);
			byMap[x][y][2] = getc(pfile);
		}
	}

	fclose(pfile);
//##########调试！！！###########
//alert(mapdata.xMax);//59
	// compute the maximum camera values
	//mapdata.xMaxCamera = (mapdata.xMax - 19) * 32;
mapdata.xMaxCamera = (mapdata.xMax - 19) * 32;
	if (mapdata.xMaxCamera < 0)
		mapdata.xMaxCamera = 0;
//##########调试！！！###########
//alert(mapdata.yMax);//59
	mapdata.yMaxCamera = (mapdata.yMax - 14) * 32;
	if (mapdata.yMaxCamera < 0)
		mapdata.yMaxCamera = 0;
	
	// set the camera at the center of the map
	mapdata.xCamera = mapdata.xMaxCamera / 2;
	mapdata.yCamera = mapdata.yMaxCamera / 2;
	//重新设置主角起点
	mapdata.xCamera = 0;
	mapdata.yCamera = 11000;
	// IMPORTANT - set the tile pointers initially pointing
	// at their corresponding tile structures
//##########调试！！！###########
	//alert(NUM_TILES);//12
	for (x=0; x<NUM_TILES; x++)
		lptile[x] = &tileData[x];
	
	// Now we're going to set up the tile structures, but this is not normally
	// how it would be done. For an actual game tileset, which can get quite
	// large, hard-coding all this information is a very bad idea. What you'd
	// actually want to do is to create a data file that contains all of this
	// information, so the game can just open the data file and read in what
	// it needs. Because this example tileset is so small, and to keep things
	// easy to understand (i.e. not having tons of data files), I'm just going
	// to fill out the structures here.
	for (x=0; x<NUM_TILES; x++)
	{
		// set all the RECTs on the surface
		//左上右下
		//RECT rcTemp = {0, x*32, 32, x*32+32};
		RECT rcTemp = { x*32,0, x*32+32,32 };
		tileData[x].rcLocation = rcTemp;
		
		// reset the frame counter
		tileData[x].uiFrames = 0;
	}
#if 1
	//TILE动画链表
	// set up the animation chain for the water
	/*tileData[4].lpNext = &tileData[5]; tileData[4].nAnimSpeed = 30;
	tileData[5].lpNext = &tileData[6]; tileData[5].nAnimSpeed = 3;
	tileData[6].lpNext = &tileData[7]; tileData[6].nAnimSpeed = 3;
	tileData[7].lpNext = &tileData[4]; tileData[7].nAnimSpeed = 3;*/
	if (stagenum==0){
		tileData[78].lpNext = &tileData[79]; tileData[78].nAnimSpeed = 10;
		tileData[79].lpNext = &tileData[80]; tileData[79].nAnimSpeed = 10;
		tileData[80].lpNext = &tileData[81]; tileData[80].nAnimSpeed = 10;
		tileData[81].lpNext = &tileData[82]; tileData[81].nAnimSpeed = 10;
		tileData[82].lpNext = &tileData[83]; tileData[82].nAnimSpeed = 10;
		tileData[83].lpNext = &tileData[84]; tileData[83].nAnimSpeed = 10;
		tileData[84].lpNext = &tileData[85]; tileData[84].nAnimSpeed = 10;
		tileData[85].lpNext = &tileData[86]; tileData[85].nAnimSpeed = 10;
		tileData[86].lpNext = &tileData[87]; tileData[86].nAnimSpeed = 10;
		tileData[87].lpNext = &tileData[78]; tileData[87].nAnimSpeed = 10;
		
		tileData[191].lpNext = &tileData[195]; tileData[191].nAnimSpeed = 50;
		tileData[195].lpNext = &tileData[191]; tileData[195].nAnimSpeed = 50;
		tileData[192].lpNext = &tileData[196]; tileData[192].nAnimSpeed = 50;
		tileData[196].lpNext = &tileData[192]; tileData[196].nAnimSpeed = 50;
		tileData[193].lpNext = &tileData[197]; tileData[193].nAnimSpeed = 50;
		tileData[197].lpNext = &tileData[193]; tileData[197].nAnimSpeed = 50;
		tileData[194].lpNext = &tileData[198]; tileData[194].nAnimSpeed = 50;
		tileData[198].lpNext = &tileData[194]; tileData[198].nAnimSpeed = 50;
	}
	else{
		tileData[78].lpNext = &tileData[78]; tileData[78].nAnimSpeed = 10;
		tileData[79].lpNext = &tileData[79]; tileData[79].nAnimSpeed = 10;
		tileData[80].lpNext = &tileData[80]; tileData[80].nAnimSpeed = 10;
		tileData[81].lpNext = &tileData[81]; tileData[81].nAnimSpeed = 10;
		tileData[82].lpNext = &tileData[82]; tileData[82].nAnimSpeed = 10;
		tileData[83].lpNext = &tileData[83]; tileData[83].nAnimSpeed = 10;
		tileData[84].lpNext = &tileData[84]; tileData[84].nAnimSpeed = 10;
		tileData[85].lpNext = &tileData[85]; tileData[85].nAnimSpeed = 10;
		tileData[86].lpNext = &tileData[86]; tileData[86].nAnimSpeed = 10;
		tileData[87].lpNext = &tileData[87]; tileData[87].nAnimSpeed = 10;
		
		tileData[191].lpNext = &tileData[191]; tileData[191].nAnimSpeed = 50;
		tileData[195].lpNext = &tileData[195]; tileData[195].nAnimSpeed = 50;
		tileData[192].lpNext = &tileData[192]; tileData[192].nAnimSpeed = 50;
		tileData[196].lpNext = &tileData[196]; tileData[196].nAnimSpeed = 50;
		tileData[193].lpNext = &tileData[193]; tileData[193].nAnimSpeed = 50;
		tileData[197].lpNext = &tileData[197]; tileData[197].nAnimSpeed = 50;
		tileData[194].lpNext = &tileData[194]; tileData[194].nAnimSpeed = 50;
		tileData[198].lpNext = &tileData[198]; tileData[198].nAnimSpeed = 50;
	}




#endif
	// bWalkOK and dwFlags are not important since there are no characters used
	// in this demo, so we don't even bother to set them here.
	
	// return success
	return(TRUE);
}

//////////////////////////////////////////////////////////////////////
// RENDER_MAP
// Draws the map on the back buffer.
//////////////////////////////////////////////////////////////////////
void Render_Map(LPDIRECTDRAWSURFACE7 lpdds,LPDIRECTDRAWSURFACE7 lpdds_dst)
{
	BYTE byTile;
	
	// set original destination RECT for first tile -- aligned with the
	// upper-left corner of the screen
	RECT rcDest = {0, 0, 32, 32};
	
	// find default tile range -- divide camera coordinates by 32 and
	// use the default of 21 tiles across and 16 tiles down
	int xStart = mapdata.xCamera >> 5;//除以32
	int yStart = mapdata.yCamera >> 5;
	int xEnd = xStart + 20;
	int yEnd = yStart + 15;
	int x, y;
	
	// now check if the camera coordinates are divisible by 32
	x = mapdata.xCamera & 0x0000001F;
	y = mapdata.yCamera & 0x0000001F;
	
	if (!x)
	{
		// if xCamera is divisible by 32, use only 20 tiles across
		xEnd--;
	}
	else
	{
		// otherwise move destination RECT to the left to clip the first column of tiles
		rcDest.left-=x;
		rcDest.right-=x;
	}
	
	if (!y)
	{
		// if yCamera is divisible by 32, use only 15 tiles down
		yEnd--;
	}
	else
	{
		// otherwise move destination RECT up to clip the first row of tiles
		rcDest.top-=y;
		rcDest.bottom-=y;
	}
	
	// finally make sure we're not exceeding map limits
	if (xEnd > mapdata.xMax)
		xEnd = mapdata.xMax;
	if (yEnd > mapdata.yMax)
		yEnd = mapdata.yMax;
	
	// store original rcDest RECT
	RECT rcTemp;
	rcTemp = rcDest;
#if 1
	RECT ss;
	ss.top=0;
	ss.right=32;
	ss.bottom=32;
	ss.left=0;
	

/*	if(FAILED(lpdds_dst->Blt(&rcDest, lpdds, &lptile[byTile]->rcLocation, DDBLT_WAIT, NULL))){
	MessageBox(NULL,"Blt failed!","caution",MB_OK);
	}*/
#endif
	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue=_RGB16BIT565(255,0,255);
	ddck.dwColorSpaceHighValue=_RGB16BIT565(255,0,255);
	lpdds->SetColorKey(DDCKEY_SRCBLT,&ddck);
	// plot the first layer画第一层（地面层）
	for (x=xStart; x<=xEnd; x++)
	{
		for (y=yStart; y<=yEnd; y++)
		{
			// blit the tile画出tile
			byTile = byMap[x][y][0];

		if(FAILED(lpdds_dst->Blt(&rcDest, lpdds, &lptile[byTile]->rcLocation, DDBLT_WAIT|DDBLT_KEYSRC, NULL))){
//	MessageBox(NULL,"Blt failed!","caution",MB_OK);
		}
				//////////////////////////////////////////////////////////////////////////
			// blit the tile if necessaryBLT需要的层
			byTile = byMap[x][y][1];
			if (byTile > 0)//注意-1的就是没有TILE的CELL
			{
			/*if(FAILED(lpdds_dst->Blt(&rcDest, lpdds, &lptile[byTile]->rcLocation, DDBLT_WAIT, NULL))){
			//MessageBox(NULL,"Blt failed!","caution",MB_OK);
			}*/
				//改成半透明的
				if(FAILED(lpdds_dst->Blt(&rcDest, lpdds, &lptile[byTile]->rcLocation,DDBLT_WAIT|DDBLT_KEYSRC, NULL))){
					MessageBox(NULL,"Blt failed!","caution",MB_OK);
				}//endif
			}//endif
		byTile = byMap[x][y][2];
		if (byTile > 0)//注意-1的就是没有TILE的CELL
		{
		/*if(FAILED(lpdds_dst->Blt(&rcDest, lpdds, &lptile[byTile]->rcLocation, DDBLT_WAIT, NULL))){
		//MessageBox(NULL,"Blt failed!","caution",MB_OK);
		}*/
			//改成半透明的
			if(FAILED(lpdds_dst->Blt(&rcDest, lpdds, &lptile[byTile]->rcLocation,DDBLT_WAIT|DDBLT_KEYSRC, NULL))){
				MessageBox(NULL,"Blt failed!","caution",MB_OK);
			}//endif
		}//endif
		//////////////////////////////////////////////////////////////////////////
			/*if(FAILED(
				lpdds_dst->BltFast(
				rcDest.left,
				rcDest.top,
				lpdds,
				&lptile[byTile]->rcLocation,
				DDBLTFAST_WAIT
				)	)	)
				MessageBox(globalhwnd,"BltFastTile失败","bug",MB_OK);*/

			// advance rcDest RECT
			rcDest.bottom += 32;
			rcDest.top += 32;
		}


		
		// reset rcDest RECT to top of next column设置RECT到下以列的头上
		rcDest.left += 32;
		rcDest.right += 32;
		rcDest.bottom -= ((yEnd - yStart + 1) << 5);
		rcDest.top -= ((yEnd - yStart + 1) << 5);
	}
	
	// restore the destination RECT恢复目的地RECT
	rcDest = rcTemp;
	/*testcode
	byMap[0][0][1]=3;
	*/
	
//#define _RGB16BIT565(r,g,b)((b%32)+((g%64)<<6)+((r%32)<<11))
//必须设置一次colorkey,不然Blt失败，不是之前有设置过吗？http://bbs.pfan.cn/post-153724.html

#if 1
	///
	// plot the second layer画第二层(物件层1)
	for (x=xStart; x<=xEnd; x++)
	{
		for (y=yStart; y<=yEnd; y++)
		{
			// blit the tile if necessaryBLT需要的层
			byTile = byMap[x][y][1];
			if (byTile > 0)//注意-1的就是没有TILE的CELL
			{
			/*if(FAILED(lpdds_dst->Blt(&rcDest, lpdds, &lptile[byTile]->rcLocation, DDBLT_WAIT, NULL))){
			//MessageBox(NULL,"Blt failed!","caution",MB_OK);
			}*/
				//改成半透明的
				if(FAILED(lpdds_dst->Blt(&rcDest, lpdds, &lptile[byTile]->rcLocation,DDBLT_WAIT|DDBLT_KEYSRC, NULL))){
					//MessageBox(NULL,"Blt failed!","caution",MB_OK);
				}//endif
			}//endif
			// advance rcDest RECT
			rcDest.bottom += 32;
			rcDest.top += 32;
		}//endfor
		byTile = byMap[x][y][2];
		if (byTile > 0)//注意-1的就是没有TILE的CELL
		{
		/*if(FAILED(lpdds_dst->Blt(&rcDest, lpdds, &lptile[byTile]->rcLocation, DDBLT_WAIT, NULL))){
		//MessageBox(NULL,"Blt failed!","caution",MB_OK);
		}*/
			//改成半透明的
			if(FAILED(lpdds_dst->Blt(&rcDest, lpdds, &lptile[byTile]->rcLocation,DDBLT_WAIT|DDBLT_KEYSRC, NULL))){
				MessageBox(NULL,"Blt failed!","caution",MB_OK);
			}//endif
		}//endif
		
		// reset rcDest RECT to top of next column设置RECT到下以列的头上
		rcDest.left += 32;
		rcDest.right += 32;
		rcDest.bottom -= ((yEnd - yStart + 1) << 5);
		rcDest.top -= ((yEnd - yStart + 1) << 5);
	}//endfor
	///
#endif

	rcDest = rcTemp;
#if 0
	///
	// plot the second layer画第三层(物件层2)
	for (x=xStart; x<=xEnd; x++)
	{
		for (y=yStart; y<=yEnd; y++)
		{
			// blit the tile if necessaryBLT需要的层
			byTile = byMap[x][y][2];
			if (byTile > 0)//注意-1的就是没有TILE的CELL
			{
			/*if(FAILED(lpdds_dst->Blt(&rcDest, lpdds, &lptile[byTile]->rcLocation, DDBLT_WAIT, NULL))){
			//MessageBox(NULL,"Blt failed!","caution",MB_OK);
			}*/
				//改成半透明的
				if(FAILED(lpdds_dst->Blt(&rcDest, lpdds, &lptile[byTile]->rcLocation,DDBLT_WAIT|DDBLT_KEYSRC, NULL))){
					MessageBox(NULL,"Blt failed!","caution",MB_OK);
				}//endif
			}//endif
			// advance rcDest RECT
			rcDest.bottom += 32;
			rcDest.top += 32;
		}//endfor
		
		// reset rcDest RECT to top of next column设置RECT到下以列的头上
		rcDest.left += 32;
		rcDest.right += 32;
		rcDest.bottom -= ((yEnd - yStart + 1) << 5);
		rcDest.top -= ((yEnd - yStart + 1) << 5);
	}//endfor
	///
#endif

}

#if 0//这个函数常用，放到了其他文件暂时叫function了。
//////////////////////////////////////////////////////////////////////
// LOAD_BITMAP
// Loads a bitmap resource onto the specified DirectDraw surface.
//////////////////////////////////////////////////////////////////////
int Load_Bitmap(char *lpszName, int x, int y, LPDIRECTDRAWSURFACE7 lpdds)
{
	HBITMAP hbitmap = NULL;
	BITMAP bmp;
	HDC hdcImage, hdcSurface;
	
	// load bitmap resource
	if (!(hbitmap = (HBITMAP)LoadImage(hMainInstance, lpszName, 
		IMAGE_BITMAP, 0, 0, 
		LR_CREATEDIBSECTION)))
		return(FALSE);
	
	// create device context for image
	if (!(hdcImage = CreateCompatibleDC(NULL)))
		return(FALSE);
	
	// select bitmap into the DC
	if (!(SelectObject(hdcImage, hbitmap)))
		return(FALSE);
	
	// get surface device context
	if (FAILED(lpdds->GetDC(&hdcSurface)))
		return(FALSE);
	
	// get image dimensions
	GetObject(hbitmap, sizeof(BITMAP), &bmp);
    int dx = bmp.bmWidth;
    int dy = bmp.bmHeight;
	
	// copy image to surface
	if (!(BitBlt(hdcSurface, x, y, dx, dy, hdcImage, 0, 0, SRCCOPY)))
		return(FALSE);
	
	// get rid of all that stuff we used
	lpdds->ReleaseDC(hdcSurface);
	DeleteDC(hdcImage);
	DeleteObject(hbitmap);
	
	return(TRUE);
}
#endif

void Shutdown_Tileset(void)//改造了下
{
	if (lpddsTileset)
	{
		// a pox on the tileset surface!
		lpddsTileset->Release();
		lpddsTileset = NULL;
	}
}

#endif
