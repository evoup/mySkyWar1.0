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
	/*普通状态*/
	normaling=1,
	/*开始任务起飞中*/
	starting=2,
	/*完成任务离开中*/
	exitareaing=3,
	/*暂时无敌闪烁中*/
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
//PLAYER的飞机类：实现左右控制，实现开火，实现子弹自己销毁。
//##ModelId=4A28D60B0271
class cls_player  
{
	friend class cls_enemy;
private:
	//##ModelId=4B02B4E501E4
	int wait_flickering_time;
	//##ModelId=4B02B4E50222
	int flickering_count;
	//等待离开计时器
	//##ModelId=4B02B4E50271
	int wait_leave_time;

	//状态，可操控和不可操控（起飞等状态）
	//##ModelId=4B02B4E502BF
	int state;

	//威力的等级
	//##ModelId=4B02B4E5030D
	int level;


	//暂时无用，油料
	//##ModelId=4A28D60B0272
	int nOil;

	//飞机速度
	//##ModelId=4A28D60B0273
	int nSpeed;
	
	//显示方式，如果被击中，就转向爆炸状态，以便飞机
	//显示函数显示爆炸效果，比如，正常是1，消失是0，
	//而爆炸就是-15。这样，显示函数可以
	//单纯的按照这个植，在SUFFACE中计算图象位置
	//直接显示就可以了，每次+1，这样-15加了15次就
	//变成0了，动画也显示了，飞机也消失了，不需要
	//其他处理。这样的方法也可以用在子弹的处理上，
	//##ModelId=4A28D60B0274
	int nDisplayState;

	//定时刷新子弹链表，除去已经爆炸或消失的子弹
	//##ModelId=4A28D60B0275
	bool FreshBullet();

	//飞机x坐标
	//##ModelId=4A28D60B0276
	int x;

	//飞机y坐标
	//##ModelId=4A28D60B0277
	int y;

	//动画帧数
	//##ModelId=4A28D60B0280
	int seqnum;
	
	//动画序列
	//##ModelId=4A28D60B0282
	vector<int > aniseq; 

	//设置动画序列数
	//##ModelId=4A28D60B0286
	void SetSeqNum(int seqnum);

	//帧速
	//##ModelId=4A28D60B0288
	int velocity;

	//动画计时器,循环控制动画播放
	//##ModelId=4A28D60B0289
	int counter;

	//##ModelId=4A28D60B0291
struct bullet *stBullet; //飞机所发出的所有子弹的链表
	//##ModelId=4A28D60B0296
struct planedisplay PlaneDisplay; //即有关飞机的位图信息指针、飞机有效区域等信息

	//子弹链表
	//##ModelId=4A28D60B029B
	std::list<bullet * > bullets_list;

	//迭代sprite_handler_list对象
	//##ModelId=4A34B1D702CF
	std::list<bullet * >::iterator bullet_iter;
	//##ModelId=4A34B1D702DF
	std::list<bullet * >::iterator temp_bullet_iter;



	//迭代anidata_list对象
	//##ModelId=4A28D60B02B0
	std::vector<anidata * >::iterator anidata_iter;



public:
	//更新导弹画面，因为导弹是多帧动画
	//##ModelId=4B02B4E5036B
	void UpdateMissileSurface(MISSILE_SURFACE *);
	//半隐身
	//##ModelId=4B02B4E503C8
	int GoFlikering();
	//离开
	//##ModelId=4B02B4E60000
	int GoLeaving();

	//起飞
	//##ModelId=4B02B4E6002E
	int GoStarting();

	//获取状态
	//##ModelId=4B02B4E6005D
	int GetState();

	//##ModelId=4B02B4E6007D
	void SetState(int);
	//再起飞（被击毁再来，一段时间内无敌，此时无法控制，是调用脚本来处理？）
	//##ModelId=4B02B4E600DA
	int Restart();

	//##ModelId=4B02B4E60109
	int GetLevel();
	//威力升级
	//##ModelId=4B02B4E60138
	int LevelUp();

	//获取子弹类型Todo想办法写到配置文件里
	//##ModelId=4B02B4E60167
	bullet GetBulletType(int btype,int sfid);

	//动画帧数据链表（包含要碰撞检测的长宽等,Todo:在这里加其他检测点等）
	//##ModelId=4A28D60B02A5
	std::vector<anidata * > anidata_list;
	//##ModelId=4B02B4E60232
	int GetHp();
	//##ModelId=4B02B4E60261
	void SetHp(int);
	//##ModelId=4B02B4E602C0
	cls_enemy * enemy;

	//主角生命值
	//##ModelId=4B02B4E602CE
	int HP;//Todo：改成public

	//在主循环里调用此函数，用来更新精灵帧，这个函数考虑从精灵
	//类继承过来?
	//##ModelId=4A28D60B02B4
	bool UpdateFrame();

	//获取当前帧
	//##ModelId=4A28D60B02B5
	int GetCurrentFrame();

	//获取当前坐标是否有子弹，并返回子弹指针，这是为了判断敌机
	//是否被击中所设计的函数，
	//##ModelId=4A28D60B02B6
	int IsAttack(cls_enemy  *the_enemy);

	//每隔设定的 TickTime，按Bullet的属性更新,Bullet的坐标，就可以形成复杂的子弹效果
	//##ModelId=4A28D60B02BF
	void RunBullet(list<cls_enemy * > &);

	/*给飞机加入子弹（节点），只要每隔设定的 
	TickTime就加入一次，就可以形成连续的发弹效果
	参数1：添加所在坐标
	参数2：子弹类型，对应里面getbullettype
	参数3：这种子弹对应的画面id
	*/
	//##ModelId=4A28D60B02C0
	void AddBullet(POINT pt,int btype,int sfid);

	//画出子弹,参数，精灵类对象，画面管理器里的画面id
	//##ModelId=4A28D60B02C2
	void DrawBullet(const cls_sprite_surface * s);
	
	//画出自己
	//##ModelId=4A28D60B02C5
	void DrawMe(const cls_sprite_surface * s,int sfid);

	//设定飞机坐标
	//##ModelId=4A28D60B02CF
	void SetPos(POINT point);

	//获取飞机的坐标
	//##ModelId=4A28D60B02D1
	POINT GetPos();

	//当前动画帧，和它有关的有2个成员变量，velocity和counter
	//##ModelId=4A28D60B02D2
	int current_frame;



	//一个画面管理器里元素的id，代表是哪个精灵素材画面
	//##ModelId=4A28D60B02DE
	int sprite_surface_id;

	//输入"0,1,2,5,4,6"这样的数据送到成员变量aniseq容器里供动画时迭代
	//##ModelId=4A28D60B02DF
	void SetAnimateSequence(string tokenword);

	//设置帧速
	//##ModelId=4A28D60B02E1
	void SetVelocity(int);

	//设置每一帧动画大小，用劈开字符串来算
	//##ModelId=4A28D60B02E3
	void cls_player::SetAnimateData(string tokenword);

	//##ModelId=4A28D60B02EE
	cls_player();

	//##ModelId=4A28D60B02EF
	virtual ~cls_player();

};

//
#endif // !defined(AFX_CLS_PLAYER_H__8884DE32_9B94_40CA_BD3A_405CB07DEF13__INCLUDED_)
