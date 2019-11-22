#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "GlobalVar.h"
#include "BasicFun.h"
#include "canny.h"
#include "binary.h"
#include "FirstLineProcess.h"

////================================================================//
////  @brief  :		�������򽵵�����
////  @param  :		*image          ͼ�������ַ
////  @param  :		start_row             ��ʼ��
////  @param  :		start_col             ��ʼ��
////  @param  :		height             ����
////  @param  :		width             ����
////  @param  :		diff_light             ���Ȳ�ֵ
////  @return :		void
////  @note   :		void
////================================================================//
//void lowLight(unsigned char* srcImage, int start_row, int start_col, int height, int width, int diff_light)
//{
//	for (int i = start_row; i <= start_row + height; i++)
//	{
//		for (int j = start_col; j <= start_col + width; j++)
//		{
//			srcImage[i * IMG_COL + j] -= diff_light;
//			srcImage[i * IMG_COL + j] = MAX(0, srcImage[i * IMG_COL + j]);
//
//		}
//	}
//}
//
////================================================================//
////  @brief  :		�Ҷ�ͼ��ֵ��
////  @param  :		*image          ͼ�������ַ
////  @param  :		col             ����
////  @param  :		row             ����
////  @return :		void
////  @note   :		void
////================================================================//
//void gray2Binary(unsigned char* srcImage, unsigned char* dstImage)
//{
//	unsigned char th = otsuThreshold(&srcImage[20 * 188], 188, 51);
//	//unsigned char th = 60;
//	lowLight(srcImage, 20, 40, 30, 100, 10);
//	unsigned char* sp = srcImage, * dp = dstImage;
//	//  for(int i = 0; i < 20 * 188; i++, sp++,dp++)
//	//    *dp = 0;
//	for (int i = 0; i < 188 * 120; i++, sp++, dp++)
//	{
//		if (*sp < th)* dp = 0;
//		else *dp = 255;
//	}
//	//  for (int i = 71*188; i < 188 * 120; i++, sp++, dp++)
//	//    *dp=0;
//}

//================================================================//
//  @brief  :		��򷨶�ֵ��
//  @param  :		*image          ͼ�������ַ
//  @param  :		col             ����
//  @param  :		row             ����
//  @return :		void
//  @note   :		void
//================================================================//
//unsigned char otsuThreshold(unsigned char* image, int col, int row)
//{
//#define GrayScale 256
//	int width = col;
//	int height = row;
//	int pixelCount[GrayScale] = { 0 };
//	float pixelPro[GrayScale] = { 0 };
//	int i, j, pixelSum = width * height;
//	unsigned char threshold = 0;
//	unsigned char* data = image;  //ָ���������ݵ�ָ��
//	for (i = 0; i < GrayScale; i++)
//	{
//		pixelCount[i] = 0;
//		pixelPro[i] = 0;
//	}
//
//	//ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���  
//	for (i = 0; i < height; i++)
//	{
//		for (j = 0; j < width; j++)
//		{
//			pixelCount[(int)data[i * width + j]]++;  //������ֵ��Ϊ����������±�
//		}
//	}
//
//	//����ÿ������������ͼ���еı���  
//	float maxPro = 0.0;
//	for (i = 0; i < GrayScale; i++)
//	{
//		pixelPro[i] = (float)pixelCount[i] / pixelSum;
//		if (pixelPro[i] > maxPro)
//		{
//			maxPro = pixelPro[i];
//		}
//	}
//
//	//�����Ҷȼ�[0,255]  
//	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
//	for (i = 0; i < GrayScale; i++)     // i��Ϊ��ֵ
//	{
//		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
//		for (j = 0; j < GrayScale; j++)
//		{
//			if (j <= i)   //��������  
//			{
//				w0 += pixelPro[j];
//				u0tmp += j * pixelPro[j];
//			}
//			else   //ǰ������  
//			{
//				w1 += pixelPro[j];
//				u1tmp += j * pixelPro[j];
//			}
//		}
//		u0 = u0tmp / w0;
//		u1 = u1tmp / w1;
//		u = u0tmp + u1tmp;
//		deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
//		if (deltaTmp > deltaMax)
//		{
//			deltaMax = deltaTmp;
//			threshold = i;
//		}
//	}
//
//	return threshold;
//}