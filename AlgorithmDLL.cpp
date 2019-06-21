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

//˵�����û�ֻ��Ҫ�޸ģ�ȫ�ֱ����� �� �Զ�������㷨�� �Ϳ��Խ����㷨�������Ŀ���
//�������������޸ġ�
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//=============================ȫ�ֱ�����==================================//
#define WHITE 0
#define BLACK 1
//��ӵ���λ������ʾ���㷨���ƣ�ֻ���޸��������ݼ���
void _stdcall AlgorithmName(char* pName)
{
	char* p = "��ͼcanny";
	strcpy(pName, p);
}
// ��ʼ��ͼ������
// �öδ��벻�����κ��޸�,����Ϊ����λ��ȡ������䵽ImageData
// ����ͬ��λ���Ĳɼ�����һ����Ч��.
void InitImgData(void* pData, int w, int h)
{
	int i = 0, j = 0;
	// ���õ�ͼ���С����λ����ͼ���С��һ�£������������������޸ĺ궨���IMG_ROWS��IMG_COLS��Ҫ����λ��ͼ���Сһ��
	if (w != IMG_COLS || h != IMG_ROWS)
	{
		AfxMessageBox("ͼ���С���ò�����");
	}
	for (i = 0; i < IMG_ROWS; i++)
	{
		for (j = 0; j < IMG_COLS; j++)
		{
			ImageData[i][j] = *((UCHAR*)pData + i * IMG_COLS + j);
		}
	}
}
// �öδ��벻�����κ��޸�
// ����Ϊ����ִ���㷨�����Ѵ�����ͼ�������͵���λ����ʾ
// ��λ������Ҫ��ʾ������pDataָ��Ϊ�׵�ַ����������
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


//================================================�Զ�������㷨����ʼ===========================================//



//=================================================================
//���ܣ�ͼ��������㷨����
//������wͼ�����ͼ���������------>��λ������ʱ�ṩͼ��Ŀ��
//      hͼ��ߣ���ͼ���������------>��λ������ʱ�ṩͼ��ĸ߶�
//      pData ��λ�����øú���ʱ�ṩ��ʾͼ�񻺳����Ŀ�ʼָ��
//================================================================
void _stdcall HandleImg(void* pData, ULONG w, ULONG h)
{
	InitImgData(pData, w, h);
	string.Format("\r\n 128 1\r\n"); PrintDebug(string);
	//============================�û���Ҫ��ӵ��㷨������뿪ʼ=============================//
	QueryPerformanceFrequency(&f);//��ȡ�ڲ��߾��ȼ�������Ƶ��

	dFreq = (double)f.QuadPart; //��ȡ��������Ƶ��

	
	QueryPerformanceCounter(&start);//��ȡ�ڲ��߾��ȼ�������ǰ�ļ���ֵ
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
	

	//ʱ��� = ����ֵ��/Ƶ�ʣ���λs��
	double duration = (double)(end.QuadPart - start.QuadPart) / dFreq;
	//for (int i = 0; i < IMG_ROWS; i++) {
	//	for (int j = 0; j < IMG_COLS; j++) {

	//		ImageData[i][j] = ImageData[i][j] > 50 ? 255 : 0;

	//	}
	//}

	////��ֵ��
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
	//ͼ��ԭ��128 * 40 ---->  128 * 320
	/*for (int i = 0; i < IMG_ROWS; i++) {
		for (int j = 0; j < IMG_COLS / 8; j++) {
			for (int k = 0; k < 8; k++) {
				//ImageData[i][j * 8 + k] = Get_Color(i, j * 8 + k) == 0 ? 255 : 0;
			}
		}
	}*/

	//str.Format("\r\n 1\r\n"); PrintDebug(str);

	//============================�û���Ҫ�ķ����㷨�������================================//
	UpdateImgData(pData);
	fclose(pfile);
}

//================================================�Զ�������㷨������===========================================//
