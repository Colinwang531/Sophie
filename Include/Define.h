//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-04-27
//		Description :					�����ֶ�ֵ����
//
//		History:						Author									Date										Description
//										������									2020-04-27									����
//										������									2020-05-13									�޸������淶
//										������									2020-07-02									��������Ǩ������ļ�
//

#ifndef DEFINE_H
#define DEFINE_H

#include <vector>

//�˿ں�ʹ�÷�Χ
static const unsigned short gMinPortNumber = 5000;
static const unsigned short gMaxPortNumber = 65530;

//MTU
static const unsigned int gMaxTransmitUnit = 512;

//���̼�ͨ��ID��ʶ
static const char* gInprocCommID = "inproc://AsynchronousServer";

#endif//DEFINE_H
