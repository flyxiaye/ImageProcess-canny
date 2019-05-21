#ifndef __XW_GLOBALVAR_H__
#define __XW_GLOBALVAR_H__


//=============================AlgorithmDLL.h==================================//
//不更改
extern char* pFileName;
extern FILE *pfile ;

//打印数据用，与void PrintDebug(CString ss)搭配使用

/*在其他文件定义
CString string="";
调用string.Format("g_ERROR = %d\r\n",g_ERROR);PrintDebug(string);
其中，string可自己更改名字，但每个文件夹的名字需不同,详见GetError里面的调用*/
extern CString string ;
//extern CString string;





//=============================ImageDeal.h==================================//
#define IMG_ROWS 120      //图像的总行数，即高
#define IMG_COLS 188      //图像的总列数，即宽


extern unsigned char ImageData[120][188];   //图像数组
extern unsigned char ImgBuff[24 * 60];     //摄像头采集数据


#endif