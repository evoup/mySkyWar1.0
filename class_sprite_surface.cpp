// class_sprite_surface.cpp: implementation of the class_sprite class.
//
//////////////////////////////////////////////////////////////////////

#include "class_sprite_surface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cls_sprite_surface::cls_sprite_surface()
{
	DDRAW_INIT_STRUCT(ddbltfx);//��ʼ��fx�ṹ
	ddbltfx.dwFillColor = _RGB16BIT565(0,0,0);
	this->hwnd=globalhwnd;
	//MessageBox(hwnd,"constructor has been called!","info",MB_OK);
	sfmgrtemp=(SF_MGR *)malloc(sizeof(SF_MGR));

	for (int i=0;i<1000;i++){
	sprite_surface[i].lpddsrc=NULL;//Ҫôָ��ʲô��ҪôΪNULL����������c++��̡�
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
	//memset(&sprite_surface,0,sizeof(sprite_surface));���ã�
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



//���ؾ��鶯�����飬����λͼ����װ���뵽����Ĳ���
/**lpdds���ΪNULL�����ú������֮�����������زĻ��棬������ǵ�ַ���ݣ����ز��ǲ��ǻᱻ��������
szBitmap��λͼ�ļ���;frame_num�Ǿ���֡��������ָ�����ɾ��鶯��ͼƬ��
*/

IDirectDrawSurface7 * cls_sprite_surface::getSpriteSurface(IDirectDrawSurface7 * lpdds,char * srcname,char * spritename,\
												   LPCSTR szBitmap,int frame_num,bool debugflag){
	//IDirectDrawSurface7 * templpdds;
	int loaded=0;
	//���زĶ��зŽṹ����
	int current_index=this->get_max_sprite_surface(0);
#if 0
	//��ѯ��������б�����Ѿ������ˣ����¶�Ӧ����
	this->iter=this->sprite_handler_list.begin();
	while(iter!=this->sprite_handler_list.end()){
		if ((*iter)->source_pic_name==szBitmap){
//			MessageBox(hwnd,"loaded!","info",MB_OK);
			loaded=1;
			return (*iter)->lpdds;//���������ֱ�ӷ��ػ���
			break;
		}
	++iter;
	}
#endif
	if (!loaded)
		lpdds=DDLoadBitmap(lpdd,szBitmap);
	//�����黭���������1��Ԫ��
	if(current_index==1000){
	MessageBox(hwnd,"already have 100 sprite surfaces!","bug",MB_OK);
	}
	else{//�����-1���ǵ�һ���ˣ����Ͼ����±�0
	//	char buf[30];
	//	sprintf(buf,"��ǰ����������Ԫ���±�%d",current_index);
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
	int hasaname=0;//�ǲ����Ѿ���������
#if 0
	char itoabuf[6];
	itoa(current_index+1,itoabuf,10);
	this->sfmgrtemp->sprite_surface_array_indexes=itoabuf;//??
/*	char buf[20];
	sprintf(buf,"%s",	this->sfmgrtemp->sprite_surface_array_indexes);
	MessageBox(this->hwnd,buf,"the info",MB_OK);*/
	//������������������1���ڵ�,���֮ǰ�нڵ�������͵�ǰspritename��һ���ģ��͸��³�����1|2
	
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
	//��ѯszBitmapλͼ�ĳ�����Ϣ��
	HBITMAP hbm;
    BITMAP bm;
    // LoadImage has some added functionality in Windows 95 that allows
    // you to load a bitmap from a file on disk.
    hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if (hbm == NULL){
		MessageBox(this->hwnd,"�޷���ȡλͼ���","bug",MB_OK);
		return NULL;//
	}
    GetObject(hbm, sizeof(bm), &bm); // get size of bitmap
	//����ָ�������ľ��鶯������

	int width=bm.bmWidth/frame_num;//�������ζ�ȡn������
	//alertme((int)bm.bmWidth);
	//alertme(frame_num);
	//alertme(width);
	int height=bm.bmHeight;
	//�հ�windows GMP�ľ���ṹsprite,�Ұ�����װ�����ҵ�SPRITE_SURFACE�ṹ���ˣ��ټ�2����Ա���ж�ÿһ֡�ĳ���
	this->sprite_surface[0].spritename=spritename;
	int i=current_index+1;
	this->sprite_surface[i].sprite.current_frame=0;
	this->sprite_surface[i].used=1;
	this->sprite_surface[i].sprite.x=0;
	this->sprite_surface[i].sprite.y=0;
	//this->sprite_surface[i].used=1;
	//�����1�������������ʱ��Ҫ����֪��Ϊʲô����������1������ʾ������
	this->sprite_surface[i].sprite.theframe_width=width;//֮�����ȥ�޸ģ����綯����Ϸ����Ĭ��Ϊ�г����Ĵ�С��
	this->sprite_surface[i].sprite.theframe_height=height;
#if 0
	if (debugflag){//debug
		alertme(i);
		this->sprite_surface[i].sprite.theframe_width=44;
		this->sprite_surface[i].sprite.theframe_height=38;
	}
#endif
	for (int j=0;j<frame_num;j++){
		this->sprite_surface[i].sprite.frames[j]=DDraw_Create_Surface(width,height,3,1);//���3��ָʹ��ϵͳ�ڴ�
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
	//Ȼ�����λͼ���������鶯������
	RECT DestRect;RECT SrcRect;
	DestRect.top=0;DestRect.left=0;DestRect.right=width;DestRect.bottom=height;

	for (j=0;j<frame_num;j++){
		//alertme("j��");
		//alertme(j);
		SrcRect.top=0;
		SrcRect.left=0+j*width;
		//alertme("SrcRect.left��");
		//alertme(SrcRect.left);
		SrcRect.right=SrcRect.left+width;
		//alertme("SrcRect.right��");
		//alertme(SrcRect.right);
		SrcRect.bottom=height;
		//alertme("SrcRect.bottom��");
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
	
	//ɾ��GDI����
	if(!DeleteObject(hbm)){
		MessageBox(hwnd,"ɾ��λͼ����ʧ��","",MB_OK);
	}

	return sprite_surface[current_index+1].lpddsrc;
}


#if 1
//jpg�汾
IDirectDrawSurface7 * cls_sprite_surface::getSpriteSurfaceJPG(IDirectDrawSurface7 * lpdds,char * srcname,char * spritename,\
												   LPCSTR szJpg,int frame_num,bool debugflag){
	//IDirectDrawSurface7 * templpdds;
	int loaded=0;
	//���زĶ��зŽṹ����
	int current_index=this->get_max_sprite_surface(0);

	JPEGINFO jinf;
	if (!loaded)
	LoadJPEG(FROM_FILE,szJpg,&lpdds,lpdd,jinf);
	//if (!loaded)
	//	lpdds=DDLoadBitmap(lpdd,szBitmap);
	//�����黭���������1��Ԫ��
	if(current_index==1000){
	MessageBox(hwnd,"already have 100 sprite surfaces!","bug",MB_OK);
	}
	else{//�����-1���ǵ�һ���ˣ����Ͼ����±�0
	//	char buf[30];
	//	sprintf(buf,"��ǰ����������Ԫ���±�%d",current_index);
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
	int hasaname=0;//�ǲ����Ѿ���������

	//��ѯszJpg JPEGͼ�ĳ�����Ϣ��
	//HBITMAP hbm;
    //BITMAP bm;
    // LoadImage has some added functionality in Windows 95 that allows
    // you to load a bitmap from a file on disk.
   // hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0,
//		LR_LOADFROMFILE|LR_CREATEDIBSECTION);
//	if (hbm == NULL){
//		MessageBox(this->hwnd,"�޷���ȡλͼ���","bug",MB_OK);
//		return NULL;//
//	}
 //   GetObject(hbm, sizeof(bm), &bm); // get size of bitmap
	//����ָ�������ľ��鶯������

	//int width=bm.bmWidth/frame_num;//�������ζ�ȡn������
	int width=jinf.width;
	//alertme((int)bm.bmWidth);
	//alertme(frame_num);
	//alertme(width);
	//int height=bm.bmHeight;
	int height=jinf.height;
	//�հ�windows GMP�ľ���ṹsprite,�Ұ�����װ�����ҵ�SPRITE_SURFACE�ṹ���ˣ��ټ�2����Ա���ж�ÿһ֡�ĳ���
	this->sprite_surface[0].spritename=spritename;
	int i=current_index+1;
	this->sprite_surface[i].sprite.current_frame=0;
	this->sprite_surface[i].used=1;
	this->sprite_surface[i].sprite.x=0;
	this->sprite_surface[i].sprite.y=0;
	//this->sprite_surface[i].used=1;
	//�����1�������������ʱ��Ҫ����֪��Ϊʲô����������1������ʾ������
	this->sprite_surface[i].sprite.theframe_width=width;//֮�����ȥ�޸ģ����綯����Ϸ����Ĭ��Ϊ�г����Ĵ�С��
	this->sprite_surface[i].sprite.theframe_height=height;
//alertme(width);
	for (int j=0;j<frame_num;j++){
		this->sprite_surface[i].sprite.frames[j]=DDraw_Create_Surface(width,height,3,1);//���3��ָʹ��ϵͳ�ڴ�
	}

	//Ȼ�����λͼ���������鶯������
	RECT DestRect;RECT SrcRect;
	DestRect.top=0;DestRect.left=0;DestRect.right=width;DestRect.bottom=height;

	for (j=0;j<frame_num;j++){
		//alertme("j��");
		//alertme(j);
		SrcRect.top=0;
		SrcRect.left=0+j*width;
		//alertme("SrcRect.left��");
		//alertme(SrcRect.left);
		SrcRect.right=SrcRect.left+width;
		//alertme("SrcRect.right��");
		//alertme(SrcRect.right);
		SrcRect.bottom=height;
		//alertme("SrcRect.bottom��");
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
	
	//ɾ��GDI����
//	if(!DeleteObject(hbm)){
//		MessageBox(hwnd,"ɾ��λͼ����ʧ��","",MB_OK);
//	}
	return sprite_surface[current_index+1].lpddsrc;
}
//end jpg�汾
#endif




LPCSTR cls_sprite_surface::get_serial_surface(char * sp_gp_name){
	//std::list<SF_MGR * >::iterator iter;
	iter=this->sprite_handler_list.begin();
	while(iter!=this->sprite_handler_list.end()){
		if((*iter)->sprite_group_name==sp_gp_name){
			MessageBox(this->hwnd,"��������","",MB_OK);
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
	return -9999;//����-9999���������
}



//��¶һ����Ԫ��������,��ʵ�б�Ҫ��ֱ��int playersfid=friend_cls_sprite_surface_getmax(myobj);�������ˣ�
int friend_cls_sprite_surface_getmax(cls_sprite_surface * obj){
return obj->get_max_sprite_surface(0);
}