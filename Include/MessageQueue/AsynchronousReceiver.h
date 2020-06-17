//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-11
//		Description :					�첽���ݽ�������
//
//		History:						Author									Date										Description
//										������									2020-05-11									����
//

#ifndef ASYNCHRONOUS_RECEIVER_H
#define ASYNCHRONOUS_RECEIVER_H

namespace mq
{
	namespace module
	{
		class AsynchronousReceiver
		{
		public:
			AsynchronousReceiver(void);
			virtual ~AsynchronousReceiver(void);

		public:
			//��������
			//@s : ���ݽ��յ�socket���ֵ
			//@data : ��������
			//@bytes : �����ֽ���
			//@Return : >0��ʾʵ�ʽ����ֽ���,<0��ʾ������ֵ
			const int receiveData(
				void* s = nullptr, char* data = nullptr, const unsigned int bytes = 0);
		};//class AsynchronousReceiver
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_RECEIVER_H
