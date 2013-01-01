// cls_sprite.cpp: implementation of the cls_sprite class.
//
//////////////////////////////////////////////////////////////////////

#include "class_sprite.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=4A1E966F029A
cls_sprite::cls_sprite() : x(0), y(0),velocity(4),counter(0),m_played_time(0),sprite_type(0),angle(270)
{

}

//##ModelId=4A1E966F029B
cls_sprite::~cls_sprite()
{

}




//##ModelId=4A1EA94701CE
floatpoint cls_sprite::GetPos()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	floatpoint point;
	point.x=this->x;
	point.y=this->y;
	return  point;
}



//##ModelId=4A1EAA500326
void cls_sprite::SetPos(floatpoint point)
{
	// TODO: Add your specialized code here.
	this->x=point.x;
	this->y=point.y;
}





//##ModelId=4A28D60B0213
void cls_sprite::DrawMe(const cls_sprite_surface * s,int sfid,int xoffset,int yoffset,int flag){
#if 0
	if (flag==1){
		if(!DDraw_Draw_Surface(s->sprite_surface[sfid].sprite.frames[this->aniseq[this->GetCurrentFrame()]], 
			this->x+xoffset, this->y+yoffset,
			44,38,
			lpddsback,1)){
			if (flag==1){
			//	MessageBox(globalhwnd,"绘图失败","",MB_OK);
			}
		}
		//if (flag==1)
		//	alertme(s->sprite_surface[sfid].sprite.theframe_width);
		return;
	}
#endif
#if 1
		if(!DDraw_Draw_Surface(s->sprite_surface[sfid].sprite.frames[this->aniseq[this->GetCurrentFrame()]], 
			this->x+xoffset, this->y+yoffset,
			s->sprite_surface[sfid].sprite.theframe_width,s->sprite_surface[sfid].sprite.theframe_height,
			lpddsback,1)){
			if (flag==1){
				MessageBox(globalhwnd,"绘图失败","",MB_OK);
			}
		}
		if (flag==1)
		alertme(s->sprite_surface[sfid].sprite.theframe_width);
#endif
}


//##ModelId=4A1FFCFB0073
void cls_sprite::SetSeqNum(int seqnum){
	this->seqnum=seqnum;
}

//##ModelId=4A1FFCFB0094
void cls_sprite::SetAnimateSequence(string tokenword){
	vector<string> v = explode(",", tokenword);
//	char buf[20];
	//int test;
	for(int i=0; i<v.size(); i++){
//		sprintf(buf,"%d",i);
		//cout <<i << " ["<< v[i] <<"] " <<endl;
//	MessageBox(globalhwnd,buf,"",MB_OK);
	//	test=(int)(atoi(v[i].c_str()));
//sprintf(buf,"%d",test);
//	MessageBox(globalhwnd,v[i].data(),"",MB_OK);
		//下面这个方法是有BUG的
	//	this->aniseq.push_back((int)(atoi(v[i].c_str())));//STL的string转换int的方法
//	sprintf(buf,"测试%s",v[i].c_str());
//MessageBox(globalhwnd,buf,"",MB_OK);
		this->aniseq.push_back(string2int(v[i].c_str()));
	}
	int seqnum=v.size();

	this->SetSeqNum(seqnum);
}


//##ModelId=4A28D60B0242
void cls_sprite::SetAnimateData(string tokenword)
{
	vector<string> v = explode(",", tokenword);
	vector<string> w;
	struct anidata * the_anidata=(anidata *)malloc(sizeof(anidata));
	for(int i=0; i<v.size(); i++){
		//MessageBox(globalhwnd,v[i].data(),"",MB_OK);		
		for(int j=0;j<2;j++){
			w.clear();
			w=explode("|",v[i].data());
			//MessageBox(globalhwnd,w[j].data(),"",MB_OK);
			if(j==0)//宽度
				the_anidata->width=(int)(atoi(w[j].c_str()));
			else if(j==1)//高度
				the_anidata->height=(int)(atoi(w[j].c_str()));
		}
	}
	this->anidata_list.push_back(the_anidata);//填入动画帧数据链表
}


//##ModelId=4A1FFD74012E
int cls_sprite::GetCurrentFrame()
{
	// TODO: Add your specialized code here.

	return (int)this->current_frame;
}



//##ModelId=4A1FFE8E015D
bool cls_sprite::UpdateFrame()
{
	// TODO: Add your specialized code here.
	
	////////////////////////
	//if(++this->counter>=(this->seqnum-this->velocity)){
	if(++this->counter>=(60-this->velocity)){
		this->counter=0;
		if(++this->current_frame>=this->seqnum){//超过1次序列循环，就是超过播放了1次
			this->m_played_time+=1;
			this->current_frame=0;
		}
	}
	return (bool)1;
}

//##ModelId=4A28D60B0232
void cls_sprite::SetVelocity(int speed){
	this->velocity=speed;
}






//##ModelId=4A2D42A801C5
void cls_sprite::SetLoopNum(int loopnum)
{
	// TODO: Add your specialized code here.
}


bool cls_sprite::PlayTime(int time,int sfid ){
	//参数time 播放次数
	//alertme(m_played_time);
	if((time-this->m_played_time)>0){
		this->DrawMe(m_spsf,sfid);
		return 0;//返回值代表是否完成
	}
	else{
		return 1;
	}
}

bool cls_sprite::Bind(cls_sprite_surface * p_spsf){
	m_spsf=p_spsf;
return 1;
}

//设置运动角度
void cls_sprite::SetAngle(float ang)
{
	this->angle=ang;
}

float cls_sprite::GetAngle()
{
return this->angle;
}

int cls_sprite::GetVelocity()
{
return this->velocity;
}

void cls_sprite::SetNspeed(int speed)
{
this->nSpeed=speed;
}

int cls_sprite::GetNspeed()
{
return this->nSpeed;
}

int cls_sprite::PushMoveState(MOVESTATE &ms)
{
this->movelist.push_back(ms);
return 1;
}
