// class_sprite.h: interface for the cls_sprite class.
//
//////////////////////////////////////////////////////////////////////

#ifndef INCLUDE_CLASS_SPRITE_H
#define INCLUDE_CLASS_SPRITE_H
//
#define KEYSTATE(key) ((GetAsyncKeyState(key) & 0x8000) ? TRUE : FALSE)
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "myengine.h"
#include <list>
#include <stdlib.h>
#include <malloc.h>
#include <vector>
#include "class_sprite_surface.h"
#include "function.h"
extern LPDIRECTDRAWSURFACE7 lpddsback;
//�����࣬ʵ��һ�㾫�����������player��boss�Ļ���

typedef enum acts{
	act_right=0,
	act_upright=1,
	act_up=2,
	act_upleft=3,
	act_left=4,
	act_downleft=5,
	act_down=6,
	act_downright=7
};
typedef struct MOVESTATE{
	
	int movetime;//��������ʱ��
	
	int move;//���Ӧһ��actsö�����ͳ�Ա
	
	int aimoveid;//��ӦLUA��AI�˶�ģʽid
	
}* STRUCT_MOVE_STATE;

//##ModelId=4A1E966F0299
class cls_sprite  
{
protected:
	//�˶��ĽǶȣ����ݼнǹ�ʽ������Ǹ�
	//##ModelId=4B02B4E401F4
	float angle;
	//�ƶ�״̬����
	list<MOVESTATE > movelist; 
	//�ƶ�������,���������ƶ�״̬�л�
	int movecounter;
	


private:



	//ѭ�����ŵĴ���
	//##ModelId=4A2D427F031C
	int loopnum;



	//�ٶ�
	//##ModelId=4A1EA76C0364
	int nSpeed;
	
	//x����
	//##ModelId=4A1E9984020D
	int x;

	//y����
	//##ModelId=4A1E99C10096
	int y;

	//����֡��
	//##ModelId=4A1FFCFB0063
	int seqnum;
	
	//��������
	//##ModelId=4A1FFCFB0067
	vector<int > aniseq; 

	//���������ŵĴ�����һ�����й��̾���һ��
	//##ModelId=4A334DB3031C
	int m_played_time;

	//֡��
	//##ModelId=4A1FFCFB0075
	int velocity;

	//������ʱ��,ѭ�����ƶ�������
	//##ModelId=4A1FFCFB0082
	int counter;



	//����anidata_list����
	//##ModelId=4A28D60B01D5
	std::vector<anidata * >::iterator anidata_iter;

	//һ���󶨵�����������ָ��
	//##ModelId=4A334DB3036C
	cls_sprite_surface * m_spsf;


public:

	//�Ƿ�ѭ���ƶ���״̬��
	int loopmove;

	//ѹ���ƶ�״̬����
	int PushMoveState(MOVESTATE  &);

	//�Ƿ�ʹ���ƶ���״̬��
	int use_move_fsm;

	//����֡������������Ҫ��ײ���ĳ����,Todo:���������������ȣ�
	//##ModelId=4A28D60B01C7
	std::vector<anidata * > anidata_list;

	//##ModelId=4B02B4E402BF
	int GetNspeed();
	//�����˶��ٶ�
	//##ModelId=4B02B4E402DE
	void SetNspeed(int);

	//����֡��
	//##ModelId=4B02B4E4032C
	int GetVelocity();

	//��ȡ�˶��Ƕ�
	//##ModelId=4B02B4E4034B
	float GetAngle();

	//�����˶��Ƕ�
	//##ModelId=4B02B4E4036B
	void SetAngle(float);



	/**��������������������ը����Ч�����飬Ĭ����0����Ч����1������**/
	//##ModelId=4B02B4E403B9
	int sprite_type;

	//���þ��鶯��ѭ�����ŵĴ���
	//##ModelId=4A2D42A801C5
	void SetLoopNum(int loopnum);

	//����ѭ������ô˺������������¾���֡������������ǴӾ���
	//��̳й���?
	//##ModelId=4A1FFE8E015D
	bool UpdateFrame();

	//��ȡ��ǰ֡
	//##ModelId=4A1FFD74012E
	int GetCurrentFrame();

	//�����Լ�
	//##ModelId=4A28D60B0213
	void DrawMe(const cls_sprite_surface * s,int sfid,int xoffset=0,int yoffset=0,int flag=0);

	//�趨��������
	//##ModelId=4A1EAA500326
	void SetPos(floatpoint point);

	//��ȡ���������
	//##ModelId=4A1EA94701CE
	floatpoint GetPos();

	//��ǰ����֡�������йص���2����Ա������velocity��counter
	//##ModelId=4A1FFCFB0092
	int current_frame;



	//һ�������������Ԫ�ص�id���������ĸ������زĻ���
	//##ModelId=4A1FFCFB0093
	int sprite_surface_id;

	//����"0,1,2,5,4,6"�����������͵���Ա����aniseq�����﹩����ʱ����
	//##ModelId=4A1FFCFB0094
	void SetAnimateSequence(string tokenword);

	//����֡��
	//##ModelId=4A28D60B0232
	void SetVelocity(int speed);

	//����ÿһ֡������С���������ַ�������
	//##ModelId=4A28D60B0242
	void cls_sprite::SetAnimateData(string tokenword);

	//���ö���������
	//##ModelId=4A1FFCFB0073
	void SetSeqNum(int seqnum);

	//�����ƶ������ľ��鶯��
	//##ModelId=4A334DB303D8
	bool PlayTime(int t,int);

	//��󶨻��������İ󶨣���ʵ�������������һ������ָ��
	//##ModelId=4A334DB400AB
	bool Bind(cls_sprite_surface *);

	//##ModelId=4A1E966F029A
	cls_sprite();

	//##ModelId=4A1E966F029B
	virtual ~cls_sprite();

};

//
#endif // !defined(AFX_cls_sprite_H__8884DE32_9B94_40CA_BD3A_405CB07DEF13__INCLUDED_)
