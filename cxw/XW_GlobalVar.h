#ifndef __XW_GLOBALVAR_H__
#define __XW_GLOBALVAR_H__


//=============================AlgorithmDLL.h==================================//
//������
extern char* pFileName;
extern FILE *pfile ;

//��ӡ�����ã���void PrintDebug(CString ss)����ʹ��

/*�������ļ�����
CString string="";
����string.Format("g_ERROR = %d\r\n",g_ERROR);PrintDebug(string);
���У�string���Լ��������֣���ÿ���ļ��е������費ͬ,���GetError����ĵ���*/
extern CString string ;
//extern CString string;





//=============================ImageDeal.h==================================//
#define IMG_ROWS 120      //ͼ���������������
#define IMG_COLS 188      //ͼ���������������


extern unsigned char ImageData[120][188];   //ͼ������
extern unsigned char ImgBuff[24 * 60];     //����ͷ�ɼ�����


#endif