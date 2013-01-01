// class_tank.cpp: implementation of the class_tank class.
//
//////////////////////////////////////////////////////////////////////

#include "class_tank.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=4B02B4E401B8
cls_tank::cls_tank()
{

}

//##ModelId=4B02B4E401B9
cls_tank::~cls_tank()
{

}

//##ModelId=4B02B4E401B6
int cls_tank::AimTarget(cls_player * player,int jiajiao)
{
	
	try{

		if (player->GetPos().x > this->GetPos().x)
		{
			dynamic_cast<cls_tank *>(this)->current_frame=7;
		}
		else if (player->GetPos().x < this->GetPos().x)
		{
			dynamic_cast<cls_tank *>(this)->current_frame=5;
		}
		if (abs(player->GetPos().x-this->GetPos().x)<=20)
		{
			dynamic_cast<cls_tank *>(this)->current_frame=6;
		}
	}
	catch(...){
	}
	return 1;
}





















