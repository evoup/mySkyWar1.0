#include "function.h"


//from gamedev.net tileengine demo load bitmap to surface function
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
	//HBITMAP hbmp = (HBITMAP)::LoadImage( AfxGetInstanceHandle(),
	//	lpszresourcename,IMAGE_BITMAP,0,0,
	//	LR_LOADFROMFILE); //װ��λͼ
	if (!(hbitmap = (HBITMAP)LoadImage(hMainInstance, lpszName, 
		                               IMAGE_BITMAP, 0, 0, 
									   LR_LOADFROMFILE|LR_CREATEDIBSECTION))){
		return(FALSE);
	}

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

//from gamedev.net tileengine demo load bitmap to surface function
//////////////////////////////////////////////////////////////////////
// LOAD_BITMAP
// Loads a bitmap resource onto the specified DirectDraw surface.
//////////////////////////////////////////////////////////////////////
int Load_Bitmap256(char *lpszName, int x, int y, LPDIRECTDRAWSURFACE7 lpdds)
{
	HBITMAP hbitmap = NULL;
	BITMAP bmp;
	HDC hdcImage, hdcSurface;

	// load bitmap resource
	//HBITMAP hbmp = (HBITMAP)::LoadImage( AfxGetInstanceHandle(),
	//	lpszresourcename,IMAGE_BITMAP,0,0,
	//	LR_LOADFROMFILE); //װ��λͼ
	if (!(hbitmap = (HBITMAP)LoadImage(hMainInstance, lpszName, 
		                               IMAGE_BITMAP, 0, 0, 
									   LR_LOADFROMFILE|LR_CREATEDIBSECTION))){
		return(FALSE);
	}

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
	//lpdds->GetPalette(&globallpddpal);
	//HPALETTE palette;
	//GetBitmapandPalette("e:\\project\ showimage\\bitmap1.bmp",bmp,palette ,&nWidth,&nHeight);
	//GetBitmapandPalette(lpszName,bmp,palette,dx,dy);
	//	SelectPalette(hdcImage,&palette,TRUE);
	//	RealizePalette(hdcImage);
	//	SetPalette(globallpddpal);
	//hOldPal = SelectPalette (hDC, hPal, FALSE);

	// copy image to surface
	if (!(BitBlt(hdcSurface, x, y, dx, dy, hdcImage, 0, 0, SRCCOPY)))
		return(FALSE);

	// get rid of all that stuff we used
	lpdds->ReleaseDC(hdcSurface);
	DeleteDC(hdcImage);
	DeleteObject(hbitmap);

	return(TRUE);
}

//void testx(){
//MessageBox(NULL,"","",MB_OK);
//}


//from internet��Ҫ�ο�����VC5����ʾ256ɫλͼ�� �����ο��������WindowsӦ�ó�������ʾ 256 ɫ��λͼ��
//������MFC�ĳ�SDK��
//�������ܣ�����λͼSRC��������ɫ�壬λͼ���ȣ����
//[in]lpszresource,bitmap
//[out]&pal,*w,*h
bool GetBitmapandPalette(LPCTSTR lpszresource,HBITMAP &bitmap, HPALETTE &pal,long *w,long *h){
	LPCTSTR lpszresourcename = (LPCTSTR)lpszresource;
	HBITMAP hbmp = (HBITMAP)::LoadImage( hMainInstance,//�ⲿ����
		lpszresourcename,IMAGE_BITMAP,0,0,
		LR_LOADFROMFILE); //װ��λͼ
	if( hbmp == NULL )
		return FALSE;

		/*	bitmap.Attach(hbmp);
	// ����λͼ��һ��CBitmap������ϵ����*/
	
	DIBSECTION ds;
	BITMAPINFOHEADER &bminfo = ds.dsBmih;
	/*bitmap.GetObject( sizeof(ds), &ds );*/
	GetObject(bitmap,sizeof(ds),&ds);
		
	// �õ�λͼ��ɫ��
	int ncolors=bminfo.biClrUsed?bminfo.biClrUsed:1<<bminfo.biBitCount;
	*w=bminfo.biWidth; //�õ�λͼ���ֵ
	*h=bminfo.biHeight; //�õ�λͼ�߶�ֵ
	/*CClientDC dc(NULL); // �����߼���ɫ��*/
	HDC dc;
	dc=GetDC(globalhwnd);
	if( ncolors> 256 )
		pal=CreateHalftonePalette(dc);
	else
	{
		// ��ɫ�� <= 256
		RGBQUAD *prgb=new RGBQUAD[ncolors];
		HDC memdc;
		memdc=CreateCompatibleDC(dc);
		SelectObject(memdc,bitmap );
		::GetDIBColorTable( memdc, 0, ncolors, prgb );
		UINT nsize=sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * ncolors);
		//�߼���ɫ��
		//LOGPALETTE *plp=(LOGPALETTE *) new byte[nsize];
		LOGPALETTE *plp = (LOGPALETTE *)malloc( sizeof(LOGPALETTE) + (256*sizeof(PALETTEENTRY)) );
		plp->palVersion=0x300;//MessageBox(globalhwnd,"MessageBox��ɱ���Է�","",MB_OK);
		plp->palNumEntries = ncolors;
		for (int i=0;i<ncolors;i++){
			plp->palPalEntry[i].peRed = prgb[i].rgbRed;
			plp->palPalEntry[i].peGreen = prgb[i].rgbGreen;
			plp->palPalEntry[i].peBlue = prgb[i].rgbBlue;
			plp->palPalEntry[i].peFlags = 0;
		}
		pal=CreatePalette(plp);
		delete[] plp;
		delete[] prgb;
	}
	ReleaseDC(globalhwnd,dc);
	return TRUE;
}

//���Զ�ȡ256ɫλͼ������ĺ�������Ҫ�ȵ���GetBitmapandPalette����
bool Loadbmp2Surface(char *lpszName, int x, int y, LPDIRECTDRAWSURFACE7 lpdds)
{
	HBITMAP hbmp;
	HPALETTE hpal;
	LONG w;
	LONG h;
	GetBitmapandPalette(lpszName,hbmp,hpal,&w,&h);
	//�����͵õ��˵�ɫ����



	HBITMAP hbitmap = NULL;
	BITMAP bmp;
	HDC hdcImage, hdcSurface;

	// load bitmap resource
	//HBITMAP hbmp = (HBITMAP)::LoadImage( AfxGetInstanceHandle(),
	//	lpszresourcename,IMAGE_BITMAP,0,0,
	//	LR_LOADFROMFILE); //װ��λͼ
	if (!(hbitmap = (HBITMAP)LoadImage(hMainInstance, lpszName, 
		                               IMAGE_BITMAP, 0, 0, 
									   LR_LOADFROMFILE|LR_CREATEDIBSECTION))){
		return(FALSE);
	}

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
	//lpdds->GetPalette(&globallpddpal);
	//HPALETTE palette;
	//GetBitmapandPalette("e:\\project\ showimage\\bitmap1.bmp",bmp,palette ,&nWidth,&nHeight);
	//GetBitmapandPalette(lpszName,bmp,palette,dx,dy);
		SelectPalette(hdcImage,hpal,TRUE);
		RealizePalette(hdcImage);

	//hOldPal = SelectPalette (hDC, hPal, FALSE);

	// copy image to surface
	if (!(BitBlt(hdcSurface, x, y, dx, dy, hdcImage, 0, 0, SRCCOPY)))
		return(FALSE);
	
	// get rid of all that stuff we used
	lpdds->ReleaseDC(hdcSurface);
	DeleteDC(hdcImage);
	DeleteObject(hbitmap);

	return(TRUE);
}


//####################################
//from gamedev new 4 function
//####################################
/*
 * Function to create an offscreen surface and load a bitmap from
 * disk into it. The szBitmap field is the filename of the Bitmap.
 */
IDirectDrawSurface7 * DDLoadBitmap(IDirectDraw7 *pdd, LPCSTR szBitmap)
{
    HBITMAP hbm;
    BITMAP bm;
    IDirectDrawSurface7 *pdds;

    // LoadImage has some added functionality in Windows 95 that allows
    // you to load a bitmap from a file on disk.
    hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0,
    LR_LOADFROMFILE|LR_CREATEDIBSECTION);

    if (hbm == NULL)
        return NULL;

    GetObject(hbm, sizeof(bm), &bm); // get size of bitmap

   /*
    * create a DirectDrawSurface for this bitmap
    * source to function CreateOffScreenSurface() follows immediately
    */

    pdds = CreateOffScreenSurface(pdd, bm.bmWidth, bm.bmHeight);
    if (pdds) {
        DDCopyBitmap(pdds, hbm, bm.bmWidth, bm.bmHeight);
    }
	

    DeleteObject(hbm);

    return pdds;
}


//V2.0����λͼ�����棬���Է���λͼ�����λͼ������Ϣ,���겻Ҫ����ɾ��λͼ�������
IDirectDrawSurface7 * DDLoadBitmap2(IDirectDraw7 *pdd, LPCSTR szBitmap,bmpinfo &bmpinfo_obj/*�ýṹ�ᱻ����*/)
{
    HBITMAP hbm;
    BITMAP bm;
    IDirectDrawSurface7 *pdds;

    // LoadImage has some added functionality in Windows 95 that allows
    // you to load a bitmap from a file on disk.
    hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0,
    LR_LOADFROMFILE|LR_CREATEDIBSECTION);

    if (hbm == NULL)
        return NULL;

    GetObject(hbm, sizeof(bm), &bm); // get size of bitmap

   /*
    * create a DirectDrawSurface for this bitmap
    * source to function CreateOffScreenSurface() follows immediately
    */

    pdds = CreateOffScreenSurface(pdd, bm.bmWidth, bm.bmHeight);
    if (pdds) {
        DDCopyBitmap(pdds, hbm, bm.bmWidth, bm.bmHeight);
    }
bmpinfo_obj.bm.bmWidth=bm.bmWidth;
bmpinfo_obj.bm.bmHeight=bm.bmHeight;
bmpinfo_obj.hbmp=hbm;
    //DeleteObject(hbm);

    return pdds;
}


/*
 * Creates a DirectDraw Surface of a specified size
 * This surface will be in video memory if enough is
 * available, otherwise it will be created in system memory
 */
IDirectDrawSurface7 * CreateOffScreenSurface(IDirectDraw7 *pdd, int dx, int dy)
{
    DDSURFACEDESC2 ddsd;
    IDirectDrawSurface7 *pdds;

    //
    // create a DirectDrawSurface for this bitmap
    //
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwWidth = dx;
    ddsd.dwHeight = dy;

    if (pdd->CreateSurface(&ddsd, &pdds, NULL) != DD_OK)
    {
        return NULL;
    } else {
        return pdds;
    }
}

/*
 * This function copies a previously loaded bitmap into a DirectDraw surface.
 * Notice that we can obtain a GDI style device context for a
 * DirectDraw surface, with which to BitBlt the bitmap into the
 * surface.
 */
HRESULT DDCopyBitmap(IDirectDrawSurface7 *pdds, HBITMAP hbm, int dx, int dy)
{
    HDC hdcImage;
    HDC hdc;
    HRESULT hr;
    HBITMAP hbmOld;

    //
    // select bitmap into a memoryDC so we can use it.
    //
    hdcImage = CreateCompatibleDC(NULL);
    hbmOld = (HBITMAP)SelectObject(hdcImage, hbm);

    if ((hr = pdds->GetDC(&hdc)) == DD_OK)
    {
        BitBlt(hdc, 0, 0, dx, dy, hdcImage, 0, 0, SRCCOPY);
        pdds->ReleaseDC(hdc);
    }

    SelectObject(hdcImage, hbmOld);
    DeleteDC(hdcImage);

    return hr;
}

/*
//V2.0����ָ��BLT��Դλ�ã�ר���������鶯����ʵ��
IDirectDrawSurface7 * DDCopyBitmap2(IDirectDrawSurface7 *pdds,IDirectDrawSurface7 *src_pdds, HBITMAP hbm,\
					  int dx, int dy,int srcx,int srcy)
{
    HDC hdcImage;
    HDC hdc;
    HRESULT hr;
    HBITMAP hbmOld;

    //
    // select bitmap into a memoryDC so we can use it.
    //
    hdcImage = CreateCompatibleDC(NULL);
    hbmOld = (HBITMAP)SelectObject(hdcImage, hbm);

    if ((hr = pdds->GetDC(&hdc)) == DD_OK)
    {
        BitBlt(hdc, 0, 0, dx, dy, hdcImage, srcx, srcy, SRCCOPY);
        pdds->ReleaseDC(hdc);
    }

    SelectObject(hdcImage, hbmOld);
    DeleteDC(hdcImage);

    return pdds;
}*/


/*
 * Creates a DirectDraw palette for a given bitmap on disk.
 * The parameter szBitmap is the file name of the bitmap.
 *
 */
IDirectDrawPalette * DDLoadPalette(IDirectDraw7 *pdd, LPCSTR szBitmap)
{
    IDirectDrawPalette* ddpal;
    int i;
    int n;
    int fh;
    PALETTEENTRY ape[256];

    if (szBitmap && (fh = _lopen(szBitmap, OF_READ)) != -1)
    {
        BITMAPFILEHEADER bf;
        BITMAPINFOHEADER bi;

        _lread(fh, &bf, sizeof(bf));
        _lread(fh, &bi, sizeof(bi));
        _lread(fh, ape, sizeof(ape));
        _lclose(fh);

        if (bi.biSize != sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (bi.biBitCount > 8)
            n = 0;
        else if (bi.biClrUsed == 0)
            n = 1 << bi.biBitCount;
        else
            n = bi.biClrUsed;

      //
      // a DIB color table has its colors stored BGR not RGB
      // so flip them around.
      //
        for(i=0; i<n; i++ )
        {
            BYTE r = ape[i].peRed;
            ape[i].peRed = ape[i].peBlue;
            ape[i].peBlue = r;
        }
    }
    if (pdd->CreatePalette(DDPCAPS_8BIT, ape, &ddpal, NULL) != DD_OK)
    {
        return NULL;
    } else {
        return ddpal;
    }
}



//�������ĺ���
bool ClearSurface(LPDIRECTDRAWSURFACE7 surface, int r, int g, int b)
{
	DDBLTFX fx;
	memset(&fx, 0, sizeof(fx));
	fx.dwSize = sizeof(fx);
	fx.dwFillColor = _16BIT(r, g, b);
	if(surface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL|DDBLT_WAIT, &fx)){
	return 0;
	}
	else
		return 1;
}


/*//һ��ĵ��Ժ���
bool alert(int word){
char buf[30];
sprintf(buf,"%d",word);
MessageBox(globalhwnd,buf,"alertinfo",MB_OK);
return 1;
}

bool alert2(LPCSTR word){
char buf[30];
sprintf(buf,"%c",word);
MessageBox(globalhwnd,buf,"alertinfo",MB_OK);
return 1;
}*/


//#######################################################
//һ���Զ�����Ժ���������MESSAGEBOX���ͱ�������ֵ
void alert3(int v){
	std::stringstream   ss; 
	std::string str;
	ss<<v;//v��������Ҳ�������ַ���
	ss>>str;//�ٴ���std��string���Ͷ���
	MessageBox(NULL,str.c_str(),"caution",MB_OK);//������ʵ����ת�����͵��ַ���
}



bool debugshow(int x,int y){
	char szBuffer[30];
	sprintf(szBuffer,"%d %d",x,y);
	HDC hdc;
	lpddsback->GetDC( &hdc );
	SetTextAlign( hdc, TA_TOP | TA_RIGHT );
	SetBkMode( hdc, TRANSPARENT );
	SelectObject( hdc, g_hFont );
	SetTextColor( hdc, RGB( 128, 0, 0 ) );
	TextOut( hdc, 450, 5, szBuffer, strlen( szBuffer ) );
	//MessageBox(globalhwnd,szBuffer,"",MB_OK);
	//lpddswork->ReleaseDC( hdc );
	lpddsback->ReleaseDC( hdc );
	return 1;
}

vector<string> explode( const string &delimiter, const string &str){
	vector<string> arr;
	
	int strleng = str.length();
	int delleng = delimiter.length();
	if (delleng==0)
		return arr;//no change
	
	int i=0;
	int k=0;
	while( i<strleng )
	{
		int j=0;
		while (i+j<strleng && j<delleng && str[i+j]
			
			==delimiter[j])
			j++;
		if (j==delleng)//found delimiter
		{
			arr.push_back( str.substr(k, i-k) );
			i+=delleng;
			k=i;
		}
		else
		{
			i++;
		}
	}
	arr.push_back( str.substr(k, i-k) );
	return arr;
}


int string2int(const string &s)
{
int a;
sscanf(s.c_str(), "%d", &a);
return a;
}

float string2float(const string &s){
	float a;
	sscanf(s.c_str(), "%f", &a);
	return a;
}