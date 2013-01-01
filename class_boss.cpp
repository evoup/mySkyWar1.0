// cls_boss.cpp: implementation of the cls_boss class.
//
//////////////////////////////////////////////////////////////////////

#include "class_boss.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=4A3353910251
cls_boss::cls_boss()
{

}

//##ModelId=4A3353910252
cls_boss::~cls_boss()
{

}


//##ModelId=4A335455035B
 cls_boss::FireLaser()
{
}


//##ModelId=4B02B4E9005D
void cls_boss::SetState(int s)
{
this->state=s;
}

//##ModelId=4B02B4E9004E
int cls_boss::GetState()
{
	return this->state;
}


//************************************
// Method:    normal_attack
// FullName:  cls_boss::normal_attack
// Access:    public 
// Returns:   int
// Qualifier:/迎面6定向发散弹
//************************************
//##ModelId=4B02B4E9001F
int cls_boss::normal_attack(int sfid)
{
	floatpoint pt;

	//++angle;
	if (++this->b_counter<80)
	{
	}
	else{
		pt.x=this->GetPos().x+43;
		pt.y=this->GetPos().y+109;
		this->AddBullet(pt,14,1,sfid,255);
		this->AddBullet(pt,14,1,sfid,270);
		this->AddBullet(pt,14,1,sfid,315);
		pt.x=this->GetPos().x+143;
		pt.y=this->GetPos().y+109;
		this->AddBullet(pt,14,1,sfid,255);
		this->AddBullet(pt,14,1,sfid,270);
		this->AddBullet(pt,14,1,sfid,315);
		this->b_counter=0;
		SetState(boss_normalattack1);
		return 1;
	}
	return 0;

}



//************************************
// Method:    normal_attack1
// FullName:  cls_boss::normal_attack1
// Access:    public 
// Returns:   int
// Qualifier:迎面4发子弹
//************************************
//##ModelId=4B02B4E803C8
int cls_boss::normal_attack1(int sfid)
{
	floatpoint pt;
	
	//++angle;
	if (++b_counter<80 )
	{
	}
	else if (b_counter>=80 && b_counter<200)
	{
		//pt.x=this->GetPos().x+43;
		//pt.y=this->GetPos().y+109;
		//this->AddBullet(pt,6,270);
		pt.x=this->GetPos().x+33;
		pt.y=this->GetPos().y+69;
		this->AddBullet(pt,15,1,sfid,270);
// 		pt.x=this->GetPos().x+143;
// 		pt.y=this->GetPos().y+109;
// 		this->AddBullet(pt,6,270);
 		pt.x=this->GetPos().x+153;
 		pt.y=this->GetPos().y+69;
 		this->AddBullet(pt,15,1,sfid,270);
		this->b_counter=200;
	}
	if (b_counter>=200 && b_counter<206){
		//停几帧再发第二击
	}
	else if (b_counter>=206)
	{
		pt.x=this->GetPos().x+33;
		pt.y=this->GetPos().y+69;
		this->AddBullet(pt,15,1,sfid,270);
			pt.x=this->GetPos().x+153;
			pt.y=this->GetPos().y+69;
			this->AddBullet(pt,15,1,sfid,270);
		this->b_counter=0;
		SetState(boss_normalattack2);
		return 1;
	}

	return 0;
}

//************************************
// Method:    normal_attack2
// FullName:  cls_boss::normal_attack2
// Access:    public 
// Returns:   int
// Qualifier: 可以被打掉的散弹
// Parameter: int sfid
//************************************
//##ModelId=4B02B4E80399
int cls_boss::normal_attack2(int sfid)
{
	floatpoint pt;
	
	//++angle;
	if (++this->b_counter<40)
	{
	}
	else{
		pt.x=this->GetPos().x+43;
		pt.y=this->GetPos().y+109;
		this->AddBullet(pt,4,2,sfid,255);
		this->AddBullet(pt,4,2,sfid,270);
		this->AddBullet(pt,4,2,sfid,315);
		this->AddBullet(pt,4,2,sfid,252);
		this->AddBullet(pt,4,2,sfid,263);
		this->AddBullet(pt,4,2,sfid,307);
		this->AddBullet(pt,4,2,sfid,227);
		pt.x=this->GetPos().x+143;
		pt.y=this->GetPos().y+109;
		this->AddBullet(pt,4,2,sfid,255);
		this->AddBullet(pt,4,2,sfid,270);
		this->AddBullet(pt,4,2,sfid,315);
		this->AddBullet(pt,4,2,sfid,258);
		this->AddBullet(pt,4,2,sfid,278);
		this->AddBullet(pt,4,2,sfid,327);
		this->AddBullet(pt,4,2,sfid,207);
		this->b_counter=0;
		SetState(boss_normalattack);
		return 1;
	}
	return 0;

}


//************************************
// Method:    normal_attack3
// FullName:  cls_boss::normal_attack3
// Access:    public 
// Returns:   int
// Qualifier: 快速一般的散弹
// Parameter: int sfid
//************************************
//##ModelId=4B02B4E80399
int cls_boss::normal_attack3(int sfid)
{
	floatpoint pt;
	
	//++angle;
	if (++this->b_counter<40)
	{
	}
	else{
		pt.x=this->GetPos().x+43;
		pt.y=this->GetPos().y+109;
		this->AddBullet(pt,8,2,sfid,255);
		this->AddBullet(pt,8,2,sfid,270);
		this->AddBullet(pt,8,2,sfid,315);
		this->AddBullet(pt,8,2,sfid,252);
		this->AddBullet(pt,8,2,sfid,263);
		this->AddBullet(pt,8,2,sfid,307);
		this->AddBullet(pt,8,2,sfid,227);
		pt.x=this->GetPos().x+143;
		pt.y=this->GetPos().y+109;
		this->AddBullet(pt,8,2,sfid,255);
		this->AddBullet(pt,8,2,sfid,270);
		this->AddBullet(pt,8,2,sfid,315);
		this->AddBullet(pt,8,2,sfid,258);
		this->AddBullet(pt,8,2,sfid,278);
		this->AddBullet(pt,8,2,sfid,327);
		this->AddBullet(pt,8,2,sfid,207);
		this->b_counter=0;
		SetState(boss_normalattack4);
		return 1;
	}
	return 0;

}


//************************************
// Method:    normal_attack1
// FullName:  cls_boss::normal_attack1
// Access:    public 
// Returns:   int
// Qualifier:迎面4发子弹
//************************************
//##ModelId=4B02B4E803C8
int cls_boss::normal_attack4(int sfid)
{
	floatpoint pt;
	
	//++angle;
	if (++b_counter<80 )
	{
	}
	else if (b_counter>=80 && b_counter<200)
	{
		//pt.x=this->GetPos().x+43;
		//pt.y=this->GetPos().y+109;
		//this->AddBullet(pt,6,270);
		pt.x=this->GetPos().x+33;
		pt.y=this->GetPos().y+69;
		this->AddBullet(pt,15,1,sfid,270);
// 		pt.x=this->GetPos().x+143;
// 		pt.y=this->GetPos().y+109;
// 		this->AddBullet(pt,6,270);
 		pt.x=this->GetPos().x+153;
 		pt.y=this->GetPos().y+69;
 		this->AddBullet(pt,15,1,sfid,270);
		this->b_counter=200;
	}
	if (b_counter>=200 && b_counter<206){
		//停几帧再发第二击
	}
	else if (b_counter>=206)
	{
		pt.x=this->GetPos().x+33;
		pt.y=this->GetPos().y+69;
		this->AddBullet(pt,15,1,sfid,270);
			pt.x=this->GetPos().x+153;
			pt.y=this->GetPos().y+69;
			this->AddBullet(pt,15,1,sfid,270);
		this->b_counter=0;
		SetState(boss_normalattack3);
		return 1;
	}

	return 0;
}
//DEL void cls_boss::SetMoveState(int s)
//DEL {
//DEL 	
//DEL }
