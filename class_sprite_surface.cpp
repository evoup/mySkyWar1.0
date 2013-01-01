// class_sprite_surface.cpp: implementation of the class_sprite class.
//
//////////////////////////////////////////////////////////////////////

#include "class_sprite_surface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cls_sprite_surface::cls_sprite_surface()
{
	DDRAW_INIT_STRUCT(ddbltfx);//初始化fx结构
	ddbltfx.dwFillColor = _RGB16BIT565(0,0,0);
	this->hwnd=globalhwnd;
	//MessageBox(hwnd,"constructor has been called!","info",MB_OK);
	sfmgrtemp=(SF_MGR *)malloc(sizeof(SF_MGR));

	for (int i=0;i<1000;i++){
	sprite_surface[i].lpddsrc=NULL;//要么指向什么，要么为NULL，《高质量c++编程》
	sprite_surface[i].srcname=NULL;
	sprite_surface[i].spritename=NULL;
	sprite_surface[i].sprite_numframe=NULL;
	sprite_surface[i].used=0;
	sprite_surface[i].sprite.theframe_height=0;
	sprite_surface[i].sprite.theframe_width=0;
	sprite_surface[i].sprite.counter=0;
	sprite_surface[i].sprite.current_frame=0;
	sprite_surface[i].sprite.x=0;
	sprite_surface[i].sprite.y=0;
	}
	//memset(&sprite_surface,0,sizeof(sprite_surface));少用！
	//http://topic.csdn.net/u/20080219/11/e4eafd76-ac72-44f0-bb48-8c4b439b07f8.html

}


cls_sprite_surface::~cls_sprite_surface()
{
free(sfmgrtemp);
}

bool cls_sprite_surface::test(){
	MessageBox(hwnd,"this is a test","info",MB_OK);
	return 1;
}


bool cls_sprite_surface::alert(int word){
char buf[30];
sprintf(buf,"%d",word);
MessageBox(hwnd,buf,"alertinfo",MB_OK);
return 1;
}

bool cls_sprite_surface::alert(LPCSTR word){
char buf[30];
sprintf(buf,"%c",word);
MessageBox(hwnd,buf,"alertinfo",MB_OK);
return 1;
}



//返回精灵动作数组，输入位图，封装读入到表面的操作
/**lpdds如果为NULL，调用函数完成之后会销毁这个素材画面，否则就是地址传递，但素材是不是会被析构掉？
szBitmap是位图文件名;frame_num是精灵帧数，横向指定若干精灵动作图片。
*/

IDirectDrawSurface7 * cls_sprite_surface::getSpriteSurface(IDirectDrawSurface7 * lpdds,char * srcname,char * spritename,\
												   LPCSTR szBitmap,int frame_num,bool debugflag){
	//IDirectDrawSurface7 * templpdds;
	int loaded=0;
	//往素材队列放结构对象
	int current_index=this->get_max_sprite_surface(0);
#if 0
	//查询画面管理列表，如果已经载入了，更新对应数据
	this->iter=this->sprite_handler_list.begin();
	while(iter!=this->sprite_handler_list.end()){
		if ((*iter)->source_pic_name==szBitmap){
//			MessageBox(hwnd,"loaded!","info",MB_OK);
			loaded=1;
			return (*iter)->lpdds;//如果读入了直接返回画面
			break;
		}
	++iter;
	}
#endif
	if (!loaded)
		lpdds=DDLoadBitmap(lpdd,szBitmap);
	//往精灵画面数组递增1个元素
	if(current_index==1000){
	MessageBox(hwnd,"already have 100 sprite surfaces!","bug",MB_OK);
	}
	else{//如果是-1就是第一次了，加上就是下标0
	//	char buf[30];
	//	sprintf(buf,"当前最大画面管理器元素下标%d",current_index);
	//	MessageBox(hwnd,buf,"",MB_OK);
	this->sprite_surface[current_index+1].used=1;
	this->sprite_surface[current_index+1].lpddsrc=lpdds;
	this->sprite_surface[current_index+1].srcname=srcname;
	this->sprite_surface[current_index+1].sprite_numframe=frame_num;
	//this->sprite_surface[current_index+1].used=1;
	}
	this->sfmgrtemp->sprite_group_name=spritename;
	this->sfmgrtemp->source_pic_name=szBitmap;
	this->sfmgrtemp->lpdds=lpdds;
	int hasaname=0;//是不是已经有了组名
#if 0
	char itoabuf[6];
	itoa(current_index+1,itoabuf,10);
	this->sfmgrtemp->sprite_surface_array_indexes=itoabuf;//??
/*	char buf[20];
	sprintf(buf,"%s",	this->sfmgrtemp->sprite_surface_array_indexes);
	MessageBox(this->hwnd,buf,"the info",MB_OK);*/
	//往画面管理链表里添加1个节点,如果之前有节点的组名和当前spritename是一样的，就更新成这样1|2
	
	//list<SF_MGR * >::iterator iter;
	iter=this->sprite_handler_list.begin();
	while(iter!=this->sprite_handler_list.end()){
		if((*iter)->sprite_group_name==spritename){
			std::string a,b;
			a=(*iter)->sprite_surface_array_indexes;
			itoa(current_index+1,itoabuf,10);
			b=itoabuf;
			a+="|";
			a+=b;
			(*iter)->sprite_surface_array_indexes=(LPCSTR)a.c_str();
			hasaname=1;
			break;
		}
		++iter;
	}

	if(!hasaname)
		this->sprite_handler_list.push_back(sfmgrtemp);
#endif
	//查询szBitmap位图的长宽信息。
	HBITMAP hbm;
    BITMAP bm;
    // LoadImage has some added functionality in Windows 95 that allows
    // you to load a bitmap from a file on disk.
    hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if (hbm == NULL){
		MessageBox(this->hwnd,"无法获取位图句柄","bug",MB_OK);
		return NULL;//
	}
    GetObject(hbm, sizeof(bm), &bm); // get size of bitmap
	//创建指定数量的精灵动作画面

	int width=bm.bmWidth/frame_num;//横向依次读取n个精灵
	//alertme((int)bm.bmWidth);
	//alertme(frame_num);
	//alertme(width);
	int height=bm.bmHeight;
	//照搬windows GMP的精灵结构sprite,我把他封装到了我的SPRITE_SURFACE结构里了，再加2个成员以判断每一帧的长宽
	this->sprite_surface[0].spritename=spritename;
	int i=current_index+1;
	this->sprite_surface[i].sprite.current_frame=0;
	this->sprite_surface[i].used=1;
	this->sprite_surface[i].sprite.x=0;
	this->sprite_surface[i].sprite.y=0;
	//this->sprite_surface[i].used=1;
	//必须加1，但创建画面的时候不要，不知道为什么？否则右面1像素显示不出！
	this->sprite_surface[i].sprite.theframe_width=width;//之后可以去修改，比如动作游戏，先默认为切出来的大小。
	this->sprite_surface[i].sprite.theframe_height=height;
#if 0
	if (debugflag){//debug
		alertme(i);
		this->sprite_surface[i].sprite.theframe_width=44;
		this->sprite_surface[i].sprite.theframe_height=38;
	}
#endif
	for (int j=0;j<frame_num;j++){
		this->sprite_surface[i].sprite.frames[j]=DDraw_Create_Surface(width,height,3,1);//标记3是指使用系统内存
	}
#if 0
	if (debugflag) alertme(i);//debug
	if (debugflag){//debug
		alertme(this->sprite_surface[i].sprite.theframe_width);
		alertme(width);
		alertme(this->sprite_surface[i].sprite.theframe_height);
		
		alertme(height);
		//this->sprite_surface[i].sprite.theframe_width=44;
		//this->sprite_surface[i].sprite.theframe_height=38;
	}
#endif
	//然后读入位图到各个精灵动作画面
	RECT DestRect;RECT SrcRect;
	DestRect.top=0;DestRect.left=0;DestRect.right=width;DestRect.bottom=height;

	for (j=0;j<frame_num;j++){
		//alertme("j是");
		//alertme(j);
		SrcRect.top=0;
		SrcRect.left=0+j*width;
		//alertme("SrcRect.left是");
		//alertme(SrcRect.left);
		SrcRect.right=SrcRect.left+width;
		//alertme("SrcRect.right是");
		//alertme(SrcRect.right);
		SrcRect.bottom=height;
		//alertme("SrcRect.bottom是");
		//alertme(SrcRect.bottom);
		if(FAILED(this->sprite_surface[i].sprite.frames[j]->Blt(&DestRect, // pointer to dest RECT
		lpdds, // pointer to source surface
		&SrcRect, // pointer to source RECT
		DDBLT_WAIT, 
		&ddbltfx)))// pointer to DDBLTFX holding info
		{
			MessageBox(this->hwnd,"copy sprite surface failed!","info",MB_OK);
		}
	}
	
	//删除GDI对象
	if(!DeleteObject(hbm)){
		MessageBox(hwnd,"删除位图对象失败","",MB_OK);
	}

	return sprite_surface[current_index+1].lpddsrc;
}


#if 1
//jpg版本
IDirectDrawSurface7 * cls_sprite_surface::getSpriteSurfaceJPG(IDirectDrawSurface7 * lpdds,char * srcname,char * spritename,\
												   LPCSTR szJpg,int frame_num,bool debugflag){
	//IDirectDrawSurface7 * templpdds;
	int loaded=0;
	//往素材队列放结构对象
	int current_index=this->get_max_sprite_surface(0);

	JPEGINFO jinf;
	if (!loaded)
	LoadJPEG(FROM_FILE,szJpg,&lpdds,lpdd,jinf);
	//if (!loaded)
	//	lpdds=DDLoadBitmap(lpdd,szBitmap);
	//往精灵画面数组递增1个元素
	if(current_index==1000){
	MessageBox(hwnd,"already have 100 sprite surfaces!","bug",MB_OK);
	}
	else{//如果是-1就是第一次了，加上就是下标0
	//	char buf[30];
	//	sprintf(buf,"当前最大画面管理器元素下标%d",current_index);
	//	MessageBox(hwnd,buf,"",MB_OK);
	this->sprite_surface[current_index+1].used=1;
	this->sprite_surface[current_index+1].lpddsrc=lpdds;
	this->sprite_surface[current_index+1].srcname=srcname;
	this->sprite_surface[current_index+1].sprite_numframe=frame_num;
	//this->sprite_surface[current_index+1].used=1;
	}
	this->sfmgrtemp->sprite_group_name=spritename;
	this->sfmgrtemp->source_pic_name=szJpg;
	this->sfmgrtemp->lpdds=lpdds;
	int hasaname=0;//是不是已经有了组名

	//查询szJpg JPEG图的长宽信息。
	//HBITMAP hbm;
    //BITMAP bm;
    // LoadImage has some added functionality in Windows 95 that allows
    // you to load a bitmap from a file on disk.
   // hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0,
//		LR_LOADFROMFILE|LR_CREATEDIBSECTION);
//	if (hbm == NULL){
//		MessageBox(this->hwnd,"无法获取位图句柄","bug",MB_OK);
//		return NULL;//
//	}
 //   GetObject(hbm, sizeof(bm), &bm); // get size of bitmap
	//创建指定数量的精灵动作画面

	//int width=bm.bmWidth/frame_num;//横向依次读取n个精灵
	int width=jinf.width;
	//alertme((int)bm.bmWidth);
	//alertme(frame_num);
	//alertme(width);
	//int height=bm.bmHeight;
	int height=jinf.height;
	//照搬windows GMP的精灵结构sprite,我把他封装到了我的SPRITE_SURFACE结构里了，再加2个成员以判断每一帧的长宽
	this->sprite_surface[0].spritename=spritename;
	int i=current_index+1;
	this->sprite_surface[i].sprite.current_frame=0;
	this->sprite_surface[i].used=1;
	this->sprite_surface[i].sprite.x=0;
	this->sprite_surface[i].sprite.y=0;
	//this->sprite_surface[i].used=1;
	//必须加1，但创建画面的时候不要，不知道为什么？否则右面1像素显示不出！
	this->sprite_surface[i].sprite.theframe_width=width;//之后可以去修改，比如动作游戏，先默认为切出来的大小。
	this->sprite_surface[i].sprite.theframe_height=height;
//alertme(width);
	for (int j=0;j<frame_num;j++){
		this->sprite_surface[i].sprite.frames[j]=DDraw_Create_Surface(width,height,3,1);//标记3是指使用系统内存
	}

	//然后读入位图到各个精灵动作画面
	RECT DestRect;RECT SrcRect;
	DestRect.top=0;DestRect.left=0;DestRect.right=width;DestRect.bottom=height;

	for (j=0;j<frame_num;j++){
		//alertme("j是");
		//alertme(j);
		SrcRect.top=0;
		SrcRect.left=0+j*width;
		//alertme("SrcRect.left是");
		//alertme(SrcRect.left);
		SrcRect.right=SrcRect.left+width;
		//alertme("SrcRect.right是");
		//alertme(SrcRect.right);
		SrcRect.bottom=height;
		//alertme("SrcRect.bottom是");
		//alertme(SrcRect.bottom);
		if(FAILED(this->sprite_surface[i].sprite.frames[j]->Blt(&DestRect, // pointer to dest RECT
		lpdds, // pointer to source surface
		&SrcRect, // pointer to source RECT
		DDBLT_WAIT, 
		&ddbltfx)))// pointer to DDBLTFX holding info
		{
			MessageBox(this->hwnd,"copy sprite surface failed!","info",MB_OK);
		}
	}
	
	//删除GDI对象
//	if(!DeleteObject(hbm)){
//		MessageBox(hwnd,"删除位图对象失败","",MB_OK);
//	}
	return sprite_surface[current_index+1].lpddsrc;
}
//end jpg版本
#endif




LPCSTR cls_sprite_surface::get_serial_surface(char * sp_gp_name){
	//std::list<SF_MGR * >::iterator iter;
	iter=this->sprite_handler_list.begin();
	while(iter!=this->sprite_handler_list.end()){
		if((*iter)->sprite_group_name==sp_gp_name){
			MessageBox(this->hwnd,"迭代到了","",MB_OK);
			return (*iter)->sprite_surface_array_indexes;
		}
		++iter;
	}
	return "1|2|3|4|5";
}

int cls_sprite_surface::get_max_sprite_surface(int debug)
{
	for (int i=0;i<1000;i++){
		//if (this->sprite_surface[i].srcname==NULL){
		if (this->sprite_surface[i].used==0){
			//if (debug==1){
			//	char buf[30];
			//	sprintf(buf,"%d",i-1);
			//	MessageBox(this->hwnd,buf,"debuginfo",MB_OK);
			//}
			//if (i!=0)
				return i-1;
			//else
			//	return 0;
			break;
		}
	}//end for
	return -9999;//返回-9999代表出错了
}



//暴露一个友元函数出来,其实有必要吗？直接int playersfid=friend_cls_sprite_surface_getmax(myobj);不就是了？
int friend_cls_sprite_surface_getmax(cls_sprite_surface * obj){
return obj->get_max_sprite_surface(0);
}