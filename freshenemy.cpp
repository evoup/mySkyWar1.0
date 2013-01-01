#include "freshenemy.h"
#if 0
//第二批
if(mapdata.yCamera<7500){
	
	static the_2nd_enemy=0;
	if(the_2nd_enemy==0){
		//alertme("差不多到了");
		for (int i=0;i<=8;i++){
			enemy_list.push_back((cls_enemy *)new cls_enemy());
			pt_startpoint.x=rand()%640-rand()%14;
			pt_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
			enemy_list.back()->SetPos(ptf_startpoint);
			enemy_list.back()->sprite_surface_id=enemy0_sfid;
			enemy_list.back()->SetAnimateSequence("0,1,2,3,4,5");
			enemy_list.back()->SetAnimateData("30|29,30|29,30|29,30|29,30|29,30|29");
			enemy_list.back()->current_frame=(rand()%1);
			enemy_list.back()->SetVelocity(5);
		}
		//enemy_iter=enemy_list.begin();
		//while(enemy_iter!=enemy_list.end()){
		
		//	++enemy_iter;
		//}
		the_2nd_enemy=1;
	}
}
#endif
#if 1

//带奖励的一般直升机编队
void enemygroup_1_1(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){

	if(themap.yCamera<mapararea){//mapararea=7500
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=7;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(5);
			thelist.back()->iFireInputDelay=55;
		/*	MOVESTATE ms;
			ms.aimoveid=7;
			ms.move=7;
			ms.movetime=30;
			//MessageBeep(MB_ABORTRETRYIGNORE);
			thelist.back()->PushMoveState(ms);
			ms.aimoveid=8;
			ms.move=8;
			ms.movetime=30;
			//MessageBeep(MB_ABORTRETRYIGNORE);
			thelist.back()->PushMoveState(ms);
			thelist.back()->use_move_fsm=1;*/
			if (i==4)//带一个奖励
			{
				thelist.back()->sprite_surface_id=sfid2;//enemy3_red_sfid
				enum bonus bs=power;//加enum可以适应c编译器！
				thelist.back()->SetBonus(bs);
				thelist.back()->SetHP(5);
			}
			//thelist.back()->SetFireState(enemy_normalattack);
			//thelist.back()->use_fire_fsm=1;
		}
		Once=1;
	}
	
}

void enemygroup_1_2t(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv)
{
	if(themap.yCamera<mapararea){//mapararea=7500
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=5;i++){
		thelist.push_back((cls_enemy *)new cls_tank());
		floatpoint ptf_startpoint;		
			if (i==0)
			{
				ptf_startpoint.x=120;
				ptf_startpoint.y=-20;
			}
			else if(i==1){
				ptf_startpoint.x=140;
				ptf_startpoint.y=-58;
			}
			else if(i==2){
				ptf_startpoint.x=60;
				ptf_startpoint.y=-436;
			}
			else if(i==3){
				ptf_startpoint.x=180;
				ptf_startpoint.y=-340;
			}
			else if(i==4){
				ptf_startpoint.x=190;
				ptf_startpoint.y=-135;
			}
			else if(i==5){
				ptf_startpoint.x=220;
				ptf_startpoint.y=-77;
			}
			else if(i==6){
				ptf_startpoint.x=240;
				ptf_startpoint.y=-240;
			}
			else if(i==7){
				ptf_startpoint.x=260;
				ptf_startpoint.y=-141;
			}
			else if(i==8){
				ptf_startpoint.x=550;
				ptf_startpoint.y=-146;
			}
			else if(i==9){
				ptf_startpoint.x=450;
				ptf_startpoint.y=20;
			}
		//ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
		thelist.back()->SetPos(ptf_startpoint);
		thelist.back()->sprite_surface_id=sfid;//tank_sfid
		thelist.back()->SetAnimateSequence("0,1,2,3,4,5,6,7");
		thelist.back()->SetAnimateData("29|30,29|30,29|30,29|30,29|30,29|30,29|30,29|30");
		thelist.back()->current_frame=(rand()%1);
		thelist.back()->SetVelocity(rand()%4);
		//thelist.back()->SetFireState(enemy_normalattack);
		thelist.back()->SetHP(7);
		//thelist.back()->use_fire_fsm=1;
		//thelist.back()->use_move_fsm=1;
		//thelist.back()->loopmove=0;
		//MOVESTATE ms;
		//ms.aimoveid=9;
		//ms.move=9;
		//ms.movetime=120;
		//thelist.back()->PushMoveState(ms);
		thelist.back()->SetAiState(99);
		}
		Once=1;
	}
	
};


void enemygroup_1_3t(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv)
{
	if(themap.yCamera<mapararea){//mapararea=7500
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=9;i++){
			thelist.push_back((cls_enemy *)new cls_tank());
			floatpoint ptf_startpoint;		
			if (i==0)
			{
				ptf_startpoint.x=120;
				ptf_startpoint.y=-20;
			}
			else if(i==1){
				ptf_startpoint.x=140;
				ptf_startpoint.y=-58;
			}
			else if(i==2){
				ptf_startpoint.x=60;
				ptf_startpoint.y=-436;
			}
			else if(i==3){
				ptf_startpoint.x=180;
				ptf_startpoint.y=-340;
			}
			else if(i==4){
				ptf_startpoint.x=190;
				ptf_startpoint.y=-135;
			}
			else if(i==5){
				ptf_startpoint.x=220;
				ptf_startpoint.y=-77;
			}
			else if(i==6){
				ptf_startpoint.x=240;
				ptf_startpoint.y=-240;
			}
			else if(i==7){
				ptf_startpoint.x=260;
				ptf_startpoint.y=-141;
			}
			else if(i==8){
				ptf_startpoint.x=550;
				ptf_startpoint.y=-146;
			}
			else if(i==9){
				ptf_startpoint.x=450;
				ptf_startpoint.y=20;
			}
			//ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//tank_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5,6,7");
			thelist.back()->SetAnimateData("29|30,29|30,29|30,29|30,29|30,29|30,29|30,29|30");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(rand()%4);
			//thelist.back()->SetFireState(enemy_normalattack);
			thelist.back()->SetHP(11);
			//thelist.back()->use_fire_fsm=1;
			//thelist.back()->use_move_fsm=1;
			//thelist.back()->loopmove=0;
			//MOVESTATE ms;
			//ms.aimoveid=9;
			//ms.move=9;
			//ms.movetime=120;
			//thelist.back()->PushMoveState(ms);
			thelist.back()->SetAiState(99);
			if (i==8)
			{
				thelist.back()->SetAiState(14);
			}
			if (i==2)
			{
				thelist.back()->SetAiState(13);
			}
		}
		Once=1;
	}
	
};

//带奖励的一般编队
void enemygroup_1_2(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){//mapararea=7500
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		int lvx,sethp,firedelay;
		if (lv==0)
		{
			lvx=7;
			sethp=3;
			firedelay=55;
		}
		else if (lv==1)
		{
			lvx=10;
			sethp=5;
			firedelay=50;
		}
		else if (lv==2)
		{
			lvx=15;
			sethp=7;
			firedelay=43;
		}
		for (int i=0;i<=lvx;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(1);
			thelist.back()->iFireInputDelay=firedelay;
			thelist.back()->SetHP(sethp);
			if (i==4)//带一个奖励
			{
				thelist.back()->sprite_surface_id=sfid2;//enemy3_red_sfid
				enum bonus bs=power;//加enum可以适应c编译器！
				thelist.back()->SetBonus(bs);
				thelist.back()->SetHP(sethp+4);
				ptf_startpoint.x=300;
				ptf_startpoint.y=0;
				thelist.back()->SetPos(ptf_startpoint);
				thelist.back()->SetAiState(5);
				thelist.back()->use_fire_fsm=1;
				thelist.back()->SetFireState(enemy_normalattack2);
			}
		}
		Once=1;
	}

}



//有1个奖励的运输直升机编队
void enemygroup_1_3(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	if(themap.yCamera<mapararea){//mapararea=7500
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		int lvx,sethp;
		if (lv==0)
		{
			lvx=1;
			sethp=35;
		}
		else if (lv==1)
		{
			lvx=3;
			sethp=40;
		}
		else if (lv==2)
		{
			lvx=3;
			sethp=60;
		}
		for (int i=0;i<=lvx;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			if (ptf_startpoint.x>400)
			{
				ptf_startpoint.x=200;
			}
			if (ptf_startpoint.x<=100)
			{
				ptf_startpoint.x=100;
			}
			ptf_startpoint.y=-rand()%70;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy4_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("100|112,100|112,100|112,100|112,100|112,100|112");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(7);
			thelist.back()->SetAiState(4);
			thelist.back()->SetHP(sethp);
			thelist.back()->iFireInputDelay=55;
			thelist.back()->SetFireState(enemy_normalattack3);
			thelist.back()->use_fire_fsm=1;
			//运输直升机携带1个奖励
			if (i==1)
			{
				enum bonus bs=power;//加enum可以适应c编译器！
				thelist.back()->SetBonus(bs);
			}
			
		}
		Once=1;
	}	
}

//直线冲过来攻击的单位
void enemygroup_1_4(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		int lvx,sethp;
		if (lv==0)
		{
			lvx=6;
			sethp=10;
		}
		else if (lv==1)
		{
			lvx=7;
			sethp=12;
		}
		else if (lv==2)
		{
			lvx=8;
			sethp=16;
		}
		for (int i=0;i<=lvx;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(0);
			if (lv==0)
			{
				thelist.back()->iFireInputDelay=55;
			}
			else if (lv==1)
			{
				thelist.back()->iFireInputDelay=55;
			}
			else if (lv==2)
			{
				thelist.back()->iFireInputDelay=22;
			}
		
			thelist.back()->SetHP(sethp);
		}
		Once=1;
	}
	
}


//带奖励的一般编队
void enemygroup_1_5(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=7;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(1);
			thelist.back()->iFireInputDelay=55;
			if (i==7)//带一个奖励
			{
				thelist.back()->sprite_surface_id=sfid2;//enemy3_red_sfid
				enum bonus bs=power;//加enum可以适应c编译器！
				thelist.back()->SetBonus(bs);
				thelist.back()->SetHP(9);
				thelist.back()->SetAiState(5);
			}
		}
		Once=1;
	}
	
}

//一般编队
void enemygroup_1_6(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=8;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(1);
			thelist.back()->iFireInputDelay=55;
		}
		Once=1;
	}
	
}

//一般编队
void enemygroup_1_7(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=12;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(0);
			thelist.back()->iFireInputDelay=50;
		}
		Once=1;
	}
	
}


//一般编队
void enemygroup_1_8(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=8;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(1);
			thelist.back()->iFireInputDelay=50;
		}
		Once=1;
	}
	
}


//一般编队
void enemygroup_1_9(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=4;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(3);
			thelist.back()->iFireInputDelay=44;
		}
		Once=1;
	}
	
}


//有1个奖励的运输直升机编队
void enemygroup_1_10(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	if(themap.yCamera<mapararea){//mapararea=7500
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=3;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%552;
			if (ptf_startpoint.x>400)
			{
				ptf_startpoint.x=200;
			}
			if (ptf_startpoint.x<=100)
			{
				ptf_startpoint.x=100;
			}
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy4_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("100|112,100|112,100|112,100|112,100|112,100|112");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(7);
			thelist.back()->SetAiState(4);
			thelist.back()->SetHP(42);
			thelist.back()->iFireInputDelay=55;
			//运输直升机携带1个奖励
			if (i==1)
			{
				enum bonus bs=power;//加enum可以适应c编译器！
				thelist.back()->SetBonus(bs);
			}
			
		}
		Once=1;
	}	
}




//第二关的
//一般编队
void enemygroup_2_1(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=9;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(2);
			thelist.back()->iFireInputDelay=44;
			thelist.back()->SetHP(4);
		}
		Once=1;
	}
	
}

//带奖励的一般编队
void enemygroup_2_2(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){//mapararea=7500
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=7;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(1);
			thelist.back()->iFireInputDelay=55;
			if (i==4)//带一个奖励
			{
				thelist.back()->sprite_surface_id=sfid2;//enemy3_red_sfid
				enum bonus bs=power;//加enum可以适应c编译器！
				thelist.back()->SetBonus(bs);
				thelist.back()->SetHP(6);
				thelist.back()->SetAiState(5);
			}
		}
		Once=1;
	}

}



//有1个奖励的运输直升机编队
void enemygroup_2_3(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	if(themap.yCamera<mapararea){//mapararea=7500
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=1;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			if (ptf_startpoint.x>400)
			{
				ptf_startpoint.x=200;
			}
			if (ptf_startpoint.x<=100)
			{
				ptf_startpoint.x=100;
			}
			ptf_startpoint.y=-rand()%70;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy4_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("100|112,100|112,100|112,100|112,100|112,100|112");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(7);
			thelist.back()->SetAiState(4);
			thelist.back()->SetHP(35);
			thelist.back()->iFireInputDelay=55;
			
			//运输直升机携带1个奖励
			if (i==1)
			{
				enum bonus bs=power;//加enum可以适应c编译器！
				thelist.back()->SetBonus(bs);
			}
			
		}
		Once=1;
	}	
}

//直线冲过来攻击的单位
void enemygroup_2_4(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		int lvx,firedelay,sethp;
		if (lv==0)
		{
			lvx=16;firedelay=48;sethp=5;
		}
		else if (lv==1)
		{
			lvx=18;firedelay=47;sethp=6;
		}
		else if (lv==2)
		{
			lvx=20;firedelay=46;sethp=7;
		}
		for (int i=0;i<=lvx;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(6);
			thelist.back()->iFireInputDelay=firedelay;
			thelist.back()->SetHP(sethp);
		}
		Once=1;
	}
	
}
//直线冲过来攻击的单位
void enemygroup_2_4b(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		int lvx,firedelay,sethp;
		if (lv==0)
		{
			lvx=16;firedelay=48;sethp=7;
		}
		else if (lv==1)
		{
			lvx=18;firedelay=47;sethp=8;
		}
		else if (lv==2)
		{
			lvx=20;firedelay=46;sethp=9;
		}
		for (int i=0;i<=lvx;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(6);
			thelist.back()->iFireInputDelay=firedelay;
			thelist.back()->SetHP(sethp);
		}
		Once=1;
	}
	
}
//直线冲过来攻击的单位
void enemygroup_2_4c(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		int lvx,firedelay,sethp;
		if (lv==0)
		{
			lvx=16;firedelay=48;sethp=7;
		}
		else if (lv==1)
		{
			lvx=18;firedelay=47;sethp=8;
		}
		else if (lv==2)
		{
			lvx=20;firedelay=46;sethp=9;
		}
		for (int i=0;i<=lvx;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(6);
			thelist.back()->iFireInputDelay=firedelay;
			thelist.back()->SetHP(sethp);
		}
		Once=1;
	}
	
}

//带奖励的一般编队
void enemygroup_2_5(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=7;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(1);
			thelist.back()->iFireInputDelay=55;
			if (i==7)//带一个奖励
			{
				thelist.back()->sprite_surface_id=sfid2;//enemy3_red_sfid
				enum bonus bs=power;//加enum可以适应c编译器！
				thelist.back()->SetBonus(bs);
				thelist.back()->SetHP(9);
				thelist.back()->SetAiState(5);
			}
		}
		Once=1;
	}
	
}

//一般编队
void enemygroup_2_6(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=8;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(1);
			thelist.back()->iFireInputDelay=55;
		}
		Once=1;
	}
	
}

//一般编队
void enemygroup_2_7(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=12;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(0);
			thelist.back()->iFireInputDelay=50;
		}
		Once=1;
	}
	
}


//一般编队
void enemygroup_2_8(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=8;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(1);
			thelist.back()->iFireInputDelay=50;
		}
		Once=1;
	}
	
}


//一般编队
void enemygroup_2_9(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=4;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(3);
			thelist.back()->iFireInputDelay=44;
		}
		Once=1;
	}
	
}


//有1个奖励的运输直升机编队
void enemygroup_2_10(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	if(themap.yCamera<mapararea){//mapararea=7500
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=3;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%552;
			if (ptf_startpoint.x>400)
			{
				ptf_startpoint.x=200;
			}
			if (ptf_startpoint.x<=100)
			{
				ptf_startpoint.x=100;
			}
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy4_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("100|112,100|112,100|112,100|112,100|112,100|112");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(7);
			thelist.back()->SetAiState(4);
			thelist.back()->SetHP(42);
			thelist.back()->iFireInputDelay=55;
			//运输直升机携带1个奖励
			if (i==1)
			{
				enum bonus bs=power;//加enum可以适应c编译器！
				thelist.back()->SetBonus(bs);
			}
			
		}
		Once=1;
	}	
}


//一般编队
void enemygroup_2_11(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=8;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(1);
			thelist.back()->iFireInputDelay=50;
		}
		Once=1;
	}
	
}


//一般编队
void enemygroup_2_12(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=8;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(1);
			thelist.back()->iFireInputDelay=50;
		}
		Once=1;
	}
	
}

//一般编队
void enemygroup_2_13(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=8;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(1);
			thelist.back()->iFireInputDelay=50;
		}
		Once=1;
	}
	
}

//一般编队
void enemygroup_2_14(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=8;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(1);
			thelist.back()->iFireInputDelay=50;
		}
		Once=1;
	}
	
}

//一般编队
void enemygroup_2_15(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv){
	
	if(themap.yCamera<mapararea){
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=8;i++){
			thelist.push_back((cls_enemy *)new cls_enemy());
			floatpoint ptf_startpoint;
			ptf_startpoint.x=rand()%(552-118-1)+118;
			ptf_startpoint.y=rand()%480-480;
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//enemy3_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5");//Todo最好把子弹的序列和敌机的序列分开
			thelist.back()->SetAnimateData("25|29,25|29,25|29,25|29,25|29,25|29");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(4);
			thelist.back()->SetAiState(1);
			thelist.back()->iFireInputDelay=50;
		}
		Once=1;
	}
	
}

//tank编队
void enemygroup_t1(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv)
{
	if(themap.yCamera<mapararea){//mapararea=7500
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=9;i++){
			thelist.push_back((cls_enemy *)new cls_tank());
			floatpoint ptf_startpoint;		
			if (i==0)
			{
				ptf_startpoint.x=120;
				ptf_startpoint.y=-20;
			}
			else if(i==1){
				ptf_startpoint.x=140;
				ptf_startpoint.y=-58;
			}
			else if(i==2){
				ptf_startpoint.x=60;
				ptf_startpoint.y=-436;
			}
			else if(i==3){
				ptf_startpoint.x=180;
				ptf_startpoint.y=-340;
			}
			else if(i==4){
				ptf_startpoint.x=190;
				ptf_startpoint.y=-135;
			}
			else if(i==5){
				ptf_startpoint.x=220;
				ptf_startpoint.y=-77;
			}
			else if(i==6){
				ptf_startpoint.x=240;
				ptf_startpoint.y=-240;
			}
			else if(i==7){
				ptf_startpoint.x=260;
				ptf_startpoint.y=-141;
			}
			else if(i==8){
				ptf_startpoint.x=550;
				ptf_startpoint.y=-146;
			}
			else if(i==9){
				ptf_startpoint.x=450;
				ptf_startpoint.y=20;
			}
			//ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//tank_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5,6,7");
			thelist.back()->SetAnimateData("29|30,29|30,29|30,29|30,29|30,29|30,29|30,29|30");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(rand()%4);
			//thelist.back()->SetFireState(enemy_normalattack);
			thelist.back()->SetHP(7);
			//thelist.back()->use_fire_fsm=1;
			//thelist.back()->use_move_fsm=1;
			//thelist.back()->loopmove=0;
			//MOVESTATE ms;
			//ms.aimoveid=9;
			//ms.move=9;
			//ms.movetime=120;
			//thelist.back()->PushMoveState(ms);
			thelist.back()->SetAiState(99);
		}
		Once=1;
	}
	
};


//tank编队
void enemygroup_t2(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv)
{
	if(themap.yCamera<mapararea){//mapararea=7500
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=9;i++){
			thelist.push_back((cls_enemy *)new cls_tank());
			floatpoint ptf_startpoint;		
			if (i==0)
			{
				ptf_startpoint.x=120;
				ptf_startpoint.y=-330;
			}
			else if(i==1){
				ptf_startpoint.x=140;
				ptf_startpoint.y=-338;
			}
			else if(i==2){
				ptf_startpoint.x=60;
				ptf_startpoint.y=-336;
			}
			else if(i==3){
				ptf_startpoint.x=180;
				ptf_startpoint.y=-330;
			}
			else if(i==4){
				ptf_startpoint.x=190;
				ptf_startpoint.y=-335;
			}
			else if(i==5){
				ptf_startpoint.x=220;
				ptf_startpoint.y=-337;
			}
			else if(i==6){
				ptf_startpoint.x=240;
				ptf_startpoint.y=-330;
			}
			else if(i==7){
				ptf_startpoint.x=260;
				ptf_startpoint.y=-311;
			}
			else if(i==8){
				ptf_startpoint.x=550;
				ptf_startpoint.y=-406;
			}
			else if(i==9){
				ptf_startpoint.x=450;
				ptf_startpoint.y=-400;
			}
			//ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//tank_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5,6,7");
			thelist.back()->SetAnimateData("29|30,29|30,29|30,29|30,29|30,29|30,29|30,29|30");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(rand()%4);
			//thelist.back()->SetFireState(enemy_normalattack);
			thelist.back()->SetHP(7);
			//thelist.back()->use_fire_fsm=1;
			//thelist.back()->use_move_fsm=1;
			//thelist.back()->loopmove=0;
			//MOVESTATE ms;
			//ms.aimoveid=9;
			//ms.move=9;
			//ms.movetime=120;
			//thelist.back()->PushMoveState(ms);
			thelist.back()->SetAiState(99);
		}
		Once=1;
	}
	
};

//tank编队
void enemygroup_t3(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv)
{
	if(themap.yCamera<mapararea){//mapararea=7500
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=9;i++){
			thelist.push_back((cls_enemy *)new cls_tank());
			floatpoint ptf_startpoint;		
			if (i==0)
			{
				ptf_startpoint.x=120;
				ptf_startpoint.y=-20;
			}
			else if(i==1){
				ptf_startpoint.x=140;
				ptf_startpoint.y=-58;
			}
			else if(i==2){
				ptf_startpoint.x=60;
				ptf_startpoint.y=-436;
			}
			else if(i==3){
				ptf_startpoint.x=180;
				ptf_startpoint.y=-340;
			}
			else if(i==4){
				ptf_startpoint.x=190;
				ptf_startpoint.y=-135;
			}
			else if(i==5){
				ptf_startpoint.x=220;
				ptf_startpoint.y=-77;
			}
			else if(i==6){
				ptf_startpoint.x=240;
				ptf_startpoint.y=-240;
			}
			else if(i==7){
				ptf_startpoint.x=260;
				ptf_startpoint.y=-141;
			}
			else if(i==8){
				ptf_startpoint.x=550;
				ptf_startpoint.y=-146;
			}
			else if(i==9){
				ptf_startpoint.x=450;
				ptf_startpoint.y=20;
			}
			//ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//tank_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5,6,7");
			thelist.back()->SetAnimateData("29|30,29|30,29|30,29|30,29|30,29|30,29|30,29|30");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(rand()%4);
			//thelist.back()->SetFireState(enemy_normalattack);
			thelist.back()->SetHP(7);
			//thelist.back()->use_fire_fsm=1;
			//thelist.back()->use_move_fsm=1;
			//thelist.back()->loopmove=0;
			//MOVESTATE ms;
			//ms.aimoveid=9;
			//ms.move=9;
			//ms.movetime=120;
			//thelist.back()->PushMoveState(ms);
			thelist.back()->SetAiState(99);
		}
		Once=1;
	}
	
};

//tank编队
void enemygroup_t4(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv)
{
	if(themap.yCamera<mapararea){//mapararea=7500
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=9;i++){
			thelist.push_back((cls_enemy *)new cls_tank());
			floatpoint ptf_startpoint;		
			if (i==0)
			{
				ptf_startpoint.x=120;
				ptf_startpoint.y=-20;
			}
			else if(i==1){
				ptf_startpoint.x=140;
				ptf_startpoint.y=-58;
			}
			else if(i==2){
				ptf_startpoint.x=60;
				ptf_startpoint.y=-436;
			}
			else if(i==3){
				ptf_startpoint.x=180;
				ptf_startpoint.y=-340;
			}
			else if(i==4){
				ptf_startpoint.x=190;
				ptf_startpoint.y=-135;
			}
			else if(i==5){
				ptf_startpoint.x=220;
				ptf_startpoint.y=-77;
			}
			else if(i==6){
				ptf_startpoint.x=240;
				ptf_startpoint.y=-240;
			}
			else if(i==7){
				ptf_startpoint.x=260;
				ptf_startpoint.y=-141;
			}
			else if(i==8){
				ptf_startpoint.x=550;
				ptf_startpoint.y=-146;
			}
			else if(i==9){
				ptf_startpoint.x=450;
				ptf_startpoint.y=20;
			}
			//ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//tank_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5,6,7");
			thelist.back()->SetAnimateData("29|30,29|30,29|30,29|30,29|30,29|30,29|30,29|30");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(rand()%4);
			//thelist.back()->SetFireState(enemy_normalattack);
			thelist.back()->SetHP(7);
			//thelist.back()->use_fire_fsm=1;
			//thelist.back()->use_move_fsm=1;
			//thelist.back()->loopmove=0;
			//MOVESTATE ms;
			//ms.aimoveid=9;
			//ms.move=9;
			//ms.movetime=120;
			//thelist.back()->PushMoveState(ms);
			thelist.back()->SetAiState(99);
		}
		Once=1;
	}
	
};

//tank编队
void enemygroup_t5(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv)
{
	if(themap.yCamera<mapararea){//mapararea=7500
		static int Once=0;
		if (Once)
		{
			return;
		}
		srand((unsigned)time(NULL));
		for (int i=0;i<=9;i++){
			thelist.push_back((cls_enemy *)new cls_tank());
			floatpoint ptf_startpoint;		
			if (i==0)
			{
				ptf_startpoint.x=120;
				ptf_startpoint.y=-20;
			}
			else if(i==1){
				ptf_startpoint.x=340;
				ptf_startpoint.y=-58;
			}
			else if(i==2){
				ptf_startpoint.x=60;
				ptf_startpoint.y=-436;
			}
			else if(i==3){
				ptf_startpoint.x=390;
				ptf_startpoint.y=-340;
			}
			else if(i==4){
				ptf_startpoint.x=190;
				ptf_startpoint.y=-135;
			}
			else if(i==5){
				ptf_startpoint.x=220;
				ptf_startpoint.y=-77;
			}
			else if(i==6){
				ptf_startpoint.x=240;
				ptf_startpoint.y=-240;
			}
			else if(i==7){
				ptf_startpoint.x=260;
				ptf_startpoint.y=-141;
			}
			else if(i==8){
				ptf_startpoint.x=550;
				ptf_startpoint.y=-146;
			}
			else if(i==9){
				ptf_startpoint.x=450;
				ptf_startpoint.y=20;
			}
			//ptf_startpoint.y=rand()%480+rand()%22-500;//让他一开始看不到！
			thelist.back()->SetPos(ptf_startpoint);
			thelist.back()->sprite_surface_id=sfid;//tank_sfid
			thelist.back()->SetAnimateSequence("0,1,2,3,4,5,6,7");
			thelist.back()->SetAnimateData("29|30,29|30,29|30,29|30,29|30,29|30,29|30,29|30");
			thelist.back()->current_frame=(rand()%1);
			thelist.back()->SetVelocity(rand()%4);
			//thelist.back()->SetFireState(enemy_normalattack);
			thelist.back()->SetHP(12);
			//thelist.back()->use_fire_fsm=1;
			//thelist.back()->use_move_fsm=1;
			//thelist.back()->loopmove=0;
			//MOVESTATE ms;
			//ms.aimoveid=9;
			//ms.move=9;
			//ms.movetime=120;
			//thelist.back()->PushMoveState(ms);
			thelist.back()->SetAiState(99);
			thelist.back()->iFireInputDelay=44;
		}
		Once=1;
	}
	
};
#endif