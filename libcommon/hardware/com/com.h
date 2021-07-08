//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-07-06
//		Description :					串口类
//
//		History:						Author									Date										Description
//										王科威									 2021-07-06									 创建
//

#ifndef FRAMEWORK_LIBCOMMON_HARDWARE_COM_H
#define FRAMEWORK_LIBCOMMON_HARDWARE_COM_H

#include <functional>

namespace framework
{
	namespace libcommon
	{
		namespace hardware
		{
			typedef std::function<void(const char*, int)> ComReadDataHandler;
			class ICom;

			class Com
			{
			public:
				Com(void);
				~Com(void);

			public:
				//启动
				//@number : 串口号
				//@baudrate : 波特率
				//@handler : 数据回调
				//@Return : 错误码
				int start(
					const int number = 0, 
					const int baudrate = 4800,
					ComReadDataHandler handler = nullptr);

				//停止
				//@Return : 错误码
				int stop(void);

			private:
				ICom* com;
			};//class Com
		}//namespace hardware
	}//namespace libcommon
}//namespace framework

#endif//FRAMEWORK_LIBCOMMON_HARDWARE_COM_H
