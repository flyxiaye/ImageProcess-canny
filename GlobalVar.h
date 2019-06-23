#ifndef GlobalVar_H
#define GlobalVar_H
#include "windows.h"
#include "Exp.h"
#define LEFT_EAGE 0			//图像左边界
#define RIGHT_EAGE 187		//图像右边界
#define MIDDLE 94			//图像中值
#define UP_EAGE 25			//图像上边界
#define DOWN_EAGE 70		//图像下边界
#define IMG_ROW 120			//图像行数
#define IMG_COL 188			//图像列数
//#define FIND_LEFT 0
//#define FIND_RIGHT 1

#define MAX(x, y) ((x) > (y)) ? (x) : (y)
#define MIN(x, y) ((x) < (y)) ? (x) : (y)

#define FINDLINE_TH 7		//搜线阈值
#define IN_OFFSET 2 //向内偏移
//#define OUT_OFFSET 6 //向外偏移
#define CIRCLEUP_TH 15		//进入环岛差距行

#define CIRCLE 2			//环岛开关
#define BROKEN 1			//断路开关
#define CURVE_BROKEN 1
#define RAMP 1				//坡道开关
#define BLOCK_BROKEN 1		//路障断路开关
#define STOPLINE 1			//停车开关
#define CI_IND 0			//出环岛标志
#define INF 0				//红外识别

typedef struct
{
	int Row;
	int Col;
}Point;

typedef struct
{
	int Type;		//0普通情况 1内跳 2外跳 3拐点 4上边界 5左边界 6右边界 7首行丢边 8：4类十字补线 9结束循环
	int ErrRow;
	int ErrCol;
}SpecialPoint;

enum CircleType
{
	CN, CL, CR
};

enum EageType
{
	NO, HIGH_TH, LOW_TH
};

extern const int MidOffset[];
extern int ML_Count;									//中线有效行
extern SpecialPoint LeftPnt, RightPnt;					//保存左右特殊点信息
extern int g_RoadType;
extern int ErrorFlag;								//错误标志位
extern int LastMiddleLine;							//保存上帧中线
extern int DiffThreshold;							//边缘检测阈值
extern int LightThreshold;						//去高光噪声阈值
extern int LightThreshold2;						//去高光噪声阈值2
extern int FindLineType;							//是否加入高光滤波标志

extern int SpeedRow;			//控速边界行数

extern int CircleFlag;
extern int CircleState;

extern int LeftIntLine;		//记录左内跳行数
extern int RightIntLine;		//记录右内跳行数
extern int Img_SpecialElemFlag;    //特殊元素标志
extern int Img_BrokenFlag;			//断路标志 1直道断路  3弯道断路
extern int Img_BlockFlag;			//路障标志
extern int Img_StopLineFlag;		//停车线标志
extern int Img_RampFlag;			//坡道标志

extern int BrokenLastAve;
extern int StopLineDist;
		
extern unsigned char Img_CircleOpen;
extern unsigned char Img_BrokenOpen;
extern unsigned char Img_StopOpen;
extern unsigned char Img_BlockOpen;
extern unsigned char Img_RampOpen;

extern int ML[IMG_ROW], LL[IMG_ROW], RL[IMG_ROW];
extern unsigned char ImageEage[IMG_ROW][IMG_COL];
void VarInit(void);

extern Point PointE;
extern Point PointF;

//计时
extern LARGE_INTEGER f;
extern double dFreq;
extern LARGE_INTEGER start, end, end2;
#endif // !GlobalVar_H
#pragma once
