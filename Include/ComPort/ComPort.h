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

#ifndef BASE_COM_COM_PORT_H
#define BASE_COM_COM_PORT_H

#include "boost/asio.hpp"
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
using ComPortPtr = boost::shared_ptr<boost::asio::serial_port>;

namespace base
{
	namespace com
	{
		typedef boost::function<void(const char*, const int)> ComPortDataNotificationCallback;

		class ComPort
		{
		public:
			ComPort(ComPortDataNotificationCallback callback = nullptr);
			~ComPort(void);

		public:
			//打开串口
			//@idx : 串口号
			//@baudrate : 波特率
			//@Return : 错误码
			int openPort(
				const int idx = 0, 
				const int baudrate = 4800);

			//关闭串口
			//@Return : 错误码
			int closePort(void);

		private:
			void asyncReadDataNotificationCallback(
				ComPortPtr cpp, 
				const boost::system::error_code e,
				const int bytes = 0);
			void processDataReceiveWorkerThread(void);

		private:
			boost::asio::io_service ios;
			ComPortDataNotificationCallback comPortDataNotificationCallback;
			//串口数据缓存
			enum
			{
				eMaxPort = 16,
				eMaxSize = 1//500 * 1024
			};
			char comPortDataCache[eMaxSize];
		};//class ComPort
	}//namespace com
}//namespace base

#endif//BASE_COM_COM_PORT_H
