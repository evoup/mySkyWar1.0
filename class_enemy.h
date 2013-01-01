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
//class cls_sprite;//effective c++类提前声明？
//敌机类：实现自动开火，实现子弹自己销毁，实现走向。

typedef enum ENEMY_FIRE_STATE{
	/*普通攻击*/
	enemy_normalattack=1,
	/*普通攻击1*/
	enemy_normalattack1=2,
	/*普通攻击3*/
	enemy_normalattack2=3,
	/*普通攻击4*/
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
	//攻击状态(Todo:和AI关联)
	int fire_state;

	//移动状态（Todo:和AI关联）
	int move_state;

	//##ModelId=4B02B4E70222
	int item;



	//暂时无用，油料
	//int nOil;

	//飞机速度
	//##ModelId=4A28D60B02FE
	int nSpeed;
	
	//显示方式，如果被击中，就转向爆炸状态，以便飞机
	//显示函数显示爆炸效果，比如，正常是1，消失是0，
	//而爆炸就是-15。这样，显示函数可以
	//单纯的按照这个植，在SUFFACE中计算图象位置
	//直接显示就可以了，每次+1，这样-15加了15次就
	//变成0了，动画也显示了，飞机也消失了，不需要
	//其他处理。这样的方法也可以用在子弹的处理上，
	//##ModelId=4A28D60B02FF
	int nDisplayState;

	//定时刷新子弹链表，除去已经爆炸或消失的子弹
	//##ModelId=4A28D60B0300
	bool FreshBullet();

	//飞机x坐标
	//##ModelId=4A28D60B030D
	int x;

	//飞机y坐标
	//##ModelId=4A28D60B030E
	int y;


	//动画帧数
	//##ModelId=4A28D60B030F
	int seqnum;
	
	//动画序列
	//##ModelId=4A28D60B0311
	vector<int > aniseq; 

	//设置动画序列数
	//##ModelId=4A28D60B0315
	void SetSeqNum(int seqnum);

	//帧速
	//##ModelId=4A28D60B031C
	int velocity;

	//动画计时器,循环控制动画播放
	//##ModelId=4A28D60B031D
	int counter;

	//AI状态机当前状态
	//##ModelId=4B02B4E70261
	int ai;




	//##ModelId=4A28D60B031F
struct bullet *stBullet; //飞机所发出的所有子弹的链表
	//##ModelId=4A28D60B0324
struct planedisplay PlaneDisplay; //即有关飞机的位图信息指针、飞机有效区域等信息

	//子弹链表
	//##ModelId=4A28D60B0329
	std::list<bullet * > bullets_list;

	//迭代sprite_handler_list对象
	//##ModelId=4A28D60B032F
	std::list<bullet * >::iterator bullet_iter,temp_bullet_iter;
	//##ModelId=4B02B4E702B0
	cls_player *player;
public:
	//是否使用状态机,如果使用的话使用封装的方法addBullet
	int use_fire_fsm;

	//获取开火状态
	int GetFireState();
	//攻击态势
	int attack(int);
	//攻击态势1
	int attack1(int);
	//攻击态势2
	int attack2(int);
	//攻击态势3
	int attack3(int);

	//设置开火状态
	void SetFireState(int);
	//获取移动状态
	void SetMoveState(int);
	//返回奖励类型
	//##ModelId=4B02B4E702BF
	int GetBonus();

	//设置被击倒败给玩家的奖励
	//##ModelId=4B02B4E702EE
	void SetBonus(int);

	//##ModelId=4B02B4E7033C
	bullet GetBulletType(int b_type,int sfid);
	//动画帧数据链表(包含要碰撞检测的长宽等,Todo:在这里加其他检测点等)
	//##ModelId=4A28D60B0360
	std::vector<anidata * > anidata_list;

	//已经死亡,如何附带的子弹链表为空则销毁本类对象
	//##ModelId=4B02B4E8000F
	bool isdie;

	//获取AI状态机当前状态
	//##ModelId=4B02B4E8004E
	int GetAiState();

	//设置AI状态机当前状态
	//##ModelId=4B02B4E8006D
	void SetAiState(int);
	//设置HP
	//##ModelId=4A335A4A004E
	void SetHP(int);

	//在主循环里调用此函数，用来更新精灵帧，这个函数考虑从精灵
	//类继承过来，但是目前还没做！
	//##ModelId=4A28D60B0333
	bool UpdateFrame();

	//获取当前帧
	//##ModelId=4A28D60B0334
	int GetCurrentFrame();

	//获取当前坐标是否有子弹，并返回子弹指针，这是为了判断敌机
	//是否被击中所设计的函数，
	//##ModelId=4A28D60B0335
	struct bullet * IsAttack(cls_player *);

	//每隔设定的 TickTime，按Bullet的属性更新,Bullet的坐标，就可以形成复杂的子弹效果
	//##ModelId=4A28D60B033D
	void RunBullet();

	//给飞机加入子弹（节点），敌人加子弹要用AI来检测是否逼近玩家(不需要)，AI设置子弹方向。
	//##ModelId=4A28D60B033E
	void AddBullet(floatpoint pt,int speed,int btype,int sfid,int direction=270);

	//画出子弹,参数，精灵类对象，画面管理器里的画面id
	//##ModelId=4A28D60B0340
	void DrawBullet(const cls_sprite_surface * s/*,int sfid*/,int xoffset=0,int yoffset=0);

	//设定飞机坐标
	//##ModelId=4A28D60B0343
	void SetPos(floatpoint point);

	//获取飞机的坐标
	//##ModelId=4A28D60B034B
	POINT GetPos();

	//获取帧数据
	//anidata * GetAniData(anidata * );

	//当前动画帧，和它有关的有2个成员变量，velocity和counter
	//##ModelId=4A28D60B034C
	int current_frame;



	//一个画面管理器里元素的id，代表是哪个精灵素材画面
	//##ModelId=4A28D60B034D
	int sprite_surface_id;

	//输入"0,1,2,5,4,6"这样的数据送到成员变量aniseq容器里供动画时迭代
	//##ModelId=4A28D60B034E
	void SetAnimateSequence(string tokenword);

	//设置帧速
	//##ModelId=4A28D60B0350
	void SetVelocity(int);

	//画出自己
	//##ModelId=4A28D60B0352
	void DrawMe(const cls_sprite_surface * s,int sfid);

	//设置每一帧动画大小，用劈开字符串来算
	//##ModelId=4A28D60B035C
	void SetAnimateData(string word);

	//单位HP(暂时不私有属性，Todo:换到private)
	//##ModelId=4A2B70C0006D
	int HP;

	//单位发子弹的延时
	//##ModelId=4B02B4E802AF
	int iFireInputDelay;

	//用来给子弹延时的计时器
	//##ModelId=4B02B4E802EE
	int b_counter;

	//用来给移动状态延时的计时器
	int m_counter;

	//子弹种类
	//##ModelId=4B02B4E8033C
	int b_type;/**0为1次按照夹角公式发射1颗normal子弹(默认)
	1为朝下3枚子弹，外侧的2枚各展开45度数**/

	//##ModelId=4A28D60B035E
	cls_enemy();



	//迭代anidata_list对象
	//##ModelId=4A28D60B036C
	std::vector<anidata * >::iterator anidata_iter;

	//##ModelId=4A28D60B0370
	virtual ~cls_enemy();
	
};

//
#endif 
