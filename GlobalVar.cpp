#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "GlobalVar.h"

const int MidOffset[] = {
	93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93,
};
unsigned char ImageEage[IMG_ROW][IMG_COL];
int ML[IMG_ROW], LL[IMG_ROW], RL[IMG_ROW];   //保存边缘线信息数组
int ML_Count;									//中线有效行
SpecialPoint LeftPnt, RightPnt;					//保存左右特殊点信息
int DiffThreshold = 25;							//边缘检测阈值
int DarkThreshold = 85;                         //亮暗阈值（路障）
int BrightThreshold = 15;                       //亮亮阈值（坡道）
int LightThreshold = 80;						//去高光噪声阈值
int LightThreshold2 = 150;						//去高光噪声阈值2
int FindLineType = 0;							//是否加入高光滤波标志
int g_RoadType = 0;
int ErrorFlag = 0;								//错误标志位
//1双侧十字错误  2误判为斜十字  3视野丢失错误  4中线错误  5双侧十字噪点错误  6平十字噪点错误
int LastMiddleLine = 0;

int SpeedRow = 0;			//控速边界行数

int CircleFlag = 1;
int CircleState = 2;

int LeftIntLine = 0;		//记录左内跳行数
int RightIntLine = 0;		//记录右内跳行数

int Img_SpecialElemFlag = 0;    //特殊元素标志
int Img_BrokenFlag = 0;			//断路标志
int Img_BlockFlag = 0;			//路障标志
int Img_StopLineFlag = 0;		//停车线标志
int Img_RampFlag = 0;			//坡道标志

int BrokenLastAve = 0;
int StopLineDist = 0;

unsigned char Img_CircleOpen = 1;
unsigned char Img_BrokenOpen = 1;
unsigned char Img_StopOpen = 1;
unsigned char Img_BlockOpen = 1;
unsigned char Img_RampOpen = 1;
void VarInit(void)
{
	ML_Count = 0;
	ErrorFlag = 0;
	g_RoadType = 0;
	LeftIntLine = 0;
	RightIntLine = 0;
	//下面某些地方起了关键作用
	LeftPnt.ErrCol = RightPnt.ErrCol = 0;
	LeftPnt.ErrRow = RightPnt.ErrRow = DOWN_EAGE;
	for (int i = 0; i < IMG_ROW; i++)
	{
		LL[i] = LEFT_EAGE;
		RL[i] = RIGHT_EAGE;
		ML[i] = MIDDLE;
	}

}
Point PointE;
Point PointF;


//计时
LARGE_INTEGER f;
double dFreq;
LARGE_INTEGER start, end, end2;
