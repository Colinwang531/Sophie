//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						������
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-03
//		Description :					������
//
//		History:						Author									Date										Description
//										������									2020-10-03									����
//

#ifndef FRAMEWORK_HARDWARE_COM_H
#define FRAMEWORK_HARDWARE_COM_H

namespace framework
{
	namespace hardware
	{
		class Com
		{
		public:
			Com(void);
			virtual ~Com(void);

		public:
			//�򿪴���
			//@number : ���ں�
			//@baudrate : ������
			//@Return : ������
			int openCom(
				const int number = 0, 
				const int baudrate = 4800);

			//�رմ���
			//@Return : ������
			int closeCom(void);

		protected:
			//���ݶ�ȡ֪ͨ
			virtual void afterReceiveComDataNotification(
				const char* data = nullptr,
				const int bytes = 0) = 0;

		private:
			void processDataReceiveWorkerThread(void);

		private:
			int fd;
		};//class Com
	}//namespace hardware
}//namespace framework

#endif//FRAMEWORK_HARDWARE_COM_H
