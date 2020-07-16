//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-05-11
//		Description :					�첽���ݷ�����
//
//		History:						Author									Date										Description
//										������									2020-05-11									����
//

#ifndef ASYNCHRONOUS_SENDER_H
#define ASYNCHRONOUS_SENDER_H

namespace mq
{
	namespace module
	{
		class AsynchronousSender
		{
		public:
			AsynchronousSender(void);
			virtual ~AsynchronousSender(void);

		public:
			//��������
			//@s : SOCKET���ֵ
			//@data : ��������
			//@bytes : �����ֽ���
			//@forcemore : ǿ�Ʒְ���ʶ
			//             true == forcemoreʱdata���������۴�С����Ϊ�ְ�
			//@Return : ʵ�ʷ����ֽ���
			const unsigned long long sendData(
				void* s = nullptr, 
				const void* data = nullptr, 
				const unsigned long long bytes = 0,
				const bool forcemore = false);

		private:
			//���ͷְ�����
			//@s : SOCKET���ֵ
			//@data : ��������
			//@bytes : �����ֽ���
			//@more : ���ݷְ�������ʶ,true��ʶ�������ݴ�����,false��ʶ���ݷ������
			//@Return : ������ֵ
			virtual const unsigned long long sendPartialData(
				void* s = nullptr, 
				const void* data = nullptr, 
				const unsigned long long bytes = 0, 
				const bool more = false);
		};//class AsynchronousSender
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_SENDER_H
