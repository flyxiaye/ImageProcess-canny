// AlgorithmDLL.cpp : Defines the initialization routines for the DLL.
//
#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "GlobalVar.h"
#include "BasicFun.h"
#include "Exp.h"
#include "FillSpecialLine.h"
#include "MainProcess.h"
#include "FirstLineProcess.h"
#include "FindLine.h"
#include "canny.h"

//说明：用户只需要修改：全局变量区 和 自定义仿真算法区 就可以进行算法仿真插件的开发
//其它内容请勿修改。
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//=============================全局变量区==================================//
#define WHITE 0
#define BLACK 1
//添加到上位机后显示的算法名称，只需修改名称内容即可
void _stdcall AlgorithmName(char* pName)
{
	char* p = "补图canny";
	strcpy(pName, p);
}
// 初始化图像数据
// 该段代码不用做任何修改,作用为从上位机取数据填充到ImageData
// 就如同下位机的采集程序一样的效果.
void InitImgData(void* pData, int w, int h)
{
	int i = 0, j = 0;
	// 配置的图像大小与上位机的图像大小不一致，将发生错误。请自行修改宏定义的IMG_ROWS和IMG_COLS，要和上位机图像大小一致
	if (w != IMG_COLS || h != IMG_ROWS)
	{
		AfxMessageBox("图像大小设置不合理");
	}
	for (i = 0; i < IMG_ROWS; i++)
	{
		for (j = 0; j < IMG_COLS; j++)
		{
			ImageData[i][j] = *((UCHAR*)pData + i * IMG_COLS + j);
		}
	}
}
// 该段代码不用做任何修改
// 作用为：把执行算法处理后把处理后的图像数据送到上位机显示
// 上位机最终要显示的是以pData指针为首地址的数组数据
void UpdateImgData(void* pData)
{
	int i = 0, j = 0;
	for (i = 0; i < IMG_ROWS; i++)
	{
		for (j = 0; j < IMG_COLS; j++)
		{
			*((UCHAR*)pData + i * IMG_COLS + j) = ImageData[i][j];
		}
	}
}
void PrintDebug(CString ss)
{
	fprintf(pfile, ss);
}


//================================================自定义仿真算法区开始===========================================//



//=================================================================
//功能：图像处理仿真算法函数
//参数：w图像宽，即图像的总列数------>上位机调用时提供图像的宽度
//      h图像高，即图像的总行数------>上位机调用时提供图像的高度
//      pData 上位机调用该函数时提供显示图像缓冲区的开始指针
//================================================================
void _stdcall HandleImg(void* pData, ULONG w, ULONG h)
{
	InitImgData(pData, w, h);
	string.Format("\r\n 128 1\r\n"); PrintDebug(string);
	//============================用户需要添加的算法仿真代码开始=============================//
	QueryPerformanceFrequency(&f);//获取内部高精度计数器的频率

	dFreq = (double)f.QuadPart; //获取计数器的频率

	
	QueryPerformanceCounter(&start);//获取内部高精度计数器当前的计数值
	GetML();
	for (int i = 0; i < 120; i++)
	{
		for (int j = 0; j < 188; j++)
		{
			if (ImageEage[i][j] == HIGH_TH) ImageData[i][j] = 255;
			else ImageData[i][j] = 0;
		}
	}
	for (int i = 0; i < 120; i++)
	{
		ImageData[i][LL[i]] = 100;
		ImageData[i][ML[i]] = 254;
		ImageData[i][RL[i]] = 128;
	}
	for (int i = 0; i < 30; i++)
		ImageData[LeftIntLine][i] = 100;
	for (int i = 187; i > 158; i--)
		ImageData[RightIntLine][i] = 128;
	QueryPerformanceCounter(&end);

	//long a = 300000;
	//while (a--);
	

	//时间差 = 计数值差/频率（单位s）
	double duration = (double)(end.QuadPart - start.QuadPart) / dFreq;
	//for (int i = 0; i < IMG_ROWS; i++) {
	//	for (int j = 0; j < IMG_COLS; j++) {

	//		ImageData[i][j] = ImageData[i][j] > 50 ? 255 : 0;

	//	}
	//}

	////二值化
	//for (int i = 0; i < 60; i++) {
	//	for (int j = 0; j < 24; j++) {
	//		ImgBuff[i * 24 + j] = 0;
	//		for (int k = 0; k < 8; k++) {
	//			ImgBuff[i * 24 + j] += (ImageData[i+25][j * 8 + k] > 0) ? WHITE : BLACK;
	//			if (k != 7)
	//				ImgBuff[i * 24 + j] = ImgBuff[i * 24 + j] << 1;
	//		}
	//	}
	//}
	//FindAllEage();
	//FindLLRLEage();
	//int a = AveGray();
	//string.Format("\r\n gray = %d \r\n", a); PrintDebug(string);
	
	ImageData[PointE.Row][PointE.Col] = 128;
	ImageData[PointF.Row][PointF.Col] = 100;
	string.Format("\r\n time = %lfs \r\n", duration); PrintDebug(string);
	string.Format("\r\n freq = %lfs \r\n", dFreq); PrintDebug(string);
	//CannyEdgeTest(&ImageData[0], 20);
	//CannyEage();
	//ShowEage(ImageEage[0]);
	//FindAllEageHight(0);

	//int RoadType = FirstRowProcess();

	//for (int i = 0; i < 188; ++i)
	//{
	//	ImageData[DOWN_EAGE + 1][i] = 100;
	//	ImageData[UP_EAGE][i] = 128;
	//}

	/*str.Format("\r\n ErrorFlag = %d \r\n", ErrorFlag); PrintDebug(str);
	str.Format("\r\n speedflag = %d \r\n", MaxSpeedFlag); PrintDebug(str);*/



	//====================================================================================//
	//图像还原：128 * 40 ---->  128 * 320
	/*for (int i = 0; i < IMG_ROWS; i++) {
		for (int j = 0; j < IMG_COLS / 8; j++) {
			for (int k = 0; k < 8; k++) {
				//ImageData[i][j * 8 + k] = Get_Color(i, j * 8 + k) == 0 ? 255 : 0;
			}
		}
	}*/

	//str.Format("\r\n 1\r\n"); PrintDebug(str);

	//============================用户需要的仿真算法代码结束================================//
	UpdateImgData(pData);
	fclose(pfile);
}

//================================================自定义仿真算法区结束===========================================//
