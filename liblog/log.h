//
//		Copyright :						@2021, ***, All Rights Reserved
//
//		Author :						王科威
//		E-mail :						wangkw531@icloud.com
//		Date :							2021-03-19
//		Description :					日志写入库
//
//		History:						Author									Date										Description
//										王科威									 2021-03-19									 创建
//

#ifndef FRAMEWORK_LIB_LOG_H
#define FRAMEWORK_LIB_LOG_H

namespace framework
{
    namespace liblog
    {
        typedef enum class tagLogError_t : int
        {
            LOG_ERROR_SUCCESS = 0,
            LOG_ERROR_INVALID_PARAMETER = -1,
            LOG_ERROR_INIT_FAILED = -2
        }LogError;

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
                virtual ~Log(void);

            public:
                //初始化
				//@name : 调用该接口应用程序的执行路径，例如:/usr/sbin/xxx
                //        注意一定没有后缀名
                //@dir : 日志文件存储目录
                //@Return : 错误码
                virtual int init(
                    const char* name = nullptr,
                    const char* dir = nullptr);

                //释放
                virtual void uninit(void);

                //写日志
                //@level : 等级值
                //@format : 日志格式
                //@... : 参数列表
                //@Return : 错误码
                virtual int write(
                    const LogLevel level = LogLevel::LOG_LEVEL_INFO,
                    const char* format = nullptr,
                    ...);
        };//class Log
    }//namespace liblog
}//namespace framework

#endif//FRAMEWORK_LIB_LOG_H