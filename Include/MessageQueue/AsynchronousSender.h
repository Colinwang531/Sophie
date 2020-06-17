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
			AsynchronousSender(const unsigned int bytes = mtu);
			virtual ~AsynchronousSender(void);

		public:
			//��������
			//@s : ���ݷ��͵�socket���ֵ
			//@data : ��������
			//@bytes : �����ֽ���
			//@more : ���ݷ��ͽ�����ʶ,true��ʶ�������ݴ�����,false��ʶ���ݷ������
			//@Return : ʵ�ʷ����ֽ���
			const unsigned int sendData(
				void* s = nullptr, const char* data = nullptr, const unsigned int bytes = 0, const bool more = false);

		private:
			//���Ͳ������
			//@s : ���ݷ��͵�socket���ֵ
			//@data : ��������
			//@bytes : �����ֽ���
			//@more : ���ݷ��ͽ�����ʶ,true��ʶ�������ݴ�����,false��ʶ���ݷ������
			//@Return : ������ֵ
			virtual const unsigned int sendDisassemblyData(
				void* s = nullptr, const char* data = nullptr, const unsigned int bytes = 0, const bool more = false);

		private:
			const unsigned int disassemblyDataBytes;
			static const unsigned int mtu = 512;
		};//class MQSender
	}//namespace module
}//namespace mq

#endif//ASYNCHRONOUS_SENDER_H
