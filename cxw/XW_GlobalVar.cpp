#include "StdAfx.h"
#include "XW_include.h"
//#include "ImageVar.h"
//=============================AlgorithmDLL.h==================================//
//������
char* pFileName = "��������ȡ��Ϣ.txt";
FILE *pfile = fopen(pFileName, "w");

//��ӡ�����ã���void PrintDebug(CString ss)����ʹ��

/*�������ļ�����
CString string="";
����string.Format("g_ERROR = %d\r\n",g_ERROR);PrintDebug(string);
���У�string���Լ��������֣���ÿ���ļ��е������費ͬ,���GetError����ĵ���*/
CString string = "";
//CString string = "";

//=============================ImageDeal.h==================================//
unsigned char ImageData[120][188];          //ͼ������
unsigned char ImgBuff[24 * 60];					//����ͷ�ɼ�����



