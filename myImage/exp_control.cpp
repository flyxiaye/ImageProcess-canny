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


//�Ӿ�ɲ��
void DelayDistantStop(void)
{
	int DistantStop = 0;		//�ۼƱ���������
	int GiveDistant = 0;		//��Ϊ�趨�ӳپ���

	if (g_stop_dis)		//���յ��Ӿ�ɲ����־ ��ʼ�ۼ���ʻ����
		DistantStop += 1;
	else DistantStop = 0;

	if (DistantStop >= GiveDistant)
	{
		g_stop_line = 1;	//ִ��ɲ��
		g_stop_dis = 0;
	}
	else;
}



