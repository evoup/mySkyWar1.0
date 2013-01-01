// class_boss.h: interface for the cls_boss class.
//
//////////////////////////////////////////////////////////////////////

#ifndef INCLUDE_CLASS_BOSS
#define INCLUDE_CLASS_BOSS
//
#include "class_enemy.h"


typedef enum BOSS_FIRE_STATE{
	/*��ͨ����*/
	boss_normalattack=1,
	/*��ͨ����1*/
	boss_normalattack1=2,
	/*��ͨ����3*/
	boss_normalattack2=3,
	/*��ʱ�޵���˸��*/
	boss_flikering=4,
	boss_normalattack3=4,
	boss_normalattack4=5
};


//##ModelId=4A3351E40000
class cls_boss : public cls_enemy  
{
public:
	//##ModelId=4B02B4E80399
	int normal_attack2(int sfid);
	//##ModelId=4B02B4E803C8
	int normal_attack1(int sfid);
	//bool UpdateFrame();
	//��ͨ����
	//##ModelId=4B02B4E9001F
	int normal_attack(int sfid);

	int normal_attack3(int sfid);
		int normal_attack4(int sfid);
	//��ȡ״̬
	//##ModelId=4B02B4E9004E
	int GetState();

	//����״̬
	//##ModelId=4B02B4E9005D
	void SetState(int);

	//���伤����
	//##ModelId=4A335455035B
	 FireLaser();

	//##ModelId=4A3353910251
	cls_boss();
	//##ModelId=4A3353910252
	virtual ~cls_boss();

private:
	//״̬��boss��״̬
	//##ModelId=4B02B4E9009C
	int state;
};

//
#endif
