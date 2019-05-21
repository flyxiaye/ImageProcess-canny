#ifndef GlobalVar_H
#define GlobalVar_H

#endif // !GlobalVar_H
#pragma once
#define FIND_LEFT 0
#define FIND_RIGHT 1
#define MaxValue 12								//寻线阈值
#define MaxValueCol 25
#define MaxValue1 60
#define MAX(x, y) x > y ? x : y
#define MIN(x, y) x < y ? x : y
#define FRONT_LINE 0
#define LEFT_EAGE 0
#define RIGHT_EAGE 187
//#define IMG_ROWS 128
//#define IMG_COLS 320

#define WHITE 0
#define BLACK 1

typedef struct
{
	int ErrType; //0普通情况 1内跳 2外跳 3拐点 4长直（补图到上边界）5左边界 6右边界 7首行丢边 8四类十字 9结束循环 10环岛
	int ErrRow;
	int ErrCol;
}SpecialPoint;

typedef struct
{
	float k;
	float b;
}Line;

typedef struct
{
	int x1;
	int x2;
}Vector;

extern SpecialPoint LeftPnt, RightPnt, LeftPntTre, RightPntTre;

extern const unsigned char Bit[];
//1断路标志位		2左右标志位
//unsigned char ImgBuff[IMG_COLS * IMG_ROWS];
extern unsigned char FirstRoadType;					//标记赛道类型 1 不确定赛道 2 直道细分 3 单边丢线赛道（丢边弯 斜十字） 4 正处于十字赛道
extern int LeftLost, RightLost;                       //标记为跳变前的一行 有数据
extern int FindLine;						//标记当前行
extern int LL[IMG_ROWS];								//保存点的列数
extern int RL[IMG_ROWS];
extern int ML[IMG_ROWS];
extern const int Mid;
extern int LeftFindFlag, RightFindFlag;
extern int g_trend_flag;			//1：向右趋势 -1：向左趋势
extern int row_n;
extern int ThreeFlag;				//1为左 2为右

extern int FillFlag;						//补图失败标志
extern int ErrorFlag;						//异常情况标记
extern int BevelCrossFlag;					//斜十字标志
extern int LevelCrossFlag;					//平十字标志
extern int StopFillFlag;					//停车标志

//===================================环岛======================//
extern int CircleFlag;			//0不是环岛 1左环岛 2右环岛
extern int CircleState;

void VarInit(void);

//globalvar
extern int g_stop_line;
extern int g_stop_dis;
extern int MaxSpeedFlag;