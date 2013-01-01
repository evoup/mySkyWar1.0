// class_ai.h: interface for the class_ai class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4786)
#ifndef INCLUDE_CLASS_AI_H
#define INCLUDE_CLASS_AI_H
//
#include <windows.h>
#include <ddraw.h>
#include <string>
#include <vector>
#include "function.h"
#include "myengine.h"
//LUA include 
extern "C"
{
#include  "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


extern HWND globalhwnd;


//定义一个从lua传回来的数值组成的结构
//##ModelId=4B02B4E900CB
extern struct movedata{
	//##ModelId=4B02B4E900CD
float x;//脚本移动后的x坐标
	//##ModelId=4B02B4E900CE
float y;//脚本移动后的y坐标
	//##ModelId=4B02B4E900CF
float angle;//脚本移动后的运动角度
}*movedata_struct;

using namespace std;
//##ModelId=4A1E88FC0067
class cls_ai  
{
private:
	//AI文件目录
	//##ModelId=4A2B8244001F
	char * ai_file_dir;

	//表示当前打开的AI脚本队列
	//##ModelId=4A2B71D0005D
	vector <string > openedfile;

	//LUA句柄
	//##ModelId=4A2B92B6030E
	lua_State * L;

public:
	//设置AI文件路径
	//##ModelId=4A2B819D0138
	void Set_AI_File_Dir(char * dirname);

	//调用LUA用AI不同模式计算新的坐标,LUA返回值之后用劈开函数
	//获取,再构造成POINT做为返回值。
	//##ModelId=4A2B758102CE
	floatpoint GetNewPos(floatpoint src_point  , int mode = 0);

	//带有运动角度改变的移动
	//##ModelId=4B02B4E900FA
	movedata GetNewAnglePos(floatpoint src_point,int speed,int mode=0,float angle=-1);

	//打开AI脚本
	//##ModelId=4A2B734100DA
	int Open_AI_File(char * filename);

	//##ModelId=4A1E88FC0068
	cls_ai();
	//##ModelId=4A1E88FC0069
	virtual ~cls_ai();

};
//
#endif



