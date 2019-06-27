#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "GlobalVar.h"

const int MidOffset[] = {
	93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93,
};
unsigned char ImageEage[IMG_ROW][IMG_COL];
int ML[IMG_ROW], LL[IMG_ROW], RL[IMG_ROW];   //�����Ե����Ϣ����
int ML_Count;									//������Ч��
SpecialPoint LeftPnt, RightPnt;					//���������������Ϣ
int DiffThreshold = 25;							//��Ե�����ֵ
int DarkThreshold = 85;                         //������ֵ��·�ϣ�
int BrightThreshold = 15;                       //������ֵ���µ���
int LightThreshold = 80;						//ȥ�߹�������ֵ
int LightThreshold2 = 150;						//ȥ�߹�������ֵ2
int FindLineType = 0;							//�Ƿ����߹��˲���־
int g_RoadType = 0;
int ErrorFlag = 0;								//�����־λ
//1˫��ʮ�ִ���  2����Ϊбʮ��  3��Ұ��ʧ����  4���ߴ���  5˫��ʮ��������  6ƽʮ��������
int LastMiddleLine = 0;

int SpeedRow = 0;			//���ٱ߽�����

int CircleFlag = 1;
int CircleState = 2;

int LeftIntLine = 0;		//��¼����������
int RightIntLine = 0;		//��¼����������

int Img_SpecialElemFlag = 0;    //����Ԫ�ر�־
int Img_BrokenFlag = 0;			//��·��־
int Img_BlockFlag = 0;			//·�ϱ�־
int Img_StopLineFlag = 0;		//ͣ���߱�־
int Img_RampFlag = 0;			//�µ���־

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
	//����ĳЩ�ط����˹ؼ�����
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


//��ʱ
LARGE_INTEGER f;
double dFreq;
LARGE_INTEGER start, end, end2;
