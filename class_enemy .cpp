// cls_enemy.cpp: implementation of the cls_enemy class.
//
//////////////////////////////////////////////////////////////////////

#include "class_enemy.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=4A28D60B035E
cls_enemy::cls_enemy() : x(0), y(0),velocity(4),counter(0),HP(1),iFireInputDelay(59),ai(1),isdie(0),b_counter(0),b_type(0)
{
enum bonus bns=nothing;
item=bns;
}

//##ModelId=4A28D60B0370
cls_enemy::~cls_enemy()
{
//MessageBeep(MB_ICONASTERISK);
}


//##ModelId=4A28D60B0300
bool cls_enemy::FreshBullet()
{
	// TODO: Add your specialized code here.
	return 1;
}



//##ModelId=4A28D60B034B
POINT cls_enemy::GetPos()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	POINT point;
	point.x=this->x;
	point.y=this->y;
	
	return  point;
}



//##ModelId=4A28D60B0343
void cls_enemy::SetPos(floatpoint point)
{
	// TODO: Add your specialized code here.
	this->x=point.x;
	this->y=point.y;
}



//##ModelId=4A28D60B033E
void cls_enemy::AddBullet(floatpoint pt,int speed,int btype,int sfid,int direction)
{
	// TODO: Add your specialized code here.
	bullet b;
	b=this->GetBulletType(btype,sfid);

#if 1
	struct bullet * the_bullet=(bullet *)malloc(sizeof(bullet));
// 	the_bullet->nStyle=0;
// 	the_bullet->nSpeed=speed;
// 	the_bullet->x=pt.x+2;
// 	the_bullet->y=pt.y+20;
// 	the_bullet->width=12;//���ǵ����ӵ����
// 	the_bullet->height=10;
// 	the_bullet->nDirection=direction;//��3���ӿ�ʼ��ʱ��Ϊ1Ȧ��
	the_bullet->nStyle=b.nStyle;
	the_bullet->nSpeed=speed;
	the_bullet->x=pt.x+2;
	the_bullet->y=pt.y+20;
	the_bullet->width=12;//���ǵ����ӵ����
	the_bullet->height=10;
	the_bullet->nDirection=direction;//��3���ӿ�ʼ��ʱ��Ϊ1Ȧ��
	the_bullet->sfid= b.sfid;
	the_bullet->damage=b.damage;
	this->bullets_list.push_back(the_bullet);
#endif
	//eall_bullet_list.push_back(the_bullet);
}



//##ModelId=4A28D60B033D
void cls_enemy::RunBullet()
{
	int x_speed;
	int y_speed;
	this->bullet_iter=this->bullets_list.begin();//ѭ�������ӵ�
	while(this->bullet_iter!=this->bullets_list.end()){
		//(*bullet_iter)->y+=(*bullet_iter)->nSpeed;
		//�ȼ���x��y������ٶ�
		//x_speed=(*bullet_iter)->nSpeed*cos(90);
		//y_speed=-(*bullet_iter)->nSpeed*sin(90);�ò��ұ��ٶȸ��죡
		/*x_speed=(*bullet_iter)->nSpeed*cos_look[(*bullet_iter)->nDirection];
		y_speed=-(*bullet_iter)->nSpeed*sin_look[(*bullet_iter)->nDirection];*/
			x_speed=(*bullet_iter)->nSpeed*cos_look[(*bullet_iter)->nDirection];
			y_speed=-(*bullet_iter)->nSpeed*sin_look[(*bullet_iter)->nDirection];
			
 			(*bullet_iter)->x+=x_speed;
		(*bullet_iter)->y+=y_speed;

		if(((*bullet_iter)->y>2000) || ((*bullet_iter)->x<-2000) || ((*bullet_iter)->x>2000)\
			|| ((*bullet_iter)->y<-2000)){//��Խ��Ļ�ĵ�����Todo��malloc��������ô��?
			++bullet_iter;//CSDN�����list,��ɾǰ�轫�α������һ��Ԫ���ϣ������������
			//http://topic.csdn.net/t/20041209/09/3628704.html
			bullets_list.pop_front();
		}
		else{
			++bullet_iter;
		}
	}
	//����ӵ�����Ϊ�ն��ҵл����HP=0�������ٵл�����
	if(this->HP==0 && this->bullets_list.size()==0){
//MessageBox(NULL,"died","",MB_OK);
		this->isdie=true;
	}
}

//##ModelId=4A28D60B0340
void cls_enemy::DrawBullet(const cls_sprite_surface * s,/*int sfid,*/int xoffset,int yoffset){
this->bullet_iter=this->bullets_list.begin();//ѭ�������ӵ�
	while(this->bullet_iter!=this->bullets_list.end()){
		DDraw_Draw_Surface(s->sprite_surface[(*bullet_iter)->sfid].sprite.frames[this->aniseq[this->GetCurrentFrame()]], 
			(*bullet_iter)->x, (*bullet_iter)->y,
			s->sprite_surface[(*bullet_iter)->sfid].sprite.theframe_width,s->sprite_surface[(*bullet_iter)->sfid].sprite.theframe_height,
			lpddsback,1);
		++bullet_iter;
	}
}


//##ModelId=4A28D60B0335
struct bullet * cls_enemy::IsAttack(cls_player * player)
{
	// TODO: Add your specialized code here.

	//��ȡ�л����ĵ�λ�ã��ȵȲ������ģ������ϣ��������ǿ����жϣ�
	int e_x=player->GetPos().x;
	int e_y=player->GetPos().y;
	//HANDLE handle=CreateMutex(NULL, false, NULL);//���˻��⻹���У�
	int frameid=player->GetCurrentFrame();
	//ReleaseMutex(handle);
	//�жϵл������Ƿ����ӵ�
	int e_width,e_height;
	int b_width,b_height;
	int b_x,b_y;
	b_width=9;
	b_height=8;
	int isbeat;
	bullet_iter=bullets_list.begin();

	while(bullet_iter!=bullets_list.end()){	
		//player->anidata_list[0]->width=12;
		e_width=player->anidata_list[frameid]->width;
		e_height=player->anidata_list[frameid]->height;		
		//Todo���ݵл�ÿ֡��������ĵ�λ��ȥ����л���Ч����Ȼ����ӵ�������Ƚϣ���ʱû�������ĵ�ȥ���㣬�������ϣ�
		b_x=(*bullet_iter)->x;
		b_y=(*bullet_iter)->y;

		//Rect enemyrect = {e_x,e_y,e_width, e_height};
		//Rect bulletrect = {b_x, b_y, b_width, b_height};
		//int isbeat = rectcl (&bulletrect,&enemyrect , e_x, e_y);
		if (player->GetState()!=flikering||exitareaing)
		{
			isbeat = Collision_Test(e_x,e_y,e_width,e_height,b_x,b_y,b_width,b_height);
		}
		

		if (isbeat==1){
		//	alertme("���������ܹ�����");
			player->HP--;
			player->Restart();
			temp_bullet_iter=bullet_iter;
			delete(*bullet_iter);
			++bullet_iter;
			bullets_list.erase(temp_bullet_iter);
			return (struct bullet *)1;
		}
		else
		++bullet_iter;
	}
	return 0;
}


//##ModelId=4A28D60B0315
void cls_enemy::SetSeqNum(int seqnum){
	this->seqnum=seqnum;
}


//##ModelId=4A28D60B034E
void cls_enemy::SetAnimateSequence(string tokenword){
	vector<string> v = explode(",", tokenword);
	//char buf[20];
	for(int i=0; i<v.size(); i++){
		//sprintf(buf,"%d",v[i].data());
		//cout <<i << " ["<< v[i] <<"] " <<endl;
	//MessageBox(globalhwnd,v[i].data(),"",MB_OK);
		this->aniseq.push_back((int)(atoi(v[i].c_str())));//STL��stringת��int�ķ���
	}
	int seqnum=v.size();
	/*char buf[20];
	sprintf(buf,"%d",seqnum);
	MessageBox(globalhwnd,buf,"",MB_OK);*/
	this->SetSeqNum(seqnum);
}


//##ModelId=4A28D60B0334
int cls_enemy::GetCurrentFrame()
{
	// TODO: Add your specialized code here.

	return (int)this->current_frame;
}



//##ModelId=4A28D60B0333
bool cls_enemy::UpdateFrame()
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
// 	if (this->seqnum>=0 && this->seqnum<=4)
// 	{
// 		//return 1;
// 	}
// 	else{
// 		char buf[20];
// 		sprintf(buf,"%d",seqnum);
// 		//MessageBox(globalhwnd,buf,"",MB_OK);
// 		return 1;
// 	}
	if(++this->counter>=(6-this->velocity)){
	this->counter=0;
	if(++this->current_frame==this->seqnum)
		this->current_frame=0;
	}
	return 1;
}

void cls_enemy::SetVelocity(int speed){
	this->velocity=speed;
}

void cls_enemy::DrawMe(const cls_sprite_surface * s,int sfid){
		DDraw_Draw_Surface(s->sprite_surface[sfid].sprite.frames[this->aniseq[this->GetCurrentFrame()]], 
			this->x, this->y,
			s->sprite_surface[sfid].sprite.theframe_width,s->sprite_surface[sfid].sprite.theframe_height,
			lpddsback,1);
}

void cls_enemy::SetAnimateData(string tokenword)
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


/*anidata * cls_enemy::GetAniData(anidata * the_anidata){
	//anidata * the_anidata=(anidata *)malloc(sizeof(anidata));
	//anidata the_anidata;
	//return &the_anidata;//effective C++��Ҫ���ؾֲ����������
//the_anidata->height
	return the_anidata;
}
*/


//##ModelId=4A335A4A004E
void cls_enemy::SetHP(int hp)
{
	// TODO: Add your specialized code here.
	this->HP=hp;
}


void cls_enemy::SetAiState(int stateid)
{
this->ai=stateid;
}

int cls_enemy::GetAiState()
{
	if (this->use_move_fsm!=1)//���û��ʹ���ƶ���״̬��
	{
		return this->ai;
	}
	else{//ʹ���ƶ���״̬��
#if 0
		try{
			//MessageBeep(MB_ICONASTERISK);
			//ͳ�ƶ����ܹ�ʱ�䣬���жϵ�ǰ�ߵ��ĸ��ж�״̬
			int totaltime=0;
	//		int current_move_state_time;//��ǰ�ж�״̬�Ѿ������˶���ʱ��
			list<MOVESTATE >::iterator moveiter;
			moveiter=movelist.begin();
			while(moveiter!=movelist.end()){
				totaltime+=(*moveiter).movetime;
				++moveiter;
			}
			++this->movecounter;//��ʱ
			int puzzytime=movecounter%totaltime;//�������״̬ʱ��
			int theai;
			if(puzzytime!=0){//�����Ϊ0��ʼ���ĸ��ڵ�
				//int thisone=0;//�ڵ���±�
				moveiter=movelist.begin();
				while(moveiter!=movelist.end()){
					puzzytime-=(*moveiter).movetime;
					if (puzzytime<=0)
					{
						theai=(*moveiter).aimoveid;
						break;
					}
					++moveiter;
					//++thisone;
				}
				return theai;
			}
			else{//��������ƶ�״̬�����һ���ڵ�������
				if (this->loopmove)
				return movelist.begin()->aimoveid;
				else
				return -1;//��Ӧai�ű���-1���������ƶ���
			}
			//return this->ai;
		}
		catch(...){
			return this->ai;//�Է���һ
		}
#endif
	}//endʹ���ƶ���״̬��
return this->ai;//�Է���һ
}

bullet cls_enemy::GetBulletType(int b_type, int sfid)
{
	bullet b;
	if (b_type==0)//��׼�л��ӵ�
	{
		b.nStyle=0;
		b.nSpeed=5;
		//b.nDirection=270;
		b.x=7;
		b.y=-10;
		//b.width=9;����û�ã������õĴ�С���жϣ��ο��Ƕ�̬��
		//b.height=8;
		b.sfid=sfid;//normal_e_bullet
		b.damage=1;
	}
	else if (b_type==1)//��׼boss�ӵ�
	{
		b.nStyle=1;
		b.nSpeed=10;
		//b.nDirection=270;
		b.x=7;
		b.y=-10;
		b.width=9;
		b.height=8;
		b.sfid=sfid;//big_e_bullet
		b.damage=1;
	}
	else if (b_type==2)//�ɻ��Ƶл��ӵ�
	{
		b.nStyle=2;
		b.nSpeed=3;
		//b.nDirection=270;
		b.x=4;
		b.y=-20;
		b.width=18;
		b.height=32;
		b.sfid=sfid;
		b.damage=1;
	}
	return b;
}

void cls_enemy::SetBonus(int bs)
{
	this->item=bs;
}

int cls_enemy::GetBonus()
{
	return this->item;
}

void cls_enemy::SetMoveState(int s)
{
	this->move_state=s;
}

void cls_enemy::SetFireState(int s)
{
	this->fire_state=s;
}

int cls_enemy::attack(int sfid)
{
	floatpoint pt;
	if (++this->b_counter<10)
	{
	}
	else{
		pt.x=this->GetPos().x+43;
		pt.y=this->GetPos().y+9;
		this->AddBullet(pt,7,1,sfid,240);
		this->AddBullet(pt,7,1,sfid,270);
		this->AddBullet(pt,7,1,sfid,310);
		/*pt.x=this->GetPos().x+143;
		pt.y=this->GetPos().y+109;
		this->AddBullet(pt,7,1,sfid,255);
		this->AddBullet(pt,7,1,sfid,270);
		this->AddBullet(pt,7,1,sfid,315);*/
		this->b_counter=0;
		SetFireState(enemy_normalattack1);
		return 1;
	}
	return 0;

}

int cls_enemy::attack2(int sfid)
{
	floatpoint pt;
	//++angle;
	if (++this->b_counter<10)
	{
	}
	else{
		pt.x=this->GetPos().x+43;
		pt.y=this->GetPos().y+9;
		this->AddBullet(pt,7,1,sfid,240);
		this->AddBullet(pt,7,1,sfid,270);
		this->AddBullet(pt,7,1,sfid,310);
		/*pt.x=this->GetPos().x+143;
		pt.y=this->GetPos().y+109;
		this->AddBullet(pt,14,1,sfid,255);
		this->AddBullet(pt,14,1,sfid,270);
		this->AddBullet(pt,14,1,sfid,315);*/
		this->b_counter=0;
		SetFireState(enemy_normalattack);
		return 1;
	}
	return 0;

}


int cls_enemy::attack3(int sfid)
{
	floatpoint pt;
	//++angle;
	if (++this->b_counter<10)
	{
	}
	else{
		pt.x=this->GetPos().x+43;
		pt.y=this->GetPos().y+9;
		this->AddBullet(pt,7,1,sfid,240);
		this->AddBullet(pt,7,1,sfid,270);
		this->AddBullet(pt,7,1,sfid,310);
		/*pt.x=this->GetPos().x+143;
		pt.y=this->GetPos().y+109;
		this->AddBullet(pt,14,1,sfid,255);
		this->AddBullet(pt,14,1,sfid,270);
		this->AddBullet(pt,14,1,sfid,315);*/
		this->b_counter=0;
		SetFireState(enemy_normalattack3);
		return 1;
	}
	return 0;

}


int cls_enemy::attack1(int sfid)
{
	floatpoint pt;
	//++angle;
	if (++this->b_counter<10)
	{
	}
	else{
		pt.x=this->GetPos().x+43;
		pt.y=this->GetPos().y+9;
		this->AddBullet(pt,7,1,sfid,240);
		this->AddBullet(pt,7,1,sfid,270);
		this->AddBullet(pt,7,1,sfid,310);
		/*pt.x=this->GetPos().x+143;
		pt.y=this->GetPos().y+109;
		this->AddBullet(pt,14,1,sfid,255);
		this->AddBullet(pt,14,1,sfid,270);
		this->AddBullet(pt,14,1,sfid,315);*/
		this->b_counter=0;
		SetFireState(enemy_normalattack);
		return 1;
	}
	return 0;

}


int cls_enemy::GetFireState()
{
return this->fire_state;
}



//DEL void cls_enemy::AimTarget()
//DEL {
//DEL 
//DEL }
