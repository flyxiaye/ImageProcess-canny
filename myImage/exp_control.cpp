#include "exp_control.h"

#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "BasicFunction.h"
#include "GlobalVar.h"
#include "FillSpecialLine.h"
#include "FindLineFun.h"
#include "Judge.h"
#include "exp.h"


//延距刹车
void DelayDistantStop(void)
{
	int DistantStop = 0;		//累计编码器脉冲
	int GiveDistant = 0;		//人为设定延迟距离

	if (g_stop_dis)		//接收到延距刹车标志 开始累计行驶距离
		DistantStop += 1;
	else DistantStop = 0;

	if (DistantStop >= GiveDistant)
	{
		g_stop_line = 1;	//执行刹车
		g_stop_dis = 0;
	}
	else;
}



