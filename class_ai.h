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


//����һ����lua����������ֵ��ɵĽṹ
//##ModelId=4B02B4E900CB
extern struct movedata{
	//##ModelId=4B02B4E900CD
float x;//�ű��ƶ����x����
	//##ModelId=4B02B4E900CE
float y;//�ű��ƶ����y����
	//##ModelId=4B02B4E900CF
float angle;//�ű��ƶ�����˶��Ƕ�
}*movedata_struct;

using namespace std;
//##ModelId=4A1E88FC0067
class cls_ai  
{
private:
	//AI�ļ�Ŀ¼
	//##ModelId=4A2B8244001F
	char * ai_file_dir;

	//��ʾ��ǰ�򿪵�AI�ű�����
	//##ModelId=4A2B71D0005D
	vector <string > openedfile;

	//LUA���
	//##ModelId=4A2B92B6030E
	lua_State * L;

public:
	//����AI�ļ�·��
	//##ModelId=4A2B819D0138
	void Set_AI_File_Dir(char * dirname);

	//����LUA��AI��ͬģʽ�����µ�����,LUA����ֵ֮������������
	//��ȡ,�ٹ����POINT��Ϊ����ֵ��
	//##ModelId=4A2B758102CE
	floatpoint GetNewPos(floatpoint src_point  , int mode = 0);

	//�����˶��Ƕȸı���ƶ�
	//##ModelId=4B02B4E900FA
	movedata GetNewAnglePos(floatpoint src_point,int speed,int mode=0,float angle=-1);

	//��AI�ű�
	//##ModelId=4A2B734100DA
	int Open_AI_File(char * filename);

	//##ModelId=4A1E88FC0068
	cls_ai();
	//##ModelId=4A1E88FC0069
	virtual ~cls_ai();

};
//
#endif



