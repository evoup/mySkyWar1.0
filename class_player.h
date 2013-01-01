// class_player.h: interface for the cls_player class.
//
//////////////////////////////////////////////////////////////////////

#ifndef INCLUDE_CLASS_PLAYER_H
#define INCLUDE_CLASS_PLAYER_H
//
#define KEYSTATE(key) ((GetAsyncKeyState(key) & 0x8000) ? TRUE : FALSE)
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "myengine.h"
#include <list>
#include <stdlib.h>
#include <malloc.h>
#include "class_sprite_surface.h"
#include "class_enemy.h"
#include "function.h"
extern LPDIRECTDRAWSURFACE7 lpddsback;

typedef enum P_STATE{
	/*��ͨ״̬*/
	normaling=1,
	/*��ʼ���������*/
	starting=2,
	/*��������뿪��*/
	exitareaing=3,
	/*��ʱ�޵���˸��*/
	flikering=4
};

//##ModelId=4B02B4E50186
typedef struct MISSILE_SURFACE_type{
int surface0;
int surface1;
int surface2;
int surface3;
int surface4;
int surface5;
int surface6;
int surface7;
int surface8;
int surface9;
int surface10;
int surface11;
int surface12;
int surface13;
int surface14;
int surface15;
}MISSILE_SURFACE,* MISSILE_SURFACE_struct;
//PLAYER�ķɻ��ࣺʵ�����ҿ��ƣ�ʵ�ֿ���ʵ���ӵ��Լ����١�
//##ModelId=4A28D60B0271
class cls_player  
{
	friend class cls_enemy;
private:
	//##ModelId=4B02B4E501E4
	int wait_flickering_time;
	//##ModelId=4B02B4E50222
	int flickering_count;
	//�ȴ��뿪��ʱ��
	//##ModelId=4B02B4E50271
	int wait_leave_time;

	//״̬���ɲٿغͲ��ɲٿأ���ɵ�״̬��
	//##ModelId=4B02B4E502BF
	int state;

	//�����ĵȼ�
	//##ModelId=4B02B4E5030D
	int level;


	//��ʱ���ã�����
	//##ModelId=4A28D60B0272
	int nOil;

	//�ɻ��ٶ�
	//##ModelId=4A28D60B0273
	int nSpeed;
	
	//��ʾ��ʽ����������У���ת��ը״̬���Ա�ɻ�
	//��ʾ������ʾ��ըЧ�������磬������1����ʧ��0��
	//����ը����-15����������ʾ��������
	//�����İ������ֲ����SUFFACE�м���ͼ��λ��
	//ֱ����ʾ�Ϳ����ˣ�ÿ��+1������-15����15�ξ�
	//���0�ˣ�����Ҳ��ʾ�ˣ��ɻ�Ҳ��ʧ�ˣ�����Ҫ
	//�������������ķ���Ҳ���������ӵ��Ĵ����ϣ�
	//##ModelId=4A28D60B0274
	int nDisplayState;

	//��ʱˢ���ӵ�������ȥ�Ѿ���ը����ʧ���ӵ�
	//##ModelId=4A28D60B0275
	bool FreshBullet();

	//�ɻ�x����
	//##ModelId=4A28D60B0276
	int x;

	//�ɻ�y����
	//##ModelId=4A28D60B0277
	int y;

	//����֡��
	//##ModelId=4A28D60B0280
	int seqnum;
	
	//��������
	//##ModelId=4A28D60B0282
	vector<int > aniseq; 

	//���ö���������
	//##ModelId=4A28D60B0286
	void SetSeqNum(int seqnum);

	//֡��
	//##ModelId=4A28D60B0288
	int velocity;

	//������ʱ��,ѭ�����ƶ�������
	//##ModelId=4A28D60B0289
	int counter;

	//##ModelId=4A28D60B0291
struct bullet *stBullet; //�ɻ��������������ӵ�������
	//##ModelId=4A28D60B0296
struct planedisplay PlaneDisplay; //���йطɻ���λͼ��Ϣָ�롢�ɻ���Ч�������Ϣ

	//�ӵ�����
	//##ModelId=4A28D60B029B
	std::list<bullet * > bullets_list;

	//����sprite_handler_list����
	//##ModelId=4A34B1D702CF
	std::list<bullet * >::iterator bullet_iter;
	//##ModelId=4A34B1D702DF
	std::list<bullet * >::iterator temp_bullet_iter;



	//����anidata_list����
	//##ModelId=4A28D60B02B0
	std::vector<anidata * >::iterator anidata_iter;



public:
	//���µ������棬��Ϊ�����Ƕ�֡����
	//##ModelId=4B02B4E5036B
	void UpdateMissileSurface(MISSILE_SURFACE *);
	//������
	//##ModelId=4B02B4E503C8
	int GoFlikering();
	//�뿪
	//##ModelId=4B02B4E60000
	int GoLeaving();

	//���
	//##ModelId=4B02B4E6002E
	int GoStarting();

	//��ȡ״̬
	//##ModelId=4B02B4E6005D
	int GetState();

	//##ModelId=4B02B4E6007D
	void SetState(int);
	//����ɣ�������������һ��ʱ�����޵У���ʱ�޷����ƣ��ǵ��ýű���������
	//##ModelId=4B02B4E600DA
	int Restart();

	//##ModelId=4B02B4E60109
	int GetLevel();
	//��������
	//##ModelId=4B02B4E60138
	int LevelUp();

	//��ȡ�ӵ�����Todo��취д�������ļ���
	//##ModelId=4B02B4E60167
	bullet GetBulletType(int btype,int sfid);

	//����֡������������Ҫ��ײ���ĳ����,Todo:���������������ȣ�
	//##ModelId=4A28D60B02A5
	std::vector<anidata * > anidata_list;
	//##ModelId=4B02B4E60232
	int GetHp();
	//##ModelId=4B02B4E60261
	void SetHp(int);
	//##ModelId=4B02B4E602C0
	cls_enemy * enemy;

	//��������ֵ
	//##ModelId=4B02B4E602CE
	int HP;//Todo���ĳ�public

	//����ѭ������ô˺������������¾���֡������������ǴӾ���
	//��̳й���?
	//##ModelId=4A28D60B02B4
	bool UpdateFrame();

	//��ȡ��ǰ֡
	//##ModelId=4A28D60B02B5
	int GetCurrentFrame();

	//��ȡ��ǰ�����Ƿ����ӵ����������ӵ�ָ�룬����Ϊ���жϵл�
	//�Ƿ񱻻�������Ƶĺ�����
	//##ModelId=4A28D60B02B6
	int IsAttack(cls_enemy  *the_enemy);

	//ÿ���趨�� TickTime����Bullet�����Ը���,Bullet�����꣬�Ϳ����γɸ��ӵ��ӵ�Ч��
	//##ModelId=4A28D60B02BF
	void RunBullet(list<cls_enemy * > &);

	/*���ɻ������ӵ����ڵ㣩��ֻҪÿ���趨�� 
	TickTime�ͼ���һ�Σ��Ϳ����γ������ķ���Ч��
	����1�������������
	����2���ӵ����ͣ���Ӧ����getbullettype
	����3�������ӵ���Ӧ�Ļ���id
	*/
	//##ModelId=4A28D60B02C0
	void AddBullet(POINT pt,int btype,int sfid);

	//�����ӵ�,��������������󣬻����������Ļ���id
	//##ModelId=4A28D60B02C2
	void DrawBullet(const cls_sprite_surface * s);
	
	//�����Լ�
	//##ModelId=4A28D60B02C5
	void DrawMe(const cls_sprite_surface * s,int sfid);

	//�趨�ɻ�����
	//##ModelId=4A28D60B02CF
	void SetPos(POINT point);

	//��ȡ�ɻ�������
	//##ModelId=4A28D60B02D1
	POINT GetPos();

	//��ǰ����֡�������йص���2����Ա������velocity��counter
	//##ModelId=4A28D60B02D2
	int current_frame;



	//һ�������������Ԫ�ص�id���������ĸ������زĻ���
	//##ModelId=4A28D60B02DE
	int sprite_surface_id;

	//����"0,1,2,5,4,6"�����������͵���Ա����aniseq�����﹩����ʱ����
	//##ModelId=4A28D60B02DF
	void SetAnimateSequence(string tokenword);

	//����֡��
	//##ModelId=4A28D60B02E1
	void SetVelocity(int);

	//����ÿһ֡������С���������ַ�������
	//##ModelId=4A28D60B02E3
	void cls_player::SetAnimateData(string tokenword);

	//##ModelId=4A28D60B02EE
	cls_player();

	//##ModelId=4A28D60B02EF
	virtual ~cls_player();

};

//
#endif // !defined(AFX_CLS_PLAYER_H__8884DE32_9B94_40CA_BD3A_405CB07DEF13__INCLUDED_)
