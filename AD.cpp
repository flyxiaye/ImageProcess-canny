#include "AD.h"
#include "stdafx.h"
#include "AlgorithmDLL.h"
#include "string.h"
#include "./cxw/XW_include.h"

#include "BasicFunction.h"
#include "GlobalVar.h"
#include "FillSpecialLine.h"
#include "FindLineFun.h"
#include "Judge.h"
#include "exp.h"

int ind_left, ind_right, ind_mid;	//��¼���
int ind_leftmax, ind_leftmin, ind_rightmax, ind_rightmin, ind_midmax, ind_midmin;	//�����ֵ
int left_norm, right_norm, mid_norm;
int g_adflag = 0;
int ad_error_1 = 0, ad_error_2 = 0;

void ind_acq(void)                         //��вɼ�
{
	//�ɼ����
	/*ind_left = ad_riyueqsort(ADC0_SE9, ADC_12bit, 10);
	ind_right = ad_riyueqsort(ADC0_SE8, ADC_12bit, 10);
	ind_mid = ad_riyueqsort(ADC0_SE6, ADC_12bit, 10);*/
	//���ֵ�޷�
	ind_left = MIN(ind_left, 3400);
	ind_left = MAX(ind_left, 20);
	ind_right = MIN(ind_right, 3400);
	ind_right = MAX(ind_right, 20);
	ind_mid = MIN(ind_mid, 3400);
	ind_mid = MAX(ind_mid, 20);
}

void ind_norm_maxmin(void)                //���ҵ�������С
{
	ind_leftmax = MAX(ind_leftmax, ind_left);
	ind_leftmin = MIN(ind_leftmin, ind_left);
	ind_rightmax = MAX(ind_rightmax, ind_right);
	ind_rightmin = MIN(ind_rightmin, ind_right);
	ind_midmax = MAX(ind_midmax, ind_mid);
	ind_midmin = MIN(ind_midmin, ind_mid);
}
void ind_norm(void)                           //�ó���һֵ��
{
	left_norm = (float)((ind_left - ind_leftmin)) / (float)((ind_leftmax - ind_leftmin));
	right_norm = (float)((ind_right - ind_rightmin)) / (float)((ind_rightmax - ind_rightmin));
	mid_norm = (float)((ind_mid - ind_midmin)) / (float)((ind_midmax - ind_midmin));
}
void ind(void)
{
	ind_acq();
	if (g_adflag == 0)
	{
		ind_norm_maxmin();
	}
	ind_norm();
}

void ind_error(void)
{
	if (g_adflag == 1)
	{


		ad_error_1 = (left_norm - right_norm) / (left_norm + right_norm);

		//g_steer_error = g_steer_p * ad_error_1 + g_steer_d * (ad_error_1 - ad_error_2);

		ad_error_2 = ad_error_1;
	}
	else;
}