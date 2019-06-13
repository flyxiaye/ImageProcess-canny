#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "GlobalVar.h"
#include "BasicFun.h"
#include "FindLine.h"
#include "FillSpecialLine.h"
#include "Exp.h"

//Ê×ÐÐËÑ±ß
void GetFirstL(void)
{
	int mid = 94;
	FindLineType = 1;
	int LLF = SearchLeftEage(DOWN_EAGE, mid);
	int RLF = SearchRightEage(DOWN_EAGE, mid);
	string.Format("\r\n LL = %d \r\n", LLF); PrintDebug(string);
	string.Format("\r\n RL = %d \r\n", RLF); PrintDebug(string);
	FindLineType = 0;
	for (int i = 0; i < IMG_ROW; i++)
	{
		ImageData[i][LLF] = 100;
		ImageData[i][RLF] = 128;
	}
}

void Exp(void)
{
	//GetFirstL();
	VarInit();
	FirstLineV4();
	//FindLineNormalNew(0);

	string.Format("\r\n LL = %d \r\n", LL[DOWN_EAGE]); PrintDebug(string);
	string.Format("\r\n RL = %d \r\n", RL[DOWN_EAGE]); PrintDebug(string);
	for (int i = 0; i < IMG_ROW; i++)
	{
		ImageData[i][LL[70]] = 254;
		ImageData[i][RL[70]] = 254;
	}
	string.Format("\r\n RL = %d \r\n", ImageEage[DOWN_EAGE][165]); PrintDebug(string);
	string.Format("\r\n RL = %d \r\n", ImageEage[DOWN_EAGE][166]); PrintDebug(string);
	string.Format("\r\n RL = %d \r\n", ImageEage[DOWN_EAGE][167]); PrintDebug(string);
}

unsigned char IndJudgeCircle(unsigned char type)
{
	return 1;
}