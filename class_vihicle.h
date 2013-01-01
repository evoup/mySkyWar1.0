// class_vihicle.h: interface for the cls_vihicle class.
//
//////////////////////////////////////////////////////////////////////
#ifndef INCLUDE_CLASS_VIHIVLE_H
#define INCLUDE_CLASS_VIHIVLE_H
//


#include "class_enemy.h"

//##ModelId=4B02B4E401A5
class cls_vihicle : public cls_enemy  
{
public:
	//×ªÍä
	//##ModelId=4B02B4E401A7
	int TurnAround();
	//##ModelId=4B02B4E401A8
	cls_vihicle();
	//##ModelId=4B02B4E401A9
	virtual ~cls_vihicle();

};
//
#endif
