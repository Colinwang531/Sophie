//
//		Copyright :						@2020, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2020-10-03
//		Description :					串口类
//
//		History:						Author									Date										Description
//										王科威									2020-10-03									创建
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
			//打开串口
			//@number : 串口号
			//@baudrate : 波特率
			//@Return : 错误码
			int openCom(
				const int number = 0, 
				const int baudrate = 4800);

			//关闭串口
			//@Return : 错误码
			int closeCom(void);

		protected:
			//数据读取通知
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
