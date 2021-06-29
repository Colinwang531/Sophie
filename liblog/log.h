//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-03-19
//		Description :					日志类
//
//		History:						Author									Date										Description
//										王科威									 2021-03-19									 创建
//

#ifndef FRAMEWORK_LIBLOG_LOG_H
#define FRAMEWORK_LIBLOG_LOG_H

namespace framework
{
    namespace liblog
    {
        typedef enum class tagLogLevel_t : int
        {
            LOG_LEVEL_INFO = 0,
            LOG_LEVEL_WARNING,
            LOG_LEVEL_ERROR
        }LogLevel;

        class Log
        {
        public:
            Log(void);
            ~Log(void);

        public:
            //初始化
			//@name : 调用该接口应用程序的执行路径，例如:/usr/sbin/*.*
            //@Return : 错误码
            int init(
                const char* name = nullptr);

            //释放
            void uninit(void);

            //写日志
            //@level : 等级值
            //@format : 日志格式
            //@... : 参数列表
            //@Return : 错误码
            int write(
                const LogLevel level = LogLevel::LOG_LEVEL_INFO,
                const char* format = nullptr,
                ...);
        };//class Log
    }//namespace liblog
}//namespace framework

#endif//FRAMEWORK_LIBLOG_LOG_H