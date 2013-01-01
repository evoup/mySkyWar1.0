#ifndef INCLUDE_FRESHENEMY_H
#define INCLUDE_FRESHENEMY_H
///
#if 1
#include <windows.h>   // include all the windows headers
#include <windowsx.h>  // include useful macros
#include <stdio.h>
#include <math.h>
#include <ddraw.h>
#include <io.h>//_lseek 要包含的
#include <iostream>//除非没有H，是无法用std的！
using namespace std;
#include "winGMP.h"//windows游戏编程大师上的一些低级操作函数（如解锁加锁表面）
#include "myengine.h"//DX和精灵的结构定义
#include "function.h"
//#include "map2.h"
#include "freshenemy.h"
#include "class_enemy.h"
#include <list.h>
#include <time.h>
#include "class_tank.h"
/*typedef struct MAPDATA_type
{
    int xMax, yMax;              // map size in tiles
    int xCamera, yCamera;        // camera location in pixels
    int xMaxCamera, yMaxCamera;  // max camera coordinates
    //int nNPCCount;               // number of NPCs
    //int nScriptCount;            // number of linked scripts
    //LPNPC lpnpc;                 // linked NPCs
    //LPSCRIPT lpscr;              // linked scripts
} MAPDATA, FAR* LPMAPDATA;*/
#endif
void enemygroup_1_1(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_1_2(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_1_2t(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_1_3t(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_1_3(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_1_4(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_1_5(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_1_6(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_1_7(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_1_8(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_1_9(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_1_10(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);


void enemygroup_2_1(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_2_2(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_2_3(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_2_4(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_2_4b(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_2_4c(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_2_5(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_2_6(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_2_7(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_2_8(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_2_9(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2,int lv=0);
void enemygroup_2_10(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_2_11(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_2_12(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_2_13(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_2_14(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_2_15(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_t1(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_t2(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_t3(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_t4(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);
void enemygroup_t5(MAPDATA & themap,int mapararea,list<cls_enemy *> & thelist,int sfid,int sfid2=0,int lv=0);

///
#endif
