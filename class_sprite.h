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
//精灵类，实现一般精灵操作，属于player和boss的基类

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
	
	int movetime;//动作持续时间
	
	int move;//会对应一个acts枚举类型成员
	
	int aimoveid;//对应LUA的AI运动模式id
	
}* STRUCT_MOVE_STATE;

//##ModelId=4A1E966F0299
class cls_sprite  
{
protected:
	//运动的角度，根据夹角公式计算的那个
	//##ModelId=4B02B4E401F4
	float angle;
	//移动状态链表
	list<MOVESTATE > movelist; 
	//移动计数器,用来触发移动状态切换
	int movecounter;
	


private:



	//循环播放的次数
	//##ModelId=4A2D427F031C
	int loopnum;



	//速度
	//##ModelId=4A1EA76C0364
	int nSpeed;
	
	//x坐标
	//##ModelId=4A1E9984020D
	int x;

	//y坐标
	//##ModelId=4A1E99C10096
	int y;

	//动画帧数
	//##ModelId=4A1FFCFB0063
	int seqnum;
	
	//动画序列
	//##ModelId=4A1FFCFB0067
	vector<int > aniseq; 

	//动画被播放的次数，一个序列过程就是一次
	//##ModelId=4A334DB3031C
	int m_played_time;

	//帧速
	//##ModelId=4A1FFCFB0075
	int velocity;

	//动画计时器,循环控制动画播放
	//##ModelId=4A1FFCFB0082
	int counter;



	//迭代anidata_list对象
	//##ModelId=4A28D60B01D5
	std::vector<anidata * >::iterator anidata_iter;

	//一个绑定到画面类对象的指针
	//##ModelId=4A334DB3036C
	cls_sprite_surface * m_spsf;


public:

	//是否循环移动简单状态机
	int loopmove;

	//压入移动状态链表
	int PushMoveState(MOVESTATE  &);

	//是否使用移动简单状态机
	int use_move_fsm;

	//动画帧数据链表（包含要碰撞检测的长宽等,Todo:在这里加其他检测点等）
	//##ModelId=4A28D60B01C7
	std::vector<anidata * > anidata_list;

	//##ModelId=4B02B4E402BF
	int GetNspeed();
	//设置运动速度
	//##ModelId=4B02B4E402DE
	void SetNspeed(int);

	//设置帧速
	//##ModelId=4B02B4E4032C
	int GetVelocity();

	//获取运动角度
	//##ModelId=4B02B4E4034B
	float GetAngle();

	//设置运动角度
	//##ModelId=4B02B4E4036B
	void SetAngle(float);



	/**代表是能量还是其他爆炸等无效果精灵，默认是0是无效果，1是能量**/
	//##ModelId=4B02B4E403B9
	int sprite_type;

	//设置精灵动画循环播放的次数
	//##ModelId=4A2D42A801C5
	void SetLoopNum(int loopnum);

	//在主循环里调用此函数，用来更新精灵帧，这个函数考虑从精灵
	//类继承过来?
	//##ModelId=4A1FFE8E015D
	bool UpdateFrame();

	//获取当前帧
	//##ModelId=4A1FFD74012E
	int GetCurrentFrame();

	//画出自己
	//##ModelId=4A28D60B0213
	void DrawMe(const cls_sprite_surface * s,int sfid,int xoffset=0,int yoffset=0,int flag=0);

	//设定精灵坐标
	//##ModelId=4A1EAA500326
	void SetPos(floatpoint point);

	//获取精灵的坐标
	//##ModelId=4A1EA94701CE
	floatpoint GetPos();

	//当前动画帧，和它有关的有2个成员变量，velocity和counter
	//##ModelId=4A1FFCFB0092
	int current_frame;



	//一个画面管理器里元素的id，代表是哪个精灵素材画面
	//##ModelId=4A1FFCFB0093
	int sprite_surface_id;

	//输入"0,1,2,5,4,6"这样的数据送到成员变量aniseq容器里供动画时迭代
	//##ModelId=4A1FFCFB0094
	void SetAnimateSequence(string tokenword);

	//设置帧速
	//##ModelId=4A28D60B0232
	void SetVelocity(int speed);

	//设置每一帧动画大小，用劈开字符串来算
	//##ModelId=4A28D60B0242
	void cls_sprite::SetAnimateData(string tokenword);

	//设置动画序列数
	//##ModelId=4A1FFCFB0073
	void SetSeqNum(int seqnum);

	//播放制定次数的精灵动画
	//##ModelId=4A334DB303D8
	bool PlayTime(int t,int);

	//与绑定画面管理类的绑定，其实就是依赖该类的一个对象指针
	//##ModelId=4A334DB400AB
	bool Bind(cls_sprite_surface *);

	//##ModelId=4A1E966F029A
	cls_sprite();

	//##ModelId=4A1E966F029B
	virtual ~cls_sprite();

};

//
#endif // !defined(AFX_cls_sprite_H__8884DE32_9B94_40CA_BD3A_405CB07DEF13__INCLUDED_)
