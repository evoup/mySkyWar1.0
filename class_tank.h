// class_tank.h: interface for the class_tank class.
//
//////////////////////////////////////////////////////////////////////

#ifndef INCLUDE_CLASS_TANK_H
#define INCLUDE_CLASS_TANK_H
//
#include "class_vihicle.h"
#include "class_player.h"
#include "myengine.h"
//##ModelId=4B02B4E401AB
class cls_tank : public cls_vihicle  
{
public:
	//炮管对准主角
	//##ModelId=4B02B4E401B6
	int AimTarget(cls_player *,int);

	//##ModelId=4B02B4E401B8
	cls_tank();
	//##ModelId=4B02B4E401B9
	virtual ~cls_tank();

private:
	int move_counter;
};
//
#endif
