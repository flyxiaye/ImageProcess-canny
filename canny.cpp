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




//��Ե�������
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



//˫��ֵ���
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

//�жϱ�Ե����
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

//���ƹ�������Ե
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



////��ʾ�߽�
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

//ͳ��ĳһ�еĻҶ�ƽ��ֵ
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


#define WHITE			255						//�궨���
#define BLACK			0						//�궨���
#define RED				128						//�궨���
#define GREEN			100						//�궨����
#define BLUE			254						//�궨����



/*-------------Canny�㷨����Ե---------------LQ--
==============================================LQ=
 * Gx:
 *     1  -1
 *     1  -1
 *---------------
 * Gy:
 *    -1  -1
 *     1   1
*----------------------------------------
% ���룺
% org��ͼ��������ǻҶ�ͼת�ɻҶ�ͼ
% lowThr������ֵ
% �����
% ImgEdge�� ����ƫ����ƽ�����ӳ�˱�Ե��ǿ��
% Sector���������Ϊ3�����򣬾�������
% ------->x
% |3 2 1
% |0 X 0
% |1 2 3
% V
% y
*---------------------------------------*/



//===========Ϊ�˱���ͼ�������ڵ�������ɫӰ�죬�ȶ�������д���=========//
//===========�����������ImagedataΪ����ɫ�ԣ�������ֵ��+1����============//
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


//=========canny�㷨�ұ߽�===================LQ==
void CannyEdgeTest(unsigned char org[IMG_ROWS][IMG_COLS], uint8 lowThr)
{
	uint8 Sector[IMG_ROWS][IMG_COLS] = { 0 }; //�����ݶ���������
	float A[IMG_ROWS][IMG_COLS] = { 0 };
	float Temp[9] = { 0 }; //˫��ֵ�����ʱ����
	//float TemMax = 0; //��ʱ���ֵ                                                                    
	//float theta = 0; //�ݶȷ���
	int Xg = 0; //X�����ݶ�
	int Yg = 0; //Y�����ݶ�

	uint8 x, y;
	uint8 ratio = 2; //�ߵ���ֵ������
	uint8 highThr = 50; // ratio*lowThr; //����ֵ
	uint8 loss = 2; //�Ǽ���ֵ���Ʋ���,���ӱ�Ե

	//==========����ݶȣ����Ⱥͷ���===========
	for (y = UP_EAGE; y < IMG_ROWS - 1; y++) //������һ�в�����
	{
		for (x = 0; x < IMG_COLS - 1; x++) //����һ�в�����
		{
			Xg = org[y][x] + org[y + 1][x] - org[y][x + 1] - org[y + 1][x + 1]; //����X�����ݶ�
			Yg = -org[y][x] + org[y + 1][x] - org[y][x + 1] + org[y + 1][x + 1]; //����Y�����ݶ�
			A[y][x] = Fsqrt(Xg * Xg + Yg * Yg); //���ֵ�����ٿ�ƽ���㷨
			Sector[y][x] = Atan2(Yg, Xg);  //���ݶȷ������ 			
		} //end of for(i)	
	} //end of for(j)

	//===============�Ǽ���ֵ����================
	for (y = UP_EAGE + 1; y < IMG_ROWS - 1; y++)
	{
		for (x = 0; x < IMG_COLS - 1; x++)
		{
			if (0 == Sector[y][x]) //ˮƽ����
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
			if (1 == Sector[y][x]) //���ϡ�����
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
			if (2 == Sector[y][x]) //��ֱ����
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
			if (3 == Sector[y][x]) //���ϡ�����
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

	//=============˫��ֵ���=================
	for (y = UP_EAGE + 1; y < IMG_ROWS - 2; y++)
	{
		for (x = 1; x < IMG_COLS - 1; x++)
		{
			if (A[y][x] < lowThr) //���ڵ���ֵ
			{
				ImageEage[y][x] = 0;
			}
			else if (A[y][x] > highThr) //���ڸ���ֵ
			{
				ImageEage[y][x] = 1;
			}
			else
			{
				Temp[0] = A[y + 1][x - 1]; Temp[1] = A[y + 1][x]; Temp[2] = A[y + 1][x + 1];
				Temp[3] = A[y][x - 1];   Temp[4] = A[y][x];   Temp[5] = A[y][x + 1];
				Temp[6] = A[y - 1][x - 1]; Temp[7] = A[y - 1][x]; Temp[8] = A[y - 1][x + 1]; //3x3�����ڵ�
				Quicksort(Temp, 9, 0, 8); //��������,�͵���

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
	for (x = 20; x < IMG_ROWS; x++)//��ʱ�߽��Ѿ�ȫ��ȷ������ʾ�߽�
		for (y = 0; y < IMG_COLS; y++)
		{
			if (ImageEage[x][y] == 1)
				ImageData[x][y] = 254;
		}


} //end of 


//======== ���ٿ��� ԭ�������죡���ã�=======LQ==
float Fsqrt(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int*)& x;         // evil floating point bit level hacking
	//i = 0x5f3759df - (i >> 1);  // what the fuck?
	i = 0X5F3504F3 - (i >> 1); //���ȸ���
	x = *(float*)& i;
	x = x * (1.5f - (xhalf * x * x));
	return 1 / x;
} //end of function Fsqrt

//=============�����к���============LQ==
/********************************************
*  ֱ�Ӽ��㼸���ֽ���ֵ,ֻ���ж� y/x ��ֵ��
* ��Ӧ�ĽǶ����ĸ���Χ���ɣ�����ʵʱ��atan
*--------------------------------------------
*    �Ƕȷ�Χ        |         ���ȷ�Χ
*--------------------------------------------
* 0     ~ 22.5  ------> 0         ~ 0.3926990
* 22.5  ~ 67.5  ------> 0.3926990 ~ 1.1780972
* 67.5  ~ 112.5 ------> 1.1780972 ~ 1.9634954
* 112.5 ~ 157.5 ------> 1.9634954 ~ 2.7488935
* 157.5 ~ 180   ------> 2.7488935 ~ 3.1415926
*--------------------------------------------
*         y/xֵ��Ӧ����
*  0          ----  0.41421356  ˮƽ����
*  0.41421347 ----  2.41421356  ���ϡ�����
*  2.41421326 ---- -2.41421356  ��ֱ����
* -2.41421362 ---- -0.41421356  ���ϡ�����
* -0.41421365 ----  0           ˮƽ����
********************************************/
uint8 Atan2(float y, float x)
{
	float tanNum;
	uint8 alpha; //���ؽǶ�
	tanNum = y / x;
	if (tanNum > -0.41421365 && tanNum < 0.41421356)
	{
		alpha = 0; //ˮƽ����
	}
	else if (tanNum >= 0.41421356 && tanNum < 2.41421356)
	{
		alpha = 1; //���ϡ�����
	}
	else if (tanNum <= -0.41421356 && tanNum > -2.41421362)
	{
		alpha = 3; //���ϡ�����
	}
	else
	{
		alpha = 2; //��ֱ����
	}

	return alpha; //����
} //end of function Atan2


void Quicksort(float array[], uint8 maxlen, uint8 begin, uint8 end)
{
	uint8 i, j;

	if (begin < end)
	{
		i = begin + 1;  // ��array[begin]��Ϊ��׼������˴�array[begin+1]��ʼ���׼���Ƚϣ�  
		j = end;        // array[end]����������һλ  

		while (i < j)
		{
			if (array[i] > array[begin])  // ����Ƚϵ�����Ԫ�ش��ڻ�׼�����򽻻�λ�á�  
			{
				Swap(&array[i], &array[j]);  // ����������  
				j--;
			} //end of if  
			else
			{
				i++;  // �����������һλ���������׼���Ƚϡ�  
			} //end of if else
		} //end of while 

		/* -------------------------------------------------------------------------------------------------
		 *����whileѭ����i = j��
		 * ��ʱ���鱻�ָ����������  -->  array[begin+1] ~ array[i-1] < array[begin]
		 *                           -->  array[i+1] ~ array[end] > array[begin]
		 * ���ʱ������array�ֳ��������֣��ٽ�array[i]��array[begin]���бȽϣ�����array[i]��λ�á�
		 * ���array[i]��array[begin]���������������ָ�ֵ������Դ����ƣ�ֱ�����i = j�������������˳���
		/*-------------------------------------------------------------------------------------------------*/

		if (array[i] >= array[begin])  // �������Ҫȡ�ȡ�>=������������Ԫ������ͬ��ֵʱ������ִ���  
		{
			i--;
		}

		Swap(&array[begin], &array[i]);  // ����array[i]��array[begin]  

		Quicksort(array, maxlen, begin, i);
		Quicksort(array, maxlen, j, end);
	} //end of if(begin < end) 
}  //end of function Quicksort 

//==========������������ֵ======LQ===
void Swap(float* a, float* b)
{
	float temp;

	temp = *a;
	*a = *b;
	*b = temp;
} //end of function Swap


