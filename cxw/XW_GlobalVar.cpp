#include "StdAfx.h"
#include "XW_include.h"
//#include "ImageVar.h"
//=============================AlgorithmDLL.h==================================//
//不更改
char* pFileName = "中心线提取信息.txt";
FILE *pfile = fopen(pFileName, "w");

//打印数据用，与void PrintDebug(CString ss)搭配使用

/*在其他文件定义
CString string="";
调用string.Format("g_ERROR = %d\r\n",g_ERROR);PrintDebug(string);
其中，string可自己更改名字，但每个文件夹的名字需不同,详见GetError里面的调用*/
CString string = "";
//CString string = "";

//=============================ImageDeal.h==================================//
unsigned char ImageData[120][188];          //图像数据
unsigned char ImgBuff[24 * 60];					//摄像头采集数据



