#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "BasicFunction.h"
#include "GlobalVar.h"
#include "FillSpecialLine.h"
#include "FindLineFun.h"


SpecialPoint LeftPnt, RightPnt, LeftPntTre, RightPntTre;

const unsigned char Bit[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
//0环岛标志位	1断路标志位		2左右标志位
//unsigned char ImgBuff[IMG_COLS * IMG_ROWS];

int LeftLost = -1, RightLost = -1;                       //标记为跳变前的一行 有数据

int LL[IMG_ROWS] = { 0 };								//保存点的列数
int RL[IMG_ROWS] = { 0 };
int ML[IMG_ROWS] = { 0 };
const int Mid = (LEFT_EAGE + RIGHT_EAGE) >> 1;



//标志位
unsigned char FirstRoadType;			//标记赛道类型 1 不确定赛道 2 直道细分 3 单边丢线赛道（丢边弯 斜十字） 4 正处于十字赛道
int FindLine = 0;						//标记当前行
int LeftFindFlag = 0, RightFindFlag = 0;  //左右寻线情况标记
int ThreeFlag = 0;						//3类赛道标志 1为左 2为右

int g_trend_flag = 0;				//1：向右趋势 -1：向左趋势
int row_n = 0;

int FillFlag = 0;
int ErrorFlag = 0;						//异常情况标记
//int CircleFlag = 0;				//环岛标志
int CutFlag = 0;				//断路标志
int BevelCrossFlag = 0;			//斜十字标志 tips:补斜十字函数只使用一次
int LevelCrossFlag = 0;			//平十字标志
int StopFillFlag = 0;			//刹车补线标志

//===================================环岛======================//
int CircleFlag = 0;
int CircleState = 0;			//	4环岛口 5环岛内 6出环岛口

void VarInit(void)
{
	//全局变量初始化
	for (int i = IMG_ROWS - 1; i >= 0; --i)
	{
		LL[i] = LEFT_EAGE;
		RL[i] = RIGHT_EAGE;
		ML[i] = Mid;
	}
	IsCross3(0, 0);
	FindLine = 0;
	FirstRoadType = 0;
	LeftLost = RightLost = -1;
	LeftFindFlag = RightFindFlag = 0;
	g_trend_flag = 0;
	FillFlag = 0;
	BevelCrossFlag = 0;
	LevelCrossFlag = 0;
	LeftPnt.ErrType = 0;
	LeftPnt.ErrRow = 0;
	LeftPnt.ErrCol = LEFT_EAGE;
	RightPnt.ErrType = 0;
	RightPnt.ErrRow = 0;
	RightPnt.ErrCol = RIGHT_EAGE;
	StopFillFlag = 0;
	ErrorFlag = 0;
}

//globalvar
int g_stop_line = 0;		//停车标志
int g_stop_dis = 0;			//延距停车标志
int MaxSpeedFlag = 0;     //高速标志
