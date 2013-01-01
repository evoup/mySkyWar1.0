// class_enemy.h: interface for the cls_enemy class.
//
//////////////////////////////////////////////////////////////////////

#ifndef INCLUDE_CLASS_ENEMY_H
#define INCLUDE_CLASS_ENEMY_H
//
#define KEYSTATE(key) ((GetAsyncKeyState(key) & 0x8000) ? TRUE : FALSE)
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "myengine.h"
#include <list>
#include <stdlib.h>
#include <malloc.h>
#include "class_sprite.h"
#include "class_sprite_surface.h"
#include <math.h>
#include "class_player.h"
extern LPDIRECTDRAWSURFACE7 lpddsback;
//class cls_sprite;//effective c++����ǰ������
//�л��ࣺʵ���Զ�����ʵ���ӵ��Լ����٣�ʵ������

typedef enum ENEMY_FIRE_STATE{
	/*��ͨ����*/
	enemy_normalattack=1,
	/*��ͨ����1*/
	enemy_normalattack1=2,
	/*��ͨ����3*/
	enemy_normalattack2=3,
	/*��ͨ����4*/
	enemy_normalattack3=4,
};

typedef enum ENEMY_MOVE_STATE{
	enemy_stop=0,
	enemy_moveleft=1,
	enemy_movetop=2,
	enemy_moveright=3,
	enemy_movebottom=4,
	enemy_moveleftbottom=5
};

//##ModelId=4A28D60B02FD
class cls_enemy :public cls_sprite
{
	friend class cls_player;
private:
	//����״̬(Todo:��AI����)
	int fire_state;

	//�ƶ�״̬��Todo:��AI������
	int move_state;

	//##ModelId=4B02B4E70222
	int item;



	//��ʱ���ã�����
	//int nOil;

	//�ɻ��ٶ�
	//##ModelId=4A28D60B02FE
	int nSpeed;
	
	//��ʾ��ʽ����������У���ת��ը״̬���Ա�ɻ�
	//��ʾ������ʾ��ըЧ�������磬������1����ʧ��0��
	//����ը����-15����������ʾ��������
	//�����İ������ֲ����SUFFACE�м���ͼ��λ��
	//ֱ����ʾ�Ϳ����ˣ�ÿ��+1������-15����15�ξ�
	//���0�ˣ�����Ҳ��ʾ�ˣ��ɻ�Ҳ��ʧ�ˣ�����Ҫ
	//�������������ķ���Ҳ���������ӵ��Ĵ����ϣ�
	//##ModelId=4A28D60B02FF
	int nDisplayState;

	//��ʱˢ���ӵ�������ȥ�Ѿ���ը����ʧ���ӵ�
	//##ModelId=4A28D60B0300
	bool FreshBullet();

	//�ɻ�x����
	//##ModelId=4A28D60B030D
	int x;

	//�ɻ�y����
	//##ModelId=4A28D60B030E
	int y;


	//����֡��
	//##ModelId=4A28D60B030F
	int seqnum;
	
	//��������
	//##ModelId=4A28D60B0311
	vector<int > aniseq; 

	//���ö���������
	//##ModelId=4A28D60B0315
	void SetSeqNum(int seqnum);

	//֡��
	//##ModelId=4A28D60B031C
	int velocity;

	//������ʱ��,ѭ�����ƶ�������
	//##ModelId=4A28D60B031D
	int counter;

	//AI״̬����ǰ״̬
	//##ModelId=4B02B4E70261
	int ai;




	//##ModelId=4A28D60B031F
struct bullet *stBullet; //�ɻ��������������ӵ�������
	//##ModelId=4A28D60B0324
struct planedisplay PlaneDisplay; //���йطɻ���λͼ��Ϣָ�롢�ɻ���Ч�������Ϣ

	//�ӵ�����
	//##ModelId=4A28D60B0329
	std::list<bullet * > bullets_list;

	//����sprite_handler_list����
	//##ModelId=4A28D60B032F
	std::list<bullet * >::iterator bullet_iter,temp_bullet_iter;
	//##ModelId=4B02B4E702B0
	cls_player *player;
public:
	//�Ƿ�ʹ��״̬��,���ʹ�õĻ�ʹ�÷�װ�ķ���addBullet
	int use_fire_fsm;

	//��ȡ����״̬
	int GetFireState();
	//����̬��
	int attack(int);
	//����̬��1
	int attack1(int);
	//����̬��2
	int attack2(int);
	//����̬��3
	int attack3(int);

	//���ÿ���״̬
	void SetFireState(int);
	//��ȡ�ƶ�״̬
	void SetMoveState(int);
	//���ؽ�������
	//##ModelId=4B02B4E702BF
	int GetBonus();

	//���ñ������ܸ���ҵĽ���
	//##ModelId=4B02B4E702EE
	void SetBonus(int);

	//##ModelId=4B02B4E7033C
	bullet GetBulletType(int b_type,int sfid);
	//����֡��������(����Ҫ��ײ���ĳ����,Todo:����������������)
	//##ModelId=4A28D60B0360
	std::vector<anidata * > anidata_list;

	//�Ѿ�����,��θ������ӵ�����Ϊ�������ٱ������
	//##ModelId=4B02B4E8000F
	bool isdie;

	//��ȡAI״̬����ǰ״̬
	//##ModelId=4B02B4E8004E
	int GetAiState();

	//����AI״̬����ǰ״̬
	//##ModelId=4B02B4E8006D
	void SetAiState(int);
	//����HP
	//##ModelId=4A335A4A004E
	void SetHP(int);

	//����ѭ������ô˺������������¾���֡������������ǴӾ���
	//��̳й���������Ŀǰ��û����
	//##ModelId=4A28D60B0333
	bool UpdateFrame();

	//��ȡ��ǰ֡
	//##ModelId=4A28D60B0334
	int GetCurrentFrame();

	//��ȡ��ǰ�����Ƿ����ӵ����������ӵ�ָ�룬����Ϊ���жϵл�
	//�Ƿ񱻻�������Ƶĺ�����
	//##ModelId=4A28D60B0335
	struct bullet * IsAttack(cls_player *);

	//ÿ���趨�� TickTime����Bullet�����Ը���,Bullet�����꣬�Ϳ����γɸ��ӵ��ӵ�Ч��
	//##ModelId=4A28D60B033D
	void RunBullet();

	//���ɻ������ӵ����ڵ㣩�����˼��ӵ�Ҫ��AI������Ƿ�ƽ����(����Ҫ)��AI�����ӵ�����
	//##ModelId=4A28D60B033E
	void AddBullet(floatpoint pt,int speed,int btype,int sfid,int direction=270);

	//�����ӵ�,��������������󣬻����������Ļ���id
	//##ModelId=4A28D60B0340
	void DrawBullet(const cls_sprite_surface * s/*,int sfid*/,int xoffset=0,int yoffset=0);

	//�趨�ɻ�����
	//##ModelId=4A28D60B0343
	void SetPos(floatpoint point);

	//��ȡ�ɻ�������
	//##ModelId=4A28D60B034B
	POINT GetPos();

	//��ȡ֡����
	//anidata * GetAniData(anidata * );

	//��ǰ����֡�������йص���2����Ա������velocity��counter
	//##ModelId=4A28D60B034C
	int current_frame;



	//һ�������������Ԫ�ص�id���������ĸ������زĻ���
	//##ModelId=4A28D60B034D
	int sprite_surface_id;

	//����"0,1,2,5,4,6"�����������͵���Ա����aniseq�����﹩����ʱ����
	//##ModelId=4A28D60B034E
	void SetAnimateSequence(string tokenword);

	//����֡��
	//##ModelId=4A28D60B0350
	void SetVelocity(int);

	//�����Լ�
	//##ModelId=4A28D60B0352
	void DrawMe(const cls_sprite_surface * s,int sfid);

	//����ÿһ֡������С���������ַ�������
	//##ModelId=4A28D60B035C
	void SetAnimateData(string word);

	//��λHP(��ʱ��˽�����ԣ�Todo:����private)
	//##ModelId=4A2B70C0006D
	int HP;

	//��λ���ӵ�����ʱ
	//##ModelId=4B02B4E802AF
	int iFireInputDelay;

	//�������ӵ���ʱ�ļ�ʱ��
	//##ModelId=4B02B4E802EE
	int b_counter;

	//�������ƶ�״̬��ʱ�ļ�ʱ��
	int m_counter;

	//�ӵ�����
	//##ModelId=4B02B4E8033C
	int b_type;/**0Ϊ1�ΰ��ռнǹ�ʽ����1��normal�ӵ�(Ĭ��)
	1Ϊ����3ö�ӵ�������2ö��չ��45����**/

	//##ModelId=4A28D60B035E
	cls_enemy();



	//����anidata_list����
	//##ModelId=4A28D60B036C
	std::vector<anidata * >::iterator anidata_iter;

	//##ModelId=4A28D60B0370
	virtual ~cls_enemy();
	
};

//
#endif 
