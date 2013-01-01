// cls_player.cpp: implementation of the cls_player class.
//
//////////////////////////////////////////////////////////////////////

#include "class_player.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=4A28D60B02EE
cls_player::cls_player() : x(0), y(0),velocity(4),counter(0),HP(3),level(0),state(starting),wait_leave_time(0),flickering_count(0),wait_flickering_time(0)
{
}

//##ModelId=4A28D60B02EF
cls_player::~cls_player()
{

}


//##ModelId=4A28D60B0275
bool cls_player::FreshBullet()
{
	// TODO: Add your specialized code here.
	return 1;
}



//##ModelId=4A28D60B02D1
POINT cls_player::GetPos()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	POINT point;
	point.x=this->x;
	point.y=this->y;
	return  point;
}



//##ModelId=4A28D60B02CF
void cls_player::SetPos(POINT point)
{
	// TODO: Add your specialized code here.
	this->x=point.x;
	this->y=point.y;
}



//##ModelId=4A28D60B02C0
void cls_player::AddBullet(POINT pt,int btype,int sfid)
{

	// TODO: Add your specialized code here.
	bullet b;
	b=this->GetBulletType(btype,sfid);
	
	struct bullet * the_bullet=(bullet *)malloc(sizeof(bullet));
// 	the_bullet->nStyle=1;
// 	the_bullet->nSpeed=16;
// 	the_bullet->nDirection=0;//��360�㣬0������
// 	the_bullet->x=pt.x-3;
// 	the_bullet->y=pt.y-20;
	the_bullet->nStyle=b.nStyle;
	the_bullet->nSpeed=b.nSpeed;
	the_bullet->nDirection=b.nDirection;
	the_bullet->x=pt.x+b.x;
	the_bullet->y=pt.y+b.y;
	the_bullet->sfid=b.sfid;
	the_bullet->width=b.width;
	the_bullet->height=b.height;
	the_bullet->damage=b.damage;
	this->bullets_list.push_back(the_bullet);
}



//##ModelId=4A28D60B02BF
void cls_player::RunBullet(list<cls_enemy * > & elist)
{
#if 1
		float x_speed;
		float y_speed;
		this->bullet_iter=this->bullets_list.begin();//ѭ�������ӵ�
		while(this->bullet_iter!=this->bullets_list.end()){
			//�ȼ���x��y������ٶ�
			//x_speed=(*bullet_iter)->nSpeed*cos(90);
			//y_speed=-(*bullet_iter)->nSpeed*sin(90);�ò��ұ��ٶȸ��죡
			//һ���ӵ�
			x_speed=(*bullet_iter)->nSpeed*cos_look[(*bullet_iter)->nDirection];
			y_speed=-(*bullet_iter)->nSpeed*sin_look[(*bullet_iter)->nDirection];
			//(*bullet_iter)->x+=x_speed;
			//(*bullet_iter)->y+=y_speed;
			//����11
			if ( (*bullet_iter)->nStyle==11)
			{
				//Ѱ�ҵ���
				list<cls_enemy * >::iterator enemy_iter;
				enemy_iter=elist.begin();
				int jiajiao;
				while(enemy_iter!=elist.end()){
					//Խ����ų�
					if ((*enemy_iter)->GetPos().y<=0 || (*enemy_iter)->GetPos().y>=480 \
						|| (*enemy_iter)->GetPos().x<=118 || (*enemy_iter)->GetPos().x>=522
						){
					}
					else
					{
						//����н�
						jiajiao=atan(fabs(\
							((float)(*bullet_iter)->y-(*enemy_iter)->GetPos().y)/\
							((float)(*bullet_iter)->x-(*enemy_iter)->GetPos().x)\
							)\
							)/PI*180;
						//�ı䵼������
						x_speed=(*bullet_iter)->nSpeed*cos_look[jiajiao];
						y_speed=-(*bullet_iter)->nSpeed*sin_look[jiajiao];
						(*bullet_iter)->nDirection=jiajiao;
						//	if((*bullet_iter)->x<(*enemy_iter)->GetPos().x)
						//		jiajiao-=180;//�����ڵл�����
						//	else
						//	jiajiao-=270;//�����ڵл�����
					}
					++enemy_iter;
				}
				
			}
			//����10
			if ( (*bullet_iter)->nStyle==10)
			{
				//Ѱ�ҵ���
				list<cls_enemy * >::iterator enemy_iter;
				enemy_iter=elist.begin();
				int jiajiao;
				while(enemy_iter!=elist.end()){
					//Խ����ų�
					if ((*enemy_iter)->GetPos().y<=0 || (*enemy_iter)->GetPos().y>=480 \
						|| (*enemy_iter)->GetPos().x<=118 || (*enemy_iter)->GetPos().x>=522
						){
					}
					else
					{
						//����н�
						jiajiao=atan(fabs(\
							((float)(*bullet_iter)->y-(*enemy_iter)->GetPos().y)/\
							((float)(*bullet_iter)->x-(*enemy_iter)->GetPos().x)\
							)\
							)/PI*180;
						//�ı䵼������
						x_speed=-(*bullet_iter)->nSpeed*cos_look[jiajiao];
						y_speed=-(*bullet_iter)->nSpeed*sin_look[jiajiao];
						
						if((*bullet_iter)->x<(*enemy_iter)->GetPos().x){
								jiajiao-=180;//�����ڵл�����
						}
						else{
							jiajiao-=270;//�����ڵл�����
						}
							(*bullet_iter)->nDirection=jiajiao;
					}
					++enemy_iter;
				}
				
			}
			//end ����11 10
			(*bullet_iter)->x+=x_speed;
			(*bullet_iter)->y+=y_speed;
#endif
		if((*bullet_iter)->y<-1000){//��Խ��Ļ�ĵ�����Todo��Ϊʲô�ĳ�<0�ٶ�Ϊ10�ĵ������ߵ�1����ʧ��
			++bullet_iter;//CSDN�����list,��ɾǰ�轫�α������һ��Ԫ���ϣ������������
			//http://topic.csdn.net/t/20041209/09/3628704.html
			bullets_list.pop_front();
		}
		else{
			++bullet_iter;
		}
	}
}

//##ModelId=4A28D60B02C2
void cls_player::DrawBullet(const cls_sprite_surface * s){
this->bullet_iter=this->bullets_list.begin();//ѭ�������ӵ�
	while(this->bullet_iter!=this->bullets_list.end()){
		if ((*bullet_iter)->nStyle!=11 || (*bullet_iter)->nStyle!=10)//�ӵ�
		{
			DDraw_Draw_Surface(s->sprite_surface[(*bullet_iter)->sfid].sprite.frames[0], 
				(*bullet_iter)->x, (*bullet_iter)->y,
				s->sprite_surface[(*bullet_iter)->sfid].sprite.theframe_width,s->sprite_surface[(*bullet_iter)->sfid].sprite.theframe_height,
			lpddsback,1);
		}
		else{//����
			DDraw_Draw_Surface(s->sprite_surface[(*bullet_iter)->sfid].sprite.frames[0], 
				(*bullet_iter)->x, (*bullet_iter)->y,
				s->sprite_surface[(*bullet_iter)->sfid].sprite.theframe_width,s->sprite_surface[(*bullet_iter)->sfid].sprite.theframe_height,
			lpddsback,1);
		}
		
		++bullet_iter;
	}
}

//##ModelId=4A28D60B02C5
void cls_player::DrawMe(const cls_sprite_surface * s,int sfid){
	if (++flickering_count<4 && (this->GetState()==starting||flikering))
	{
		DDraw_Draw_Surface(s->sprite_surface[sfid].sprite.frames[this->aniseq[this->GetCurrentFrame()]], 
			this->x, this->y,
			s->sprite_surface[sfid].sprite.theframe_width,s->sprite_surface[sfid].sprite.theframe_height,
			lpddsback,1);
	}
	else
		flickering_count=0;
	if (this->GetState()==normaling||exitareaing && this->GetState()!=starting && this->GetState()!=flikering)
	{
		DDraw_Draw_Surface(s->sprite_surface[sfid].sprite.frames[this->aniseq[this->GetCurrentFrame()]], 
			this->x, this->y,
			s->sprite_surface[sfid].sprite.theframe_width,s->sprite_surface[sfid].sprite.theframe_height,
			lpddsback,1);
	}		
}


//##ModelId=4A28D60B02B6
int cls_player::IsAttack(cls_enemy * the_enemy)
{
	if(the_enemy->HP==0)//��Ϊ����ֱ�Ӿ���������HP==0���ӵ�Ԫ��==0���㣩������Ҫ�ж�һ��
		return 0;
	// TODO: Add your specialized code here.'
	//��ȡ�л����ĵ�λ�ã��ȵȲ������ģ������ϣ��������ǿ����жϣ�
	int e_x=the_enemy->GetPos().x;
	int e_y=the_enemy->GetPos().y;
	//HANDLE handle=CreateMutex(NULL, false, NULL);//���˻��⻹���У�
	int frameid=the_enemy->GetCurrentFrame();
	//ReleaseMutex(handle);
	//�жϵл������Ƿ����ӵ�
	int e_width,e_height;
	int b_width,b_height;
	int b_x,b_y;
	b_width=0;
	b_height=0;
	int isbeat;
	bullet_iter=bullets_list.begin();
	while(bullet_iter!=bullets_list.end()){	
		e_width=the_enemy->anidata_list[frameid]->width;
		e_height=the_enemy->anidata_list[frameid]->height;
		//Todo���ݵл�ÿ֡��������ĵ�λ��ȥ����л���Ч����Ȼ����ӵ�������Ƚϣ���ʱû�������ĵ�ȥ���㣬�������ϣ�
		b_x=(*bullet_iter)->x;
		b_y=(*bullet_iter)->y;
		b_width=(*bullet_iter)->width;
		b_height=(*bullet_iter)->height;
		//Rect enemyrect = {e_x,e_y,e_width, e_height};
		//Rect bulletrect = {b_x, b_y, b_width, b_height};
		//int isbeat = rectcl (&bulletrect,&enemyrect , e_x, e_y);
		if (b_y>=0)//�е��ӵ�Խ��(���Ķ��٣���Ļ�Ϸ�)һ��ʱ�����������Ҳ��Ҫ���
		{
			isbeat = Collision_Test(e_x,e_y,e_width,e_height,b_x,b_y,b_width,b_height);
		}
		if (isbeat==1){
			//alertme("�л���λ�������ܹ�����");
			//the_enemy->HP--;
			the_enemy->HP=the_enemy->HP-(*bullet_iter)->damage;
			temp_bullet_iter=bullet_iter;
			delete(*bullet_iter);
			++bullet_iter;
			bullets_list.erase(temp_bullet_iter);
			return 1;
		}
		++bullet_iter;
	}
	return 0;
}

//##ModelId=4A28D60B0286
void cls_player::SetSeqNum(int seqnum){
	this->seqnum=seqnum;
}

//##ModelId=4A28D60B02DF
void cls_player::SetAnimateSequence(string tokenword){
	vector<string> v = explode(",", tokenword);
	//char buf[20];
	for(int i=0; i<v.size(); i++){

		//sprintf(buf,"%d",i);
		//cout <<i << " ["<< v[i] <<"] " <<endl;
	//MessageBox(globalhwnd,buf,"",MB_OK);
		this->aniseq.push_back((int)(atoi(v[i].c_str())));//STL��stringת��int�ķ���
	}
	int seqnum=v.size();
	this->SetSeqNum(seqnum);
}


//##ModelId=4A28D60B02E3
void cls_player::SetAnimateData(string tokenword)
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
			if(j==0)//���
				the_anidata->width=(int)(atoi(w[j].c_str()));
			else if(j==1)//�߶�
				the_anidata->height=(int)(atoi(w[j].c_str()));
			this->anidata_list.push_back(the_anidata);//���붯��֡��������
		}
	}
	
}


//##ModelId=4A28D60B02B5
int cls_player::GetCurrentFrame()
{
	// TODO: Add your specialized code here.

	return (int)this->current_frame;
}



//##ModelId=4A28D60B02B4
bool cls_player::UpdateFrame()
{
	// TODO: Add your specialized code here.

	////////�ο�����////////////
#if 0
	if (++girl_swordman.counter >= (8 - girl_swordman.velocity))
	{
		// reset counter
		girl_swordman.counter = 0;
		
		// advance to next frame
		if (++girl_swordman.current_frame > 3)
			girl_swordman.current_frame = 0;
		
	} // end if
#endif
	////////////////////////
	if(++this->counter>=(8-this->velocity)){
	this->counter=0;
	if(++this->current_frame>1)
		this->current_frame=0;
	}
	if (this->GetState()==exitareaing)
	{
	}
	else if (this->GetState()==normaling||flikering)
	{
		//�ȵ������ͺ����𣿼��̲����ж�����
		if(KEYSTATE(VK_RIGHT)){
			this->current_frame=2;
		}
		if(KEYSTATE(VK_LEFT)){
			this->current_frame=4;
		}
	}


	return (bool)1;
}

void cls_player::SetVelocity(int speed){
	this->velocity=speed;
}



void cls_player::SetHp(int hp)
{
	this->HP=hp;
}

int cls_player::GetHp()
{
	return this->HP;
}

bullet cls_player::GetBulletType(int b_type,int sfid){

	bullet b;
	if (b_type==0)
	{
		b.nStyle=0;
		b.nSpeed=28;
		b.nDirection=90;//��360�㣬0��3����
		b.x=7;
		b.y=-10;
		b.width=9;
		b.height=24;
		b.sfid=sfid;
		b.damage=3;
	}
	else if (b_type==1)
	{
		b.nStyle=1;
		b.nSpeed=29;
		b.nDirection=90;//��360�㣬0��3����
		b.x=-3;
		b.y=-20;
		b.width=30;
		b.height=32;
		b.sfid=sfid;
		b.damage=3;
	}
	else if (b_type==2)
	{
		b.nStyle=2;
		b.nSpeed=30;
		b.nDirection=90;//��360�㣬0��3����
		b.x=4;
		b.y=-20;
		b.width=18;
		b.height=32;
		b.sfid=sfid;
		b.damage=4;
	}
	else if (b_type==3)
	{
		b.nStyle=3;
		b.nSpeed=31;
		b.nDirection=90;//��360�㣬0��3����
		b.x=-33;
		b.y=-20;
		b.width=90;
		b.height=32;
		b.sfid=sfid;
		b.damage=4;
	}
	else if (b_type==10)//�����󵼵�
	{
		b.nStyle=10;
		b.nSpeed=10;
		b.nDirection=90;//��360�㣬0��3����
		b.x=-48;
		b.y=-20;
		b.width=12;
		b.height=12;
		b.sfid=sfid;
		b.damage=12;
	}
	else if (b_type==11)//�����ҵ���
	{
		b.nStyle=11;
		b.nSpeed=10;
		b.nDirection=90;//��360�㣬0��3����
		b.x=-24;
		b.y=-20;
		b.width=12;
		b.height=12;
		b.sfid=sfid;
		b.damage=12;
	}
	return b;
}

int cls_player::LevelUp()
{
if (level<3)
{
	++level;
}
return 1;
}

int cls_player::GetLevel()
{
	return this->level;
}

int cls_player::Restart()
{
this->level=0;
return 1;
}

void cls_player::SetState(int s)
{
	this->state=s;
}

int cls_player::GetState()
{
	return this->state;
}

int cls_player::GoStarting()
{
	if (y>=300)
	{
		y-=3;
		return 0;
	}
	else
		return 1;
}

int cls_player::GoLeaving()
{
	if (wait_leave_time<60)
	{
		++wait_leave_time;
		return 0;
	}
	if (y>-1000)
	{
		y-=8;
		return 0;
	}
	else{
		wait_leave_time=0;
		return 1;
	}
}

int cls_player::GoFlikering()
{
	if (wait_flickering_time<70)
	{
		++wait_flickering_time;
		return 0;
	}
	else{
		wait_flickering_time=0;
		return 1;
	}
}

void cls_player::UpdateMissileSurface(MISSILE_SURFACE * mss)
{
	this->bullet_iter=this->bullets_list.begin();
	while(this->bullet_iter!=this->bullets_list.end()){
		if ((*bullet_iter)->nStyle==10 || (*bullet_iter)->nStyle==11)
		{
			if ((*bullet_iter)->nDirection>=350 && (*bullet_iter)->nDirection<=11)
			{
				(*bullet_iter)->sfid=mss->surface0;
			}
			else if ((*bullet_iter)->nDirection>11 && (*bullet_iter)->nDirection<=34)
			{
				(*bullet_iter)->sfid=mss->surface1;
			}
			else if ((*bullet_iter)->nDirection>35 && (*bullet_iter)->nDirection<=56)
			{
				(*bullet_iter)->sfid=mss->surface2;
			}
			else if ((*bullet_iter)->nDirection>57 && (*bullet_iter)->nDirection<=76)
			{
				(*bullet_iter)->sfid=mss->surface3;
			}
			else if ((*bullet_iter)->nDirection>76 && (*bullet_iter)->nDirection<=111)
			{
				(*bullet_iter)->sfid=mss->surface4;
			}
			else if ((*bullet_iter)->nDirection>112 && (*bullet_iter)->nDirection<=124)
			{
				(*bullet_iter)->sfid=mss->surface4;
			}
			else if ((*bullet_iter)->nDirection>124 && (*bullet_iter)->nDirection<=146)
			{
				(*bullet_iter)->sfid=mss->surface5;
			}
			else if ((*bullet_iter)->nDirection>146 && (*bullet_iter)->nDirection<=158)
			{
				(*bullet_iter)->sfid=mss->surface6;
			}
			else if ((*bullet_iter)->nDirection>159 && (*bullet_iter)->nDirection<=169)
			{
				(*bullet_iter)->sfid=mss->surface7;
			}
			else if ((*bullet_iter)->nDirection>170 && (*bullet_iter)->nDirection<=191)
			{
				(*bullet_iter)->sfid=mss->surface8;
			}




		}
		++this->bullet_iter;
	}
}
