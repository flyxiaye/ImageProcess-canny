#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "GlobalVar.h"
#include "canny.h"



#define ROW 51
#define COL 188
int HighThreshold = 80;
int LowThreshold = HighThreshold * 0.4;




//边缘检测算子
int GetEageThreshold(int row, int col)
{
	col = MAX(col, LEFT_EAGE + 1);
	unsigned char* p = ImageData[0] + (row - 1) * IMG_COL + col - 1;
	signed int sum_1 = 0, sum_2 = 0;
	sum_1 += *p++; sum_2 += *p++;
	p += (IMG_COL - 2);
	sum_2 -= *p++; sum_1 -= *p++;
	if (sum_1 < 0)sum_1 = -sum_1;
	if (sum_2 < 0)sum_2 = -sum_2;
	sum_1 += sum_2;
	if (sum_1 > HighThreshold)return HIGH_TH;
	else if (sum_1 > LowThreshold)return LOW_TH;
	else return NO;
}



//双阈值检测
void DoubleTh(unsigned char* source)
{
	for (int i = UP_EAGE; i <= DOWN_EAGE; i++)
	{
		unsigned char* p = source + i * IMG_COL;
		for (int j = 0; j < IMG_COL; j++)
		{
			if (*p > HighThreshold)
				* p = HIGH_TH;
			else if (*p > LowThreshold)
				* p = LOW_TH;
			else *p = NO;
			p++;
		}
	}
}

//判断边缘连接
unsigned char IsStrongEage(unsigned char* p)
{
	p -= (1 + IMG_COL);
	unsigned char ack = 0;
	ack += HIGH_TH == *p++;
	ack += HIGH_TH == *p++;
	ack += HIGH_TH == *p++;
	p += (IMG_COL - 3);
	ack += HIGH_TH == *p++;
	ack += HIGH_TH == *p++;
	ack += HIGH_TH == *p++;
	p += (IMG_COL - 3);
	ack += HIGH_TH == *p++;
	ack += HIGH_TH == *p++;
	ack += HIGH_TH == *p++;
	return ack;
}

//抑制孤立弱边缘
void PressEage(unsigned char* source)
{
	for (int i = UP_EAGE; i <= DOWN_EAGE; i++)
	{
		unsigned char* p = (source + IMG_COL * i + 1);
		for (int j = 1; j < IMG_COL - 1; j++)
		{
			if (LOW_TH == *p)
			{
				if (IsStrongEage(p))
					* p = HIGH_TH;
			}
			p++;
		}
	}
}



////显示边界
//void ShowEage(unsigned char *p)
//{
//	unsigned char *p_image = ImageData[0];
//	for (int i = 0; i < IMG_ROW*IMG_COL; i++)
//	{
//		if (HIGH_TH == *p++)
//			*p_image = 254;
//		*p_image++;
//	}
//}

//统计某一行的灰度平均值
int GetAveGray(int row)
{
	unsigned char* p = ImageData[row];
	int sum = 0;
	for (int i = 0; i < IMG_COL; i++)
	{
		sum += *p++;
	}
	return sum / 188;

}

void CannyEage(void)
{

	for (int i = UP_EAGE; i <= DOWN_EAGE; i++)
	{
		for (int j = LEFT_EAGE; j <= RIGHT_EAGE; j++)
		{
			ImageEage[i][j] = GetEageThreshold(i, j);
		}
	}

	LightThreshold = GetAveGray(DOWN_EAGE);
	string.Format("\r\n AveGray = %d \r\n", LightThreshold); PrintDebug(string);
	//DoubleTh(ImageEage[0]);
	PressEage(ImageEage[0]);

}


#define WHITE			255						//宏定义白
#define BLACK			0						//宏定义黑
#define RED				128						//宏定义红
#define GREEN			100						//宏定义绿
#define BLUE			254						//宏定义蓝



/*-------------Canny算法检测边缘---------------LQ--
==============================================LQ=
 * Gx:
 *     1  -1
 *     1  -1
 *---------------
 * Gy:
 *    -1  -1
 *     1   1
*----------------------------------------
% 输入：
% org：图像，如果不是灰度图转成灰度图
% lowThr：低阈值
% 输出：
% ImgEdge： 两个偏导的平方差，反映了边缘的强度
% Sector：将方向分为3个区域，具体如下
% ------->x
% |3 2 1
% |0 X 0
% |1 2 3
% V
% y
*---------------------------------------*/



//===========为了避免图像本来存在的特殊颜色影响，先对数组进行处理=========//
//===========方法：当检测Imagedata为特殊色试，对它的值做+1处理============//
void SpecialColorPuls()
{
	int i, j;
	for (i = 0; i < IMG_ROWS; i++)
		for (j = 0; j < IMG_COLS; j++)
		{
			if (ImageData[i][j] == RED || ImageData[i][j] == GREEN || ImageData[i][j] == WHITE || ImageData[i][j] == BLUE)
				ImageData[i][j] = ImageData[i][j] - 2;
			else
				if (ImageData[i][j] == BLACK)
					ImageData[i][j] = ImageData[i][j] + 1;
		}


}


//=========canny算法找边界===================LQ==
void CannyEdgeTest(unsigned char org[IMG_ROWS][IMG_COLS], uint8 lowThr)
{
	uint8 Sector[IMG_ROWS][IMG_COLS] = { 0 }; //像素梯度所在扇区
	float A[IMG_ROWS][IMG_COLS] = { 0 };
	float Temp[9] = { 0 }; //双阈值检测临时数组
	//float TemMax = 0; //临时最大值                                                                    
	//float theta = 0; //梯度方向
	int Xg = 0; //X方向梯度
	int Yg = 0; //Y方向梯度

	uint8 x, y;
	uint8 ratio = 2; //高低阈值检测比例
	uint8 highThr = 50; // ratio*lowThr; //高阈值
	uint8 loss = 2; //非极大值抑制补偿,增加边缘

	//==========求解梯度，幅度和方向===========
	for (y = UP_EAGE; y < IMG_ROWS - 1; y++) //最下面一行不处理
	{
		for (x = 0; x < IMG_COLS - 1; x++) //最上一列不处理
		{
			Xg = org[y][x] + org[y + 1][x] - org[y][x + 1] - org[y + 1][x + 1]; //计算X方向梯度
			Yg = -org[y][x] + org[y + 1][x] - org[y][x + 1] + org[y + 1][x + 1]; //计算Y方向梯度
			A[y][x] = Fsqrt(Xg * Xg + Yg * Yg); //求幅值，快速开平方算法
			Sector[y][x] = Atan2(Yg, Xg);  //求梯度方向分区 			
		} //end of for(i)	
	} //end of for(j)

	//===============非极大值抑制================
	for (y = UP_EAGE + 1; y < IMG_ROWS - 1; y++)
	{
		for (x = 0; x < IMG_COLS - 1; x++)
		{
			if (0 == Sector[y][x]) //水平方向
			{
				if ((A[y][x] > A[y][x + 1] - loss) && A[y][x] > (A[y][x - 1] - loss))
				{
					A[y][x] = A[y][x];
				}
				else
				{
					A[y][x] = 0;
				}

			} // end of 0
			if (1 == Sector[y][x]) //右上、左下
			{
				if (A[y][x] > (A[y - 1][x + 1] - loss) && A[y][x] > (A[y + 1][x - 1] - loss))
				{
					A[y][x] = A[y][x];
				}
				else
				{
					A[y][x] = 0;
				}
			} //end of 1
			if (2 == Sector[y][x]) //竖直方向
			{
				if (A[y][x] > (A[y - 1][x] - loss) && A[y][x] > (A[y + 1][x] - loss))
				{
					A[y][x] = A[y][x];
				}
				else
				{
					A[y][x] = 0;
				}
			} //end of 2
			if (3 == Sector[y][x]) //左上、右下
			{
				if (A[y][x] > (A[y - 1][x - 1] - loss) && A[y][x] > (A[y + 1][x + 1] - loss))
				{
					A[y][x] = A[y][x];
				}
				else
				{
					A[y][x] = 0;
				}
			} //end of 3	
		} //end of for(x)
	} // end of for(y)

	//=============双阈值检测=================
	for (y = UP_EAGE + 1; y < IMG_ROWS - 2; y++)
	{
		for (x = 1; x < IMG_COLS - 1; x++)
		{
			if (A[y][x] < lowThr) //低于低阈值
			{
				ImageEage[y][x] = 0;
			}
			else if (A[y][x] > highThr) //高于高阈值
			{
				ImageEage[y][x] = 1;
			}
			else
			{
				Temp[0] = A[y + 1][x - 1]; Temp[1] = A[y + 1][x]; Temp[2] = A[y + 1][x + 1];
				Temp[3] = A[y][x - 1];   Temp[4] = A[y][x];   Temp[5] = A[y][x + 1];
				Temp[6] = A[y - 1][x - 1]; Temp[7] = A[y - 1][x]; Temp[8] = A[y - 1][x + 1]; //3x3区域内点
				Quicksort(Temp, 9, 0, 8); //快速排序,低到高

				if (Temp[8] > highThr)
				{
					ImageEage[y][x] = 1;
					A[y][x] = 1;
				} //end of if
				else
				{
					ImageEage[y][x] = 0;
					A[y][x] = 0;
				} //end of if else	
			} //end of if else	
		} //end of for(x)
	} //end of for(y)
	for (x = 20; x < IMG_ROWS; x++)//此时边界已经全部确定，显示边界
		for (y = 0; y < IMG_COLS; y++)
		{
			if (ImageEage[x][y] == 1)
				ImageData[x][y] = 254;
		}


} //end of 


//======== 快速开方 原理不懂，快！好用！=======LQ==
float Fsqrt(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int*)& x;         // evil floating point bit level hacking
	//i = 0x5f3759df - (i >> 1);  // what the fuck?
	i = 0X5F3504F3 - (i >> 1); //精度更高
	x = *(float*)& i;
	x = x * (1.5f - (xhalf * x * x));
	return 1 / x;
} //end of function Fsqrt

//=============反正切函数============LQ==
/********************************************
*  直接计算几个分界点的值,只需判断 y/x 的值所
* 对应的角度在哪个范围即可，不用实时求atan
*--------------------------------------------
*    角度范围        |         弧度范围
*--------------------------------------------
* 0     ~ 22.5  ------> 0         ~ 0.3926990
* 22.5  ~ 67.5  ------> 0.3926990 ~ 1.1780972
* 67.5  ~ 112.5 ------> 1.1780972 ~ 1.9634954
* 112.5 ~ 157.5 ------> 1.9634954 ~ 2.7488935
* 157.5 ~ 180   ------> 2.7488935 ~ 3.1415926
*--------------------------------------------
*         y/x值对应弧度
*  0          ----  0.41421356  水平方向
*  0.41421347 ----  2.41421356  右上、左下
*  2.41421326 ---- -2.41421356  竖直方向
* -2.41421362 ---- -0.41421356  左上、右下
* -0.41421365 ----  0           水平方向
********************************************/
uint8 Atan2(float y, float x)
{
	float tanNum;
	uint8 alpha; //返回角度
	tanNum = y / x;
	if (tanNum > -0.41421365 && tanNum < 0.41421356)
	{
		alpha = 0; //水平方向
	}
	else if (tanNum >= 0.41421356 && tanNum < 2.41421356)
	{
		alpha = 1; //右上、左下
	}
	else if (tanNum <= -0.41421356 && tanNum > -2.41421362)
	{
		alpha = 3; //左上、右下
	}
	else
	{
		alpha = 2; //竖直方向
	}

	return alpha; //方向
} //end of function Atan2


void Quicksort(float array[], uint8 maxlen, uint8 begin, uint8 end)
{
	uint8 i, j;

	if (begin < end)
	{
		i = begin + 1;  // 将array[begin]作为基准数，因此从array[begin+1]开始与基准数比较！  
		j = end;        // array[end]是数组的最后一位  

		while (i < j)
		{
			if (array[i] > array[begin])  // 如果比较的数组元素大于基准数，则交换位置。  
			{
				Swap(&array[i], &array[j]);  // 交换两个数  
				j--;
			} //end of if  
			else
			{
				i++;  // 将数组向后移一位，继续与基准数比较。  
			} //end of if else
		} //end of while 

		/* -------------------------------------------------------------------------------------------------
		 *跳出while循环后，i = j。
		 * 此时数组被分割成两个部分  -->  array[begin+1] ~ array[i-1] < array[begin]
		 *                           -->  array[i+1] ~ array[end] > array[begin]
		 * 这个时候将数组array分成两个部分，再将array[i]与array[begin]进行比较，决定array[i]的位置。
		 * 最后将array[i]与array[begin]交换，进行两个分割部分的排序！以此类推，直到最后i = j不满足条件就退出！
		/*-------------------------------------------------------------------------------------------------*/

		if (array[i] >= array[begin])  // 这里必须要取等“>=”，否则数组元素由相同的值时，会出现错误！  
		{
			i--;
		}

		Swap(&array[begin], &array[i]);  // 交换array[i]与array[begin]  

		Quicksort(array, maxlen, begin, i);
		Quicksort(array, maxlen, j, end);
	} //end of if(begin < end) 
}  //end of function Quicksort 

//==========交换两个数的值======LQ===
void Swap(float* a, float* b)
{
	float temp;

	temp = *a;
	*a = *b;
	*b = temp;
} //end of function Swap


