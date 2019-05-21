#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "GlobalVar.h"
#include "BasicFun.h"
#include "FindLine.h"
#include "FillSpecialLine.h"
#include "FirstLineProcess.h"
unsigned char BinaryArray[188];

//================================================================//
//  @brief  :		首行边界获取
//  @param  :		void
//  @return :		void
//  @note   :		主要做两件事，准确给出起始左右边界，确定高光阈值
//================================================================//
void GetFirstLineEage(void)
{
	//首行二值化处理
	unsigned char *p_image = ImageData[DOWN_EAGE];
	unsigned char *p_binary = BinaryArray;
	int Threshold = otsuThreshold(p_image, 188, 1);
	//Threshold = MIN(Threshold, 55);
	LightThreshold = MIN(Threshold + 30, 70);
	LightThreshold = MAX(LightThreshold, 50);
	string.Format("\r\n Threshold = %d \r\n", Threshold); PrintDebug(string);
	for (int i = 188; i > 0; i--)
	{
		if (*p_image++ >= Threshold)
			*p_binary++ = 255;
		else *p_binary++  = 0;
	}

	FindFirstEage();
	LL[DOWN_EAGE] = GetLL(DOWN_EAGE, LL[DOWN_EAGE]);
	RL[DOWN_EAGE] = GetRL(DOWN_EAGE, RL[DOWN_EAGE]);
	//for (int i = 0; i < 188; i++)
	//{
	//	ImageData[DOWN_EAGE][i] = BinaryArray[i];
	//}
	//ImageData[DOWN_EAGE][LL[DOWN_EAGE]] = 254;
	//ImageData[DOWN_EAGE][RL[DOWN_EAGE]] = 254;
	string.Format("\r\n LL = %d \r\n", LL[DOWN_EAGE]); PrintDebug(string);
	string.Format("\r\n RL = %d \r\n", RL[DOWN_EAGE]); PrintDebug(string);
}
//================================================================//
//  @brief  :		大津法二值化
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
char otsuThreshold(unsigned char *image, int col, int row)
{
#define GrayScale 256
	int width = col;
	int height = row;
	int pixelCount[GrayScale] = { 0 };
	float pixelPro[GrayScale] = { 0 };
	int i, j, pixelSum = width * height;
	char threshold = 0;
	unsigned char* data = image;  //指向像素数据的指针
	for (i = 0; i < GrayScale; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	//统计灰度级中每个像素在整幅图像中的个数  
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
		}
	}

	//计算每个像素在整幅图像中的比例  
	float maxPro = 0.0;
	for (i = 0; i < GrayScale; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;
		if (pixelPro[i] > maxPro)
		{
			maxPro = pixelPro[i];
		}
	}

	//遍历灰度级[0,255]  
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < GrayScale; i++)     // i作为阈值
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for (j = 0; j < GrayScale; j++)
		{
			if (j <= i)   //背景部分  
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else   //前景部分  
			{
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		u = u0tmp + u1tmp;
		deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}

	return threshold;
}

//================================================================//
//  @brief  :		首行搜线
//  @param  :		void
//  @return :		void
//  @note   :		void
//================================================================//
void FindFirstEage(void)
{
	int MaxDistance = 0;
	int LeftFindFlag = 0, RightFindFlag = 0;
	int TmpLeft, TmpRight;
	for (int i = 0; i < 188; i++)
	{
		if (1 == IsFirstEage(i))
		{
			TmpLeft = i;
			LeftFindFlag = 1;
		}
		if (LeftFindFlag)
		{
			if (2 == IsFirstEage(i))
			{
				TmpRight = i;
				RightFindFlag = 1;
			}
		}
		if (LeftFindFlag && RightFindFlag)
		{
			LeftFindFlag = RightFindFlag = 0;
			if (TmpRight - TmpLeft >= MaxDistance)
			{
				MaxDistance = TmpRight - TmpLeft;
				LL[DOWN_EAGE] = TmpLeft;
				RL[DOWN_EAGE] = TmpRight;
			}
		}
	}
}
//================================================================//
//  @brief  :		边界判断
//  @param  :		void
//  @return :		1 left eage  2 right eage
//  @note   :		void
//================================================================//
int IsFirstEage(int col)
{
	
	if (BinaryArray[col] == 255)
	{
		if (0 == col)
		{
			if (GetBinary(BinaryArray, col + 1) == 255
				&& GetBinary(BinaryArray, col + 2) == 255)
				return 1;
			else return 0;
		}
		else if (1 == col)
		{
			if (GetBinary(BinaryArray, col + 1) == 255
				&& GetBinary(BinaryArray, col + 2) == 255
				&& GetBinary(BinaryArray, col - 1) == 0)
				return 1;
			else return 0;
		}
		else if (186 == col)
		{
			if (GetBinary(BinaryArray, col + 1) == 0
				&& GetBinary(BinaryArray, col - 1) == 255
				&& GetBinary(BinaryArray, col - 2) == 255)
				return 2;
			else return 0;
		}
		else if (187 == col)
		{
			if (GetBinary(BinaryArray, col - 1) == 255
				&& GetBinary(BinaryArray, col - 2) == 255)
				return 2;
			else return 0;
		}
		else
		{
			if (GetBinary(BinaryArray, col + 1) == 255
				&& GetBinary(BinaryArray, col + 2) == 255
				&& GetBinary(BinaryArray, col - 1) == 0
				/*&& GetBinary(BinaryArray, col - 2) == 0*/)
				return 1;
			else if (GetBinary(BinaryArray, col + 1) == 0
				/*&& GetBinary(BinaryArray, col + 2) == 0*/
				&& GetBinary(BinaryArray, col - 1) == 255
				&& GetBinary(BinaryArray, col - 2) == 255)
				return 2;
			else return 0;
		}
	}
	else return 0;
}

//================================================================//
//  @brief  :		获取数组值
//  @param  :		void
//  @return :		1 left eage  2 right eage
//  @note   :		void
//================================================================//
unsigned char GetBinary(unsigned char *FirstLine, int col)
{
	if (col < 0)col = 0;
	if (col > 187)col = 187;
	return *(FirstLine + col);
}

