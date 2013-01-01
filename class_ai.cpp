// class_ai.cpp: implementation of the class_ai class.
//
//////////////////////////////////////////////////////////////////////

#include "class_ai.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=4A1E88FC0068
cls_ai::cls_ai()
{
	L=luaL_newstate();
}

//##ModelId=4A1E88FC0069
cls_ai::~cls_ai()
{
	lua_close(L);
}


//##ModelId=4A2B734100DA
int cls_ai::Open_AI_File(char * filename)
{
	// TODO: Add your specialized code here.
	//lua_State* L=luaL_newstate();
	luaL_openlibs(L);//打开标准库
					 /*luaopen_base(L);
					 luaopen_string(L);
					 luaopen_table(L);
					 luaopen_math(L);
	luaopen_io(L);*/
	if(luaL_dofile(L,filename)){
		MessageBox(globalhwnd,"打开AI文件失败","info",MB_OK);
		return 0;
	}
	else{
		//MessageBox(globalhwnd,"打开文件OK","info",MB_OK);
		return 1;
	}
}



//##ModelId=4A2B758102CE
floatpoint cls_ai::GetNewPos(floatpoint src_point, int mode)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	lua_settop(L,0);//重置栈索引
	lua_getglobal(L,"GetNewPos");
	if(!lua_isfunction(L,-1)){
		MessageBox(globalhwnd,"脚本里没有GetNewPos函数","",MB_OK);
	}
	lua_pushnumber(L,src_point.x);
	lua_pushnumber(L,src_point.y);
	lua_pushnumber(L,mode);
	lua_call(L,3,1);
	string str=lua_tostring(L,-1);
	vector<string > v=explode(",",str);
	//MessageBox(NULL,v[0].c_str(),"坐标",MB_OK);
	//MessageBox(NULL,v[1].c_str(),"坐标",MB_OK);
	int new_x=string2int(v[0].c_str());
	int new_y=string2int(v[1].c_str());

	/*int ints=(int)lua_tonumber(L,-1);
	char buf[20];
	sprintf(buf,"%d",ints);
	MessageBox(NULL,buf,"",MB_OK);*/
	floatpoint pt;
	//pt.x=0;
	//pt.y=0;
	pt.x=new_x;
	pt.y=new_y;
	
	return pt;
}

#if 1
//##ModelId=4A2B758102CE
movedata cls_ai::GetNewAnglePos(floatpoint src_point, int speed,int mode,float angle)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	lua_settop(L,0);//重置栈索引
	lua_getglobal(L,"GetNewAnglePos");
	if(!lua_isfunction(L,-1)){
		MessageBox(globalhwnd,"脚本里没有GetNewAnglePos函数","",MB_OK);
	}
	lua_pushnumber(L,(float)src_point.x);

	lua_pushnumber(L,(float)src_point.y);
	lua_pushnumber(L,speed);
	lua_pushnumber(L,mode);
	lua_pushnumber(L,(float)angle);
	lua_call(L,5,1);
	string str=lua_tostring(L,-1);
	vector<string > v=explode(",",str);
	//MessageBox(NULL,v[0].c_str(),"坐标",MB_OK);
	//MessageBox(NULL,v[1].c_str(),"坐标",MB_OK);
	int new_x=string2int(v[0].c_str());
	int new_y=string2int(v[1].c_str());
	float new_angle;
	if (v.size()==3)//代表有3个未劈开的字符
	{
	 new_angle=string2float(v[2].c_str());
	}
	/*int ints=(int)lua_tonumber(L,-1);
	char buf[20];
	sprintf(buf,"%d",ints);
	MessageBox(NULL,buf,"",MB_OK);*/
	//POINT pt;
	//pt.x=0;
	//pt.y=0;
	//pt.x=new_x;
	//pt.y=new_y;
	movedata mdata;
	mdata.x=new_x;
	mdata.y=new_y;
	mdata.angle=new_angle;
	return mdata;
}
#endif

//##ModelId=4A2B819D0138
void cls_ai::Set_AI_File_Dir(char * dirname)
{
	// TODO: Add your specialized code here.
	this->ai_file_dir=dirname;
}


//DEL void cls_ai::SetAngle()
//DEL {
//DEL 
//DEL }
